/* Q36: Affine Caesar cipher C = (a*p + b) mod 26.
   (a) b has NO limitation: any b in 0..25 works (it is just a shift added after the multiplication).
   (b) a must satisfy gcd(a,26)=1, otherwise E is not one-to-one. Allowed a: 1,3,5,7,9,11,15,17,19,21,23,25.
       Not allowed: even a (2,4,...,24) and a=13. Example a=2,b=3: E(0)=E(13)=3. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int gcd(int a,int b){ while(b){int t=a%b;a=b;b=t;} return a; }
static int inv(int a){ for(int i=1;i<26;i++) if(a*i%26==1) return i; return -1; }
static int one_to_one(int a,int b){ int seen[26]={0}; for(int p=0;p<26;p++){ int c=(a*p+b)%26; if(seen[c]) return 0; seen[c]=1; } return 1; }

int main(void){
    printf("Brute-force check of which a are allowed (any b):\n  allowed a:");
    for(int a=1;a<26;a++){ int ok=1; for(int b=0;b<26;b++) if(!one_to_one(a,b)) ok=0; if(ok) printf(" %d",a); }
    printf("\n  not allowed:"); for(int a=1;a<26;a++) if(gcd(a,26)!=1) printf(" %d",a);
    printf("\n  b: every value 0..25 is allowed (no limitation)\n");
    printf("  a=2,b=3 -> E(0)=%d, E(13)=%d (collision)\n\n",(2*0+3)%26,(2*13+3)%26);

    int a,b,mode; char line[512];
    printf("Enter a: "); if(scanf("%d",&a)!=1) return 1;
    printf("Enter b: "); if(scanf("%d",&b)!=1) return 1;
    if(gcd(a,26)!=1){ printf("a=%d is not allowed (gcd(a,26)!=1).\n",a); return 1; }
    printf("1 = encrypt, 2 = decrypt: "); if(scanf("%d",&mode)!=1) return 1;
    getchar(); printf("Enter text: "); if(!fgets(line,sizeof line,stdin)) return 1;
    int ai=inv(a);
    printf("Result: ");
    for(int i=0;line[i];i++){
        if(isalpha((unsigned char)line[i])){
            int p=toupper((unsigned char)line[i])-'A';
            int c= mode==1 ? (a*p+b)%26 : (ai*((p-b+26)%26))%26;
            putchar(mode==1?'A'+c:'a'+c);
        } else if(line[i]!='\n') putchar(line[i]);
    }
    putchar('\n');
    return 0;
}
