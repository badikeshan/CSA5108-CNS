/* Q28: Diffie-Hellman variant where each side sends x*a mod q instead of a^x mod q.
   Key agreement: Alice computes x*(y*a) = x*y*a, Bob computes y*(x*a) = x*y*a  (mod q).
   Eve CAN break it: a is public and invertible mod q, so x = (x*a)*a^-1 mod q (multiplication is
   easy to undo, unlike the discrete logarithm). She can also form the key directly as
   (x*a)(y*a)*a^-1 = x*y*a without knowing x or y, and she can also recover x and y. */
#include <stdio.h>

typedef long long ll;
static ll egcd(ll a,ll b,ll*x,ll*y){ if(!b){*x=1;*y=0;return a;} ll x1,y1,g=egcd(b,a%b,&x1,&y1); *x=y1; *y=x1-(a/b)*y1; return g; }
static ll inv(ll a,ll m){ ll x,y; egcd(a,m,&x,&y); return ((x%m)+m)%m; }

int main(void){
    ll q=1019,a=5, x=123, y=456;            /* q prime, a public, x,y secret */
    ll A=x*a%q, B=y*a%q;                    /* values sent over the channel */
    printf("Public: q=%lld a=%lld\nAlice sends %lld, Bob sends %lld\n",q,a,A,B);
    ll kA=x*B%q, kB=y*A%q;
    printf("Alice key = %lld, Bob key = %lld\n",kA,kB);
    ll ia=inv(a,q);
    ll kE=A*B%q*ia%q;
    printf("Eve (no secrets used): key = (xa)(ya)a^-1 mod q = %lld\n",kE);
    printf("Eve finds secret numbers: x = %lld, y = %lld\n",A*ia%q,B*ia%q);
    printf("So the scheme is insecure: Eve can find the key AND the secret numbers.\n");
    return 0;
}
