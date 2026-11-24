/* Q33: DES (64-bit block, 56-bit key). Same algorithm for encryption and decryption
   (decryption uses the 16 round keys in reverse order). Input/output in hexadecimal. */
#include <stdio.h>
#include "des.h"

int main(void){
    unsigned long long key,data; int mode;
    printf("Enter 64-bit key (16 hex digits): "); if(scanf("%llx",&key)!=1) return 1;
    printf("Enter 64-bit data (16 hex digits): "); if(scanf("%llx",&data)!=1) return 1;
    printf("1 = encrypt, 2 = decrypt : "); if(scanf("%d",&mode)!=1) return 1;
    u64 sub[16]; des_keys(key,sub);
    u64 out=des_crypt(data,sub,mode==2);
    printf("Result: %016llX\n",(unsigned long long)out);
    printf("(Self-test: key 133457799BBCDFF1, plaintext 0123456789ABCDEF -> ");
    u64 s[16]; des_keys(0x133457799BBCDFF1ULL,s);
    printf("%016llX, expected 85E813540F0AB405)\n",(unsigned long long)des_crypt(0x0123456789ABCDEFULL,s,0));
    return 0;
}
