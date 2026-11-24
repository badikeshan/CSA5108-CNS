/* Q17: DES decryption uses K16,K15,...,K1. Key-generation scheme for decryption:
   K16 = PC2(C0,D0) (no shift), then rotate C and D RIGHT by 0,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1. */
#include <stdio.h>
#include "des.h"

int main(void){
    u64 key=0x133457799BBCDFF1ULL, enc[16], dec[16];
    const int rs[16]={0,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};   /* right-shift schedule for decryption */
    des_keys(key,enc);

    u64 cd=des_permute(key,DES_PC1,64,56);
    u32 c=(cd>>28)&0xFFFFFFF, d=cd&0xFFFFFFF;
    for(int i=0;i<16;i++){
        int s=rs[i];
        if(s){ c=((c>>s)|(c<<(28-s)))&0xFFFFFFF; d=((d>>s)|(d<<(28-s)))&0xFFFFFFF; }
        dec[i]=des_permute(((u64)c<<28)|d,DES_PC2,56,48);
    }
    int ok=1;
    printf("Round | right shift | decryption key (round i)      | equals K(17-i)?\n");
    for(int i=0;i<16;i++){
        int same=(dec[i]==enc[15-i]); ok&=same;
        printf("%5d | %11d | %012llX | %s (K%d)\n",i+1,rs[i],(unsigned long long)dec[i],same?"yes":"NO",16-i);
    }
    printf("\nSchedule is %s.\n",ok?"correct":"WRONG");

    u64 pt=0x0123456789ABCDEFULL;
    u64 ct=des_crypt(pt,enc,0);
    u64 back=des_crypt(ct,dec,0);   /* plain encryption routine fed the reversed key order = decryption */
    printf("Plaintext  : %016llX\nCiphertext : %016llX\nDecrypted  : %016llX\n",
           (unsigned long long)pt,(unsigned long long)ct,(unsigned long long)back);
    return 0;
}
