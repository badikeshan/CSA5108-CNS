/* Q22: Encrypt/decrypt in Cipher Block Chaining mode using S-DES (8-bit blocks). */
#include <stdio.h>
#include "sdes.h"

static void pbin(int v,int n){ for(int i=n-1;i>=0;i--){ putchar('0'+((v>>i)&1)); if(i%4==0&&i) putchar(' ');} }

static void cbc_encrypt(const int*p,int*c,int n,int k1,int k2,int iv){
    int prev=iv; for(int i=0;i<n;i++){ c[i]=sdes_enc(p[i]^prev,k1,k2); prev=c[i]; }
}
static void cbc_decrypt(const int*c,int*p,int n,int k1,int k2,int iv){
    int prev=iv; for(int i=0;i<n;i++){ p[i]=sdes_dec(c[i],k1,k2)^prev; prev=c[i]; }
}
int main(void){
    int key=0x1FD;           /* 01111 11101 */
    int iv=0xAA;             /* 1010 1010 */
    int pt[2]={0x01,0x23};   /* 0000 0001 0010 0011 */
    int ct[2], back[2], k1,k2;
    sdes_keys(key,&k1,&k2);
    cbc_encrypt(pt,ct,2,k1,k2,iv);
    cbc_decrypt(ct,back,2,k1,k2,iv);
    printf("Key        : 01111 11101\nIV         : 1010 1010\nPlaintext  : "); pbin(pt[0],8); putchar(' '); pbin(pt[1],8);
    printf("\nCiphertext : "); pbin(ct[0],8); putchar(' '); pbin(ct[1],8);
    printf("   (expected 1111 0100 0000 1011)\nDecrypted  : "); pbin(back[0],8); putchar(' '); pbin(back[1],8); putchar('\n');
    return 0;
}
