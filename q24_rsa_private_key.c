/* Q24: RSA public key e=31, n=3599. Find private key d. */
#include <stdio.h>

static long long egcd(long long a,long long b,long long*x,long long*y){
    if(!b){*x=1;*y=0;return a;}
    long long x1,y1,g=egcd(b,a%b,&x1,&y1); *x=y1; *y=x1-(a/b)*y1; return g;
}
int main(void){
    long long e=31,n=3599,p=0,q=0;
    for(long long i=2;i*i<=n;i++) if(n%i==0){p=i;q=n/i;break;}      /* trial and error */
    long long phi=(p-1)*(q-1),x,y;
    printf("n = %lld = %lld x %lld\nphi(n) = %lld\n",n,p,q,phi);
    egcd(e,phi,&x,&y);
    long long d=((x%phi)+phi)%phi;                                      /* inverse of e mod phi */
    printf("Extended Euclid: %lld*%lld + %lld*%lld = 1\n",e,x,phi,y);
    printf("Private key d = %lld  (check: e*d mod phi = %lld)\n",d,(e*d)%phi);
    return 0;
}
