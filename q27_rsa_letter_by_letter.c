/* Q27: Encrypting each letter (A=0..Z=25) separately with RSA is NOT secure.
   Only 26 possible plaintexts exist, so the attacker encrypts all 26 with the public key (e,n)
   and compares with each ciphertext (a lookup / dictionary attack). No factoring needed; and the
   same letter always gives the same ciphertext, so frequency analysis also works. */
#include <stdio.h>
#include <string.h>

typedef unsigned long long u64;
static u64 mpow(u64 b,u64 e,u64 m){ u64 r=1; b%=m; while(e){ if(e&1) r=r*b%m; b=b*b%m; e>>=1; } return r; }

int main(void){
    u64 n=3599,e=31;                         /* toy key; the attack is identical for huge n */
    const char *msg="HELLOWORLD";
    int L=strlen(msg); u64 C[64];
    printf("Ciphertext:"); for(int i=0;i<L;i++){ C[i]=mpow(msg[i]-'A',e,n); printf(" %llu",C[i]); }
    printf("\n\nAttacker's table (public key only):\n");
    u64 table[26]; for(int m=0;m<26;m++){ table[m]=mpow(m,e,n); printf("%c->%llu ",'A'+m,table[m]); }
    printf("\n");
    printf("Recovered message: ");
    for(int i=0;i<L;i++){ char ch='?'; for(int m=0;m<26;m++) if(table[m]==C[i]) ch='A'+m; putchar(ch); }
    printf("\n\nMost efficient attack: encrypt all 26 letters with (e,n), build the table, and read the message.\n");
    return 0;
}
