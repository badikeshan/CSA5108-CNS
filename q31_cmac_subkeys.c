/* Q31: CMAC subkey generation.  L = E(K, 0^b).  K1 = L<<1  (xor Rb if MSB(L)=1);  K2 = K1<<1 (xor Rb if MSB(K1)=1).
   a) Rb = 0x1B for 64-bit blocks (x^64+x^4+x^3+x+1), Rb = 0x87 for 128-bit blocks (x^128+x^7+x^2+x+1).
   b) Writing a block as a polynomial over GF(2^b), a left shift by one bit is multiplication by x.
      If the MSB was 1 the result has a term x^b, which is reduced using x^b = Rb (mod the irreducible
      polynomial), i.e. by XORing Rb into the low bits. So K1 = x*L, K2 = x*K1 = x^2*L in GF(2^b). */
#include <stdio.h>
#include <string.h>
#include "des.h"

static void dbl(const unsigned char*in,unsigned char*out,int len,unsigned char rb){
    int msb=in[0]>>7;
    for(int i=0;i<len;i++) out[i]=(unsigned char)((in[i]<<1)|((i+1<len)?(in[i+1]>>7):0));
    if(msb) out[len-1]^=rb;
}
static void hex(const char*t,const unsigned char*b,int n){ printf("%s",t); for(int i=0;i<n;i++) printf("%02X",b[i]); printf("\n"); }

int main(void){
    printf("a) Constants: block size 64 -> Rb = 0x1B ; block size 128 -> Rb = 0x87\n\n");
    /* 64-bit example with DES as the block cipher */
    u64 sub[16]; des_keys(0x133457799BBCDFF1ULL,sub);
    u64 Lv=des_crypt(0,sub,0); unsigned char L8[8],K18[8],K28[8];
    for(int i=0;i<8;i++) L8[i]=(Lv>>(56-8*i))&0xFF;
    dbl(L8,K18,8,0x1B); dbl(K18,K28,8,0x1B);
    printf("64-bit block (DES):\n"); hex("  L  = ",L8,8); hex("  K1 = ",K18,8); hex("  K2 = ",K28,8);
    /* 128-bit example: L as in RFC 4493 (AES-128 key 2b7e1516 28aed2a6 abf71588 09cf4f3c) */
    unsigned char L16[16]={0x7d,0xf7,0x6b,0x0c,0x1a,0xb8,0x99,0xb3,0x3e,0x42,0xf0,0x47,0xb9,0x1b,0x54,0x6f},K116[16],K216[16];
    dbl(L16,K116,16,0x87); dbl(K116,K216,16,0x87);
    printf("128-bit block (L from the RFC 4493 AES example):\n"); hex("  L  = ",L16,16); hex("  K1 = ",K116,16); hex("  K2 = ",K216,16);
    printf("  (expected K1 = FBEED618357133667C85E08F7236A8DE, K2 = F7DDAC306AE266CCF90BC11EE46D513B)\n");
    return 0;
}
