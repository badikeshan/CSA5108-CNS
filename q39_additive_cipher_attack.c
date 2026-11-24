/* Q39: Letter-frequency attack on an additive (shift) cipher - top N plaintexts */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const double F[26]={8.17,1.49,2.78,4.25,12.70,2.23,2.02,6.09,6.97,0.15,0.77,4.03,2.41,
                           6.75,7.51,1.93,0.10,5.99,6.33,9.06,2.76,0.98,2.36,0.15,1.97,0.07};
typedef struct { int key; double chi; } Cand;

static int cmp(const void *a, const void *b){
    double d=((const Cand*)a)->chi-((const Cand*)b)->chi;
    return (d>0)-(d<0);
}

int main(void){
    char ct[4096], letters[4096]; int n=0, top;
    printf("Enter ciphertext: ");
    if(!fgets(ct,sizeof ct,stdin)) return 1;
    for(int i=0;ct[i];i++) if(isalpha((unsigned char)ct[i])) letters[n++]=toupper((unsigned char)ct[i]);
    letters[n]=0;
    if(n==0){ printf("No letters found.\n"); return 1; }
    printf("How many top plaintexts do you want? "); 
    if(scanf("%d",&top)!=1||top<1) top=10;
    if(top>26) top=26;

    Cand c[26];
    for(int k=0;k<26;k++){
        int cnt[26]={0};
        for(int i=0;i<n;i++) cnt[(letters[i]-'A'-k+26)%26]++;
        double chi=0;
        for(int j=0;j<26;j++){ double e=n*F[j]/100.0; chi+=(cnt[j]-e)*(cnt[j]-e)/e; }
        c[k].key=k; c[k].chi=chi;
    }
    qsort(c,26,sizeof(Cand),cmp);      /* smallest chi-square = most English-like */
    for(int r=0;r<top;r++){
        printf("%2d) key=%2d  chi2=%8.2f  ",r+1,c[r].key,c[r].chi);
        for(int i=0;i<n;i++) putchar('a'+(letters[i]-'A'-c[r].key+26)%26);
        putchar('\n');
    }
    return 0;
}
