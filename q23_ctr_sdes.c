/* Q23: Encrypt/decrypt in Counter (CTR) mode using S-DES. Same routine encrypts and decrypts. */
#include <stdio.h>
#include "sdes.h"

static void pbin(int v,int n){ for(int i=n-1;i>=0;i--){ putchar('0'+((v>>i)&1)); if(i%4==0&&i) putchar(' ');} }

static void ctr(const int*in,int*out,int n,int k1,int k2,int counter){
    for(int i=0;i<n;i++) out[i]=in[i]^sdes_enc((counter+i)&0xFF,k1,k2);
}
int main(void){
    int key=0x1FD, k1,k2;     /* 01111 11101 */
    int pt[3]={0x01,0x02,0x04}, ct[3], back[3];
    sdes_keys(key,&k1,&k2);
    ctr(pt,ct,3,k1,k2,0x00);
    ctr(ct,back,3,k1,k2,0x00);
    printf("Key        : 01111 11101\nCounter    : 0000 0000\nPlaintext  : ");
    for(int i=0;i<3;i++){ pbin(pt[i],8); putchar(' ');}
    printf("\nCiphertext : "); for(int i=0;i<3;i++){ pbin(ct[i],8); putchar(' ');}
    printf("  (expected 0011 1000 0100 1111 0011 0010)\nDecrypted  : ");
    for(int i=0;i<3;i++){ pbin(back[i],8); putchar(' ');} putchar('\n');
    return 0;
}
