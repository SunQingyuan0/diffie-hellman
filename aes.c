#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <tommath.h>
#include <tomcrypt.h>

/*
AES_GCM的输入三要素：plaintext（必选）、tag（可选）、iv（必选）
AES_GCM的输出三要素：ciphertext（必选）、tag（必选）
*/
//加密，使用tomath库，AES_GCM256加密
void encrypt(unsigned char *key, unsigned char *plain_text, int pt_len, unsigned char *cipher_text, unsigned char *iv,
             int iv_len, unsigned char *tag, unsigned long tag_len)
{
    int key_len = 32; // 选取256-bit key
    register_cipher(&aes_desc);
    int err = gcm_memory(find_cipher("aes"), (const unsigned char *) key, key_len, (const unsigned char *) iv, iv_len,
                         NULL, NULL, plain_text, pt_len, cipher_text, tag, &tag_len, GCM_ENCRYPT);
}

//解密，使用tomath库，AES_GCM256
int decrypt(unsigned char *key, unsigned char *plain_text, int ct_len, unsigned char *cipher_text, unsigned char *iv,
            int iv_len, unsigned char *tag, unsigned long tag_len)
{
    int key_len = 32; // 256-bit key
    register_cipher(&aes_desc);
    int err = gcm_memory(find_cipher("aes"), (const unsigned char *) key, key_len, (const unsigned char *) iv, iv_len,
                         NULL, NULL, plain_text, ct_len, cipher_text, tag, &tag_len, GCM_DECRYPT);
}
