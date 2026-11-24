/* Q32: DSA signatures are randomized (fresh k per signature) so signing the same message twice gives
   different signatures; RSA signatures (no random padding) are deterministic.
   Implications: (1) with DSA an observer cannot tell whether the same message was signed again, RSA
   reveals repeats; (2) k must be unpredictable, secret and NEVER reused: if the same k signs two
   messages, the private key x can be computed (demonstrated below); (3) DSA needs a good random source. */
#include <stdio.h>

typedef long long ll;
static ll mpow(ll b,ll e,ll m){ ll r=1; b%=m; while(e){ if(e&1) r=r*b%m; b=b*b%m; e>>=1; } return r; }
static ll egcd(ll a,ll b,ll*x,ll*y){ if(!b){*x=1;*y=0;return a;} ll x1,y1,g=egcd(b,a%b,&x1,&y1); *x=y1; *y=x1-(a/b)*y1; return g; }
static ll inv(ll a,ll m){ ll x,y; egcd(((a%m)+m)%m,m,&x,&y); return ((x%m)+m)%m; }

static ll p=283,q=47,g,x=24,y;      /* toy DSA parameters: q | p-1 */
static int dsa_sign(ll H,ll k,ll*r,ll*s){
    *r=mpow(g,k,p)%q; if(!*r) return 0;
    *s=inv(k,q)*((H+x*(*r))%q)%q; return *s!=0;
}
int main(void){
    g=mpow(2,(p-1)/q,p); y=mpow(g,x,p);
    printf("DSA toy parameters: p=%lld q=%lld g=%lld, private x=%lld, public y=%lld\n\n",p,q,g,x,y);
    ll H=41,r1,s1,r2,s2;
    if(dsa_sign(H,10,&r1,&s1)&&dsa_sign(H,19,&r2,&s2)){
        printf("DSA : same message H=%lld signed twice -> (r,s)=(%lld,%lld) and (%lld,%lld)  => different\n",H,r1,s1,r2,s2);
    }
    ll n=3599,d=3031;
    printf("RSA : same message H=%lld signed twice -> %lld and %lld  => identical\n\n",H,mpow(H,d,n),mpow(H,d,n));

    /* reuse of k */
    ll Ha=41,Hb=17,ra,sa,rb,sb,k=10;
    if(dsa_sign(Ha,k,&ra,&sa)&&dsa_sign(Hb,k,&rb,&sb)){
        ll kk=((Ha-Hb)%q+q)%q*inv(sa-sb,q)%q;
        ll xx=(((sa*kk-Ha)%q+q)%q)*inv(ra,q)%q;
        printf("k reused for two messages: attacker recovers k=%lld and private key x=%lld (true x=%lld)\n",kk,xx,x);
    }
    return 0;
}
