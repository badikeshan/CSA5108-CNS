/* Q30: CBC-MAC of a one-block message X is T = E(K,X).
   For the two-block message X || (X xor T):  C1 = E(K,X) = T,  C2 = E(K,(X xor T) xor T) = E(K,X) = T.
   So the adversary knows the MAC of the 2-block message without knowing K (existential forgery). */
#include <stdio.h>
#include "des.h"

static u64 cbc_mac(const u64*m,int n,const u64*sub){
    u64 c=0; for(int i=0;i<n;i++) c=des_crypt(m[i]^c,sub,0); return c;
}
int main(void){
    u64 sub[16]; des_keys(0x133457799BBCDFF1ULL,sub);
    u64 X=0x0123456789ABCDEFULL;
    u64 T=cbc_mac(&X,1,sub);
    u64 two[2]={X,X^T};
    u64 T2=cbc_mac(two,2,sub);
    printf("X            = %016llX\nT = MAC(K,X) = %016llX\n",(unsigned long long)X,(unsigned long long)T);
    printf("X || (X^T)   = %016llX %016llX\nMAC of 2 blocks = %016llX\n",(unsigned long long)two[0],(unsigned long long)two[1],(unsigned long long)T2);
    printf("%s\n",T==T2?"MAC(X||(X^T)) == T : forgery works without knowing the key.":"mismatch");
    return 0;
}
