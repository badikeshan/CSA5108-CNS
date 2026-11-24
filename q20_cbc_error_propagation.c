/* Q20: Error propagation in CBC (DES used as the block cipher). */
#include <stdio.h>
#include <string.h>
#include "des.h"

static u64 sub[16];
static void cbc_enc(u64*b,int n,u64 iv){ u64 p=iv; for(int i=0;i<n;i++){ b[i]=des_crypt(b[i]^p,sub,0); p=b[i]; } }
static void cbc_dec(u64*b,int n,u64 iv){ u64 p=iv; for(int i=0;i<n;i++){ u64 c=b[i]; b[i]=des_crypt(c,sub,1)^p; p=c; } }
static int bits(u64 x){ int c=0; while(x){c+=x&1;x>>=1;} return c; }

int main(void){
    des_keys(0x133457799BBCDFF1ULL,sub);
    u64 iv=0xA5A5A5A5A5A5A5A5ULL;
    u64 P[5]={0x1111111111111111ULL,0x2222222222222222ULL,0x3333333333333333ULL,0x4444444444444444ULL,0x5555555555555555ULL};
    u64 C[5]; memcpy(C,P,sizeof C); cbc_enc(C,5,iv);

    printf("(a) One-bit error in transmitted C1:\n");
    u64 Cx[5]; memcpy(Cx,C,sizeof C); Cx[0]^=1;
    cbc_dec(Cx,5,iv);
    for(int i=0;i<5;i++) printf("   P%d : %2d bit(s) wrong\n",i+1,bits(Cx[i]^P[i]));
    printf("   => P1 is garbled, P2 gets exactly the same 1-bit error as C1, P3.. are NOT affected.\n\n");

    printf("(b) One-bit error in the source P1 (before encryption):\n");
    u64 Pe[5]; memcpy(Pe,P,sizeof P); Pe[0]^=1;
    u64 Ce[5]; memcpy(Ce,Pe,sizeof Ce); cbc_enc(Ce,5,iv);
    for(int i=0;i<5;i++) printf("   C%d differs from error-free C%d in %2d bit(s)\n",i+1,i+1,bits(Ce[i]^C[i]));
    cbc_dec(Ce,5,iv);
    for(int i=0;i<5;i++) printf("   receiver P%d : %2d bit(s) differ from the TRUE message\n",i+1,bits(Ce[i]^P[i]));
    printf("   => the error propagates through ALL following ciphertext blocks (C1..Cn), but the receiver\n"
           "      decrypts everything correctly: only P1 contains the original 1-bit error, other blocks are fine.\n");
    return 0;
}
