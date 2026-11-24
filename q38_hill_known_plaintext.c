/* Q38: Known-plaintext and chosen-plaintext attacks on a 2x2 Hill cipher (mod 26).
   Encryption of a digraph (row vector): c = p*K mod 26.
   Known plaintext: with two digraph pairs, P (2x2 of plaintext rows) and C (2x2 of ciphertext rows) give
   C = P*K  =>  K = P^-1 * C mod 26 (P must be invertible mod 26; try other pairs if not).
   Chosen plaintext: choose P = identity ("BA","AB"); the ciphertexts ARE the rows of K. */
#include <stdio.h>
#include <string.h>

static int M(int x){ return ((x%26)+26)%26; }
static int inv26(int a){ a=M(a); for(int i=1;i<26;i++) if(a*i%26==1) return i; return -1; }
static void enc(const int K[2][2],const int*p,int*c){
    c[0]=M(p[0]*K[0][0]+p[1]*K[1][0]); c[1]=M(p[0]*K[0][1]+p[1]*K[1][1]);
}
static int invert(const int P[2][2],int R[2][2]){
    int det=M(P[0][0]*P[1][1]-P[0][1]*P[1][0]), di=inv26(det);
    if(di<0) return 0;
    R[0][0]=M(di*P[1][1]); R[0][1]=M(-di*P[0][1]); R[1][0]=M(-di*P[1][0]); R[1][1]=M(di*P[0][0]);
    return 1;
}
int main(void){
    int K[2][2]={{9,4},{5,7}};                          /* secret key, unknown to the attacker */
    const char *plain="meetmeattheusualplaceattenratherthaneightoclock";
    int n=strlen(plain)/2*2, p[64][2], c[64][2];
    for(int i=0;i<n/2;i++){ p[i][0]=plain[2*i]-'a'; p[i][1]=plain[2*i+1]-'a'; enc(K,p[i],c[i]); }
    printf("Attacker sees %d plaintext/ciphertext digraph pairs.\n",n/2);

    int found=0, Kg[2][2];
    for(int i=0;i<n/2&&!found;i++) for(int j=i+1;j<n/2&&!found;j++){
        int P[2][2]={{p[i][0],p[i][1]},{p[j][0],p[j][1]}}, C[2][2]={{c[i][0],c[i][1]},{c[j][0],c[j][1]}}, Pi[2][2];
        if(!invert(P,Pi)) continue;
        for(int r=0;r<2;r++) for(int s=0;s<2;s++) Kg[r][s]=M(Pi[r][0]*C[0][s]+Pi[r][1]*C[1][s]);
        int ok=1; for(int t=0;t<n/2;t++){ int cc[2]; enc(Kg,p[t],cc); if(cc[0]!=c[t][0]||cc[1]!=c[t][1]) ok=0; }
        if(ok){ found=1; printf("Known plaintext: used pairs %d and %d -> recovered key [[%d %d][%d %d]]\n",i+1,j+1,Kg[0][0],Kg[0][1],Kg[1][0],Kg[1][1]); }
    }
    if(!found) printf("Known plaintext attack failed (need more pairs).\n");

    int e1[2]={1,0},e2[2]={0,1},r1[2],r2[2];            /* chosen plaintext "BA" and "AB" */
    enc(K,e1,r1); enc(K,e2,r2);
    printf("Chosen plaintext: E(\"BA\")=\"%c%c\", E(\"AB\")=\"%c%c\" -> key rows [%d %d] and [%d %d]\n",
           'A'+r1[0],'A'+r1[1],'A'+r2[0],'A'+r2[1],r1[0],r1[1],r2[0],r2[1]);
    printf("True key        : [[%d %d][%d %d]]\n",K[0][0],K[0][1],K[1][0],K[1][1]);
    return 0;
}
