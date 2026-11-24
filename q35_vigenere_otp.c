/* Q35: One-time-pad version of the Vigenere cipher: key = stream of random numbers 0..25 (as long as
   the message, used once). Ciphertext letter = (plain + key) mod 26. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

static int rand26(FILE*f){
    int b;
    do { b = f ? fgetc(f) : rand()%256; } while(b>=234);     /* reject to avoid modulo bias (234 = 26*9) */
    return b%26;
}
int main(void){
    char line[1024], pt[1024]; int n=0;
    printf("Enter plaintext: "); if(!fgets(line,sizeof line,stdin)) return 1;
    for(int i=0;line[i];i++) if(isalpha((unsigned char)line[i])) pt[n++]=toupper((unsigned char)line[i]);
    FILE*f=fopen("/dev/urandom","rb"); if(!f) srand((unsigned)time(NULL));
    int key[1024]; char ct[1024], back[1024];
    for(int i=0;i<n;i++){ key[i]=rand26(f); ct[i]='A'+(pt[i]-'A'+key[i])%26; }
    for(int i=0;i<n;i++) back[i]='A'+(ct[i]-'A'-key[i]+26)%26;
    ct[n]=back[n]=0; if(f) fclose(f);
    printf("Key stream : "); for(int i=0;i<n;i++) printf("%d ",key[i]);
    printf("\nCiphertext : %s\nDecrypted  : %s\n",ct,back);
    return 0;
}
