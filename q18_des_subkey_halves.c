/* Q18: In DES, the first 24 bits of every subkey come from the same 28-bit subset (C register)
   of the key, and the last 24 bits come from a disjoint 28-bit subset (D register).
   Reason: PC-1 splits the key into C0 (28 bits) and D0 (28 bits), shifts are done separately on
   C and D, and PC-2 takes positions 1-28 (C) for its first 24 outputs and 29-56 (D) for its last 24. */
#include <stdio.h>
#include "des.h"

int main(void){
    int ok1=1, ok2=1;
    for(int i=0;i<24;i++){ if(DES_PC2[i]>28) ok1=0; }
    for(int i=24;i<48;i++){ if(DES_PC2[i]<29) ok2=0; }
    printf("First 24 PC-2 outputs all taken from C (positions 1..28): %s\n",ok1?"yes":"no");
    printf("Last 24 PC-2 outputs all taken from D (positions 29..56): %s\n",ok2?"yes":"no");

    int usedC[29]={0}, usedD[57]={0};
    for(int i=0;i<24;i++) usedC[DES_PC2[i]]=1;
    for(int i=24;i<48;i++) usedD[DES_PC2[i]]=1;
    printf("C positions dropped by PC-2:"); for(int i=1;i<=28;i++) if(!usedC[i]) printf(" %d",i);
    printf("\nD positions dropped by PC-2:"); for(int i=29;i<=56;i++) if(!usedD[i]) printf(" %d",i);

    int inC[65]={0}, disjoint=1;
    printf("\n\nKey bits (of 64) feeding C0:"); 
    for(int i=0;i<28;i++){ inC[DES_PC1[i]]=1; printf(" %d",DES_PC1[i]); }
    printf("\nKey bits (of 64) feeding D0:");
    for(int i=28;i<56;i++){ if(inC[DES_PC1[i]]) disjoint=0; printf(" %d",DES_PC1[i]); }
    printf("\n\nThe two 28-bit key subsets are %s.\n",disjoint?"disjoint":"NOT disjoint");

    /* experiment: change one key bit that feeds C only and see which half of the subkeys change */
    u64 k1=0x133457799BBCDFF1ULL, s1[16], s2[16];
    u64 k2=k1^(1ULL<<(64-DES_PC1[0]));      /* flip a bit that goes into C0 */
    des_keys(k1,s1); des_keys(k2,s2);
    int upper=0,lower=0;
    for(int r=0;r<16;r++){ u64 x=s1[r]^s2[r]; if(x>>24) upper=1; if(x&0xFFFFFF) lower=1; }
    printf("Flip one C-bit: first-24-bit half changed=%s, last-24-bit half changed=%s\n",upper?"yes":"no",lower?"yes":"no");
    return 0;
}
