/* Q25: If one plaintext block M shares a factor with n, RSA is broken.
   gcd(M,n)=p>1  =>  gcd(C,n)=p too, since C = M^e mod n.  Compute gcd of every ciphertext block with n:
   any value >1 factors n, giving p,q, phi(n) and the private key, so ALL blocks can be decrypted. */
#include <stdio.h>

typedef unsigned long long u64;
static u64 gcd(u64 a,u64 b){ while(b){u64 t=a%b;a=b;b=t;} return a; }
static u64 mpow(u64 b,u64 e,u64 m){ u64 r=1; b%=m; while(e){ if(e&1) r=r*b%m; b=b*b%m; e>>=1; } return r; }
static long long egcd(long long a,long long b,long long*x,long long*y){
    if(!b){*x=1;*y=0;return a;} long long x1,y1,g=egcd(b,a%b,&x1,&y1); *x=y1; *y=x1-(a/b)*y1; return g; }

int main(void){
    u64 n=3599,e=31;                       /* attacker knows only n and e */
    u64 M[5]={1000,2000,118,3000,777};     /* 118 = 2*59 shares factor 59 with n (unknown to attacker) */
    u64 C[5]; for(int i=0;i<5;i++) C[i]=mpow(M[i],e,n);
    printf("Intercepted ciphertext blocks:"); for(int i=0;i<5;i++) printf(" %llu",C[i]); printf("\n");

    u64 p=0;
    for(int i=0;i<5;i++){ u64 g=gcd(C[i],n); if(g>1&&g<n){ p=g; printf("gcd(C%d,n) = %llu  -> factor of n found!\n",i+1,g); break; } }
    if(!p){ printf("No factor found.\n"); return 0; }
    u64 q=n/p, phi=(p-1)*(q-1); long long x,y; egcd(e,phi,&x,&y);
    u64 d=(u64)(((x%(long long)phi)+phi)%phi);
    printf("p=%llu q=%llu phi=%llu d=%llu\nRecovered plaintext blocks:",p,q,phi,d);
    for(int i=0;i<5;i++) printf(" %llu",mpow(C[i],d,n));
    printf("\n");
    printf("Answer: YES, it helps completely - the whole system for this n is broken.\n");
    return 0;
}
