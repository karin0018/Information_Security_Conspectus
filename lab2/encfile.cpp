// cryptoDemo.cpp : Defines the entry point for the console application.
// Windows: cl cryptoDemo.cpp
// Linux: gcc -o cryptoDemo cryptoDemo.cpp -lcrypto

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "openssl\aes.h"

#pragma comment(lib, "libeay32.lib")

#define MAXLEN 5000

void testAes(char flag[], char file_name[], char passwd[], int pwdLen) {
  int i, j, len, nLoop, nRes;
  char enString[MAXLEN];
  char deString[MAXLEN];

  memset(enString, 0, MAXLEN);
  memset(deString, 0, MAXLEN);

  unsigned char buf[16];
  unsigned char buf2[16];
  unsigned char aes_keybuf[32];
  AES_KEY aeskey;

  // ��ȡ�ļ�
  FILE *fp = fopen(file_name, "rb");
  if (!fp) {
    printf("the file can not open!\n");
    exit(-1);
  }

  // ׼��32�ֽ�(256λ)��AES�����ֽ�
  memset(aes_keybuf, 0x90, 32);
  if (pwdLen < 32) {
    len = pwdLen;
  } else {
    len = 32;
  }
  for (i = 0; i < len; i++) aes_keybuf[i] = passwd[i];

  printf("get the aes_key!\n");

  // ʵ�ּ��ܹ���
  if (flag[0] == 'e') {
    // ����������ֽڴ�
    AES_set_encrypt_key(aes_keybuf, 256, &aeskey);

    printf("encoding ----------\n");

    // ���ܺ���ļ���ŵ� encode.dat ��
    FILE *encode = fopen("encode.dat", "wb");
    if (!encode) {
      printf("the encode.dat can not open!\n");
      exit(-1);
    }

    int enlen = 0;  // �Ѽ��ܵ��ֽ���
    int lenth = 0;  // ��¼������ֽڸ���
    memset(buf, 0, 16);
    while (lenth = fread(buf, sizeof(char), 16, fp)) {
      // �� 16 �ֽ�Ϊ��λ��ȡ�ַ���������� buf ��
      if (lenth % 16) {
        // lenth < 16 ��������� 16 �ֽڵ��ַ���
        for (int i = lenth; i < 16; i++) {
          buf[i] = 0;
        }
        printf("the original key is %s,the lenth is %d\n", buf, lenth);
      }

      // ���ܺ���ַ������� buf2 ��
      AES_encrypt(buf, buf2, &aeskey);

      if (lenth % 16) {
        printf("the new key is %s", buf2);
      }

      for (int j = 0; j < 16; j++, enlen++) {
        // you need to save 16 bytes encode characters
        enString[enlen] = buf2[j];
      }
      memset(buf, 0, 16);
      memset(buf2, 0, 16);
    }
    enString[enlen] = 0;
    fwrite(enString, sizeof(char), enlen, encode);
  }

  // ���Ĵ��Ľ���
  else if (flag[0] == 'd') {
    printf("decoding----------\n");
    // ���ܺ���ļ����� decode.dat ��
    FILE *decode = fopen("decode.cpp", "wb");
    if (!decode) {
      printf("the decode.cpp can not open!\n");
      exit(-1);
    }

    AES_set_decrypt_key(aes_keybuf, 256, &aeskey);

    int delen = 0;  // ���ܳ������ַ���
    int lenth = 0;  // ʵ�ʶ�ȡ�������ַ�����
    int res = 0;    // record the last res

    memset(buf, 0, 16);
    while (lenth = fread(buf, sizeof(char), 16, fp)) {
      // ���ܺ���ַ����滺���� buf2 ��
      AES_decrypt(buf, buf2, &aeskey);

      for (int j = 0; j < 16; j++, delen++) {
        if (buf2[j] == 0) {
          res++;
        }
        deString[delen] = buf2[j];
      }

      memset(buf, 0, 16);
      memset(buf2, 0, 16);
    }
    deString[delen] = 0;
    fwrite(deString, sizeof(char), delen - res, decode);
  }
}

int main(int argc, char *argv[]) {
  // char flag[] = argv[1];
  // char file_name[] = argv[2];
  // char passwd[] = argv[3];

  testAes(argv[1], argv[2], argv[3], strlen(argv[3]));

  return 0;
}
