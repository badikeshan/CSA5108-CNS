/* Q19: CBC encryption with 3DES (EDE).  Two ways to combine CBC with triple encryption:
   OUTER-CBC : one CBC chain whose block cipher is the whole 3DES  (C_i = E3(D2(E1(P_i ^ C_{i-1}))))
   INNER-CBC : three separate CBC passes (enc with K1, dec with K2, enc with K3), 3 IVs
   a) Security    -> Outer-CBC (the three stages are not independently chained, so the known
                     attacks on inner-CBC do not apply; inner-CBC leaks structure between passes).
   b) Performance -> Inner-CBC (the three passes can be pipelined / run in parallel hardware,
                     whereas outer-CBC must finish one whole 3DES block before the next starts). */
#include <stdio.h>
#include <string.h>
#include "des.h"

static u64 K1[16],K2[16],K3[16];
static u64 tdes_e(u64 x){ return des_crypt(des_crypt(des_crypt(x,K1,0),K2,1),K3,0); }
static u64 tdes_d(u64 x){ return des_crypt(des_crypt(des_crypt(x,K3,1),K2,0),K1,1); }

static void cbc_enc(u64*b,int n,const u64*sub,u64 iv){ u64 p=iv; for(int i=0;i<n;i++){ b[i]=des_crypt(b[i]^p,sub,0); p=b[i]; } }
static void cbc_dec(u64*b,int n,const u64*sub,u64 iv){ u64 p=iv; for(int i=0;i<n;i++){ u64 c=b[i]; b[i]=des_crypt(c,sub,1)^p; p=c; } }

static void outer_enc(u64*b,int n,u64 iv){ u64 p=iv; for(int i=0;i<n;i++){ b[i]=tdes_e(b[i]^p); p=b[i]; } }
static void outer_dec(u64*b,int n,u64 iv){ u64 p=iv; for(int i=0;i<n;i++){ u64 c=b[i]; b[i]=tdes_d(c)^p; p=c; } }

static void show(const char*t,const u64*b,int n){ printf("%s",t); for(int i=0;i<n;i++) printf(" %016llX",(unsigned long long)b[i]); printf("\n"); }

int main(void){
    des_keys(0x0123456789ABCDEFULL,K1); des_keys(0x23456789ABCDEF01ULL,K2); des_keys(0x456789ABCDEF0123ULL,K3);
    u64 iv1=0x1111111111111111ULL, iv2=0x2222222222222222ULL, iv3=0x3333333333333333ULL;
    u64 pt[4]={0x4E6F77206973207AULL,0x68652074696D6520ULL,0x666F7220616C6C20ULL,0x6D656E2E00000000ULL};
    u64 b[4];

    memcpy(b,pt,sizeof b); outer_enc(b,4,iv1); show("Outer-CBC cipher:",b,4);
    outer_dec(b,4,iv1);   show("Outer-CBC decrypt:",b,4);
    printf("Outer-CBC round trip %s\n\n",memcmp(b,pt,sizeof b)?"FAILED":"OK");

    memcpy(b,pt,sizeof b); cbc_enc(b,4,K1,iv1); cbc_dec(b,4,K2,iv2); cbc_enc(b,4,K3,iv3);
    show("Inner-CBC cipher:",b,4);
    cbc_dec(b,4,K3,iv3); cbc_enc(b,4,K2,iv2); cbc_dec(b,4,K1,iv1);
    show("Inner-CBC decrypt:",b,4);
    printf("Inner-CBC round trip %s\n\n",memcmp(b,pt,sizeof b)?"FAILED":"OK");

    printf("a) For security    : choose OUTER-CBC.\nb) For performance : choose INNER-CBC (three stages can be pipelined).\n");
    return 0;
}
