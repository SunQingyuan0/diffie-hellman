# Diffie-Hellman
Diffie-Hellman协议实现以及改进（中间人攻击）

编译：`make`

-   Step1. 客户端服务器通信

    服务器运行 `./server [server_ip]`

    客户端运行 `./client [server_ip] [client_ip]` ，这里客户端也绑定一下ip是为了后面测试方便

-   Step2. 中间人攻击
+ 终端相关命令
1. 修改中间人到客户端的源IP为服务器IP
iptables -t nat -A POSTROUTING -p tcp -s [mid_ip] -d [client_ip] -j SNAT --to-source [server_ip]
2. 修改客户端到服务器的目的IP为中间人IP
iptables -t nat -A PREROUTING -p tcp -s [client_ip] -d [server_ip] -j DNAT --to [mid_ip]
3. 开启本机的IP转发功能
+ 不开启的话攻击之后会使目标机断网而不是欺骗
+ 开启: echo 1 >/proc/sys/net/ipv4/ip_forward
+ 关闭: echo 0 >/proc/sys/net/ipv4/ip_forward

+ arp欺骗
+ 1.欺骗网关
arpspoof -i [mid_nic_name] -t [client_ip] [gateway_ip]
+ 2.欺骗服务器
arpspoof -i [mid_nic_name] -t [client_ip] [server_ip]

+ 步骤:
    中间人运行 `echo 1 >/proc/sys/net/ipv4/ip_forward` 开启IP转发

    `arpspoof -i [mid_nic_name] -t [client_ip] [gateway_ip]` 欺骗网关

    `arpspoof -i [mid_nic_name] -t [client_ip] [server_ip]`欺骗服务器

    客户端运行`iptables -t nat -A PREROUTING -p tcp -s [client_ip] -d [server_ip] -j DNAT --to [mid_ip]`

    服务器运行 `./server [server_ip]`

    中间人运行 `./mid [mid_ip] [server_ip]`

    客户端运行 `./client [server_ip] [client_ip]`

-   Step3. 预共享密钥改进 防御中间人攻击

+ 使用PSK时server和client必须配置相同的密钥
server发送一个随机字符串到客户端，客户端接受该字符串根据密钥对其进行一些操作(如hash)然后发送回server，server进行一次相同的操作 如果结果和客户端的相同就允许客户端连接，运行时，别的不变，改成 `server-pro` 和 `client-pro` 就行了

以上是单向的，即客户端->(中间人)->服务器，没加服务器对客户端的回应，要加的话把注释去掉加调用改改就行了