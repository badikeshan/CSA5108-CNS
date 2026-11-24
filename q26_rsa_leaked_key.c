/* Q26: Bob leaks d and picks a new (e',d') with the SAME modulus n. Not safe!
   Anyone who knows (n,e,d) can factor n (probabilistic algorithm below). Once n is factored, every
   key pair for that n is compromised, so Bob must choose a new modulus. */
#include <stdio.h>

typedef unsigned long long u64;
static u64 gcd(u64 a,u64 b){ while(b){u64 t=a%b;a=b;b=t;} return a; }
static u64 mpow(u64 b,u64 e,u64 m){ u64 r=1; b%=m; while(e){ if(e&1) r=r*b%m; b=b*b%m; e>>=1; } return r; }

int main(void){
    u64 n=3599,e=31,d=3031;                 /* leaked pair */
    u64 k=e*d-1, r=k; int t=0;
    while(r%2==0){ r/=2; t++; }             /* k = 2^t * r, r odd */
    printf("e*d-1 = %llu = 2^%d * %llu\n",k,t,r);
    for(u64 a=2;a<50;a++){
        u64 g=gcd(a,n); if(g>1&&g<n){ printf("lucky: gcd(%llu,n)=%llu\n",a,g); return 0; }
        u64 x=mpow(a,r,n);
        for(int i=0;i<t;i++){
            u64 y=x*x%n;
            if(y==1 && x!=1 && x!=n-1){
                u64 p=gcd(x-1,n), q=n/p;
                printf("base a=%llu : non-trivial square root of 1 found (x=%llu)\n",a,x);
                printf("n factored: p=%llu, q=%llu\n",p,q);
                u64 phi=(p-1)*(q-1);
                printf("phi(n)=%llu. For ANY new e' with gcd(e',phi)=1 the attacker computes d' himself.\n",phi);
                printf("Conclusion: generating new (e,d) with the same n is NOT safe; Bob needs a new modulus.\n");
                return 0;
            }
            x=y;
        }
    }
    printf("No factor found with the chosen bases; try more bases.\n");
    return 0;
}
