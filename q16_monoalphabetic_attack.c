/* Q16: Letter-frequency attack on any monoalphabetic substitution cipher.
   Starts from a frequency-rank guess, improves it by hill climbing (swapping key letters)
   using unigram + bigram + trigram English statistics, and prints the top N plaintexts. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAXN 8192
#define MAXR 400
static const double F[26]={8.17,1.49,2.78,4.25,12.70,2.23,2.02,6.09,6.97,0.15,0.77,4.03,2.41,
                           6.75,7.51,1.93,0.10,5.99,6.33,9.06,2.76,0.98,2.36,0.15,1.97,0.07};
static const char *ORDER="ETAOINSHRDLCUMWFGYPBVKJXQZ";
static const char *BIG="TH5 HE5 IN4 ER4 AN4 RE3 ON3 AT3 EN3 ND3 TI3 ES3 OR3 TE3 OF3 ED3 IS3 IT3 AL3 AR3 ST3 TO3 NT3 NG3 SE2 HA2 AS2 OU2 IO2 LE2 VE2 CO2 ME2 DE2 HI2 RI2 RO2 IC2 NE2 EA2 RA2 CE2";
static const char *TRI="THE9 AND6 ING6 ENT4 ION4 HER4 FOR4 THA4 NTH3 INT3 ERE3 TIO3 TER3 EST3 ERS3 ATI3 HAT3 ATE3 ALL3 ETH3 HES3 VER3 HIS3 OFT2 ITH2 FTH2 STH2 OTH2 RES2 ONT2";

static double bi[26][26], tri[26][26][26], uni[26];
static int ct[MAXN], n;

static void init_tables(void){
    for(int i=0;i<26;i++) uni[i]=0.3*log(F[i]/100.0);
    char buf[512]; strcpy(buf,BIG);
    for(char*t=strtok(buf," ");t;t=strtok(NULL," ")) bi[t[0]-'A'][t[1]-'A']=t[2]-'0';
    strcpy(buf,TRI);
    for(char*t=strtok(buf," ");t;t=strtok(NULL," ")) tri[t[0]-'A'][t[1]-'A'][t[2]-'A']=t[3]-'0';
}
/* key[c] = plaintext letter assigned to cipher letter c */
static double score(const int *key){
    double s=0; int p0=-1,p1=-1;
    for(int i=0;i<n;i++){
        int p=key[ct[i]];
        s+=uni[p];
        if(p1>=0) s+=bi[p1][p];
        if(p0>=0) s+=tri[p0][p1][p];
        p0=p1; p1=p;
    }
    return s;
}
typedef struct { int key[26]; double sc; } Res;
static Res res[MAXR]; static int nres=0;
static int cmpr(const void*a,const void*b){ double d=((const Res*)b)->sc-((const Res*)a)->sc; return (d>0)-(d<0); }

static void climb(int *key){
    double best=score(key); int improved=1;
    while(improved){
        improved=0;
        for(int i=0;i<25;i++) for(int j=i+1;j<26;j++){
            int t=key[i]; key[i]=key[j]; key[j]=t;
            double s=score(key);
            if(s>best+1e-9){ best=s; improved=1; }
            else { t=key[i]; key[i]=key[j]; key[j]=t; }
        }
    }
}
int main(void){
    static char line[MAXN]; int top;
    printf("Enter ciphertext (letters only are used): ");
    if(!fgets(line,sizeof line,stdin)) return 1;
    for(int i=0;line[i];i++) if(isalpha((unsigned char)line[i])) ct[n++]=toupper((unsigned char)line[i])-'A';
    if(n<3){ printf("Ciphertext too short.\n"); return 1; }
    printf("How many top plaintexts do you want? ");
    if(scanf("%d",&top)!=1||top<1) top=10;
    init_tables();

    /* initial key: i-th most frequent cipher letter -> i-th most frequent English letter */
    int cnt[26]={0}, rank[26], base[26];
    for(int i=0;i<n;i++) cnt[ct[i]]++;
    for(int i=0;i<26;i++) rank[i]=i;
    for(int i=0;i<25;i++) for(int j=i+1;j<26;j++) if(cnt[rank[j]]>cnt[rank[i]]){int t=rank[i];rank[i]=rank[j];rank[j]=t;}
    for(int i=0;i<26;i++) base[rank[i]]=ORDER[i]-'A';

    srand(12345);
    for(int r=0;r<MAXR;r++){
        int key[26]; memcpy(key,base,sizeof key);
        int shakes=(r==0)?0:2+r%12;           /* random perturbations => different local optima */
        for(int s=0;s<shakes;s++){ int i=rand()%26,j=rand()%26,t=key[i]; key[i]=key[j]; key[j]=t; }
        climb(key);
        int dup=0;
        for(int k=0;k<nres;k++) if(!memcmp(res[k].key,key,sizeof key)){dup=1;break;}
        if(!dup){ memcpy(res[nres].key,key,sizeof key); res[nres].sc=score(key); nres++; }
    }
    qsort(res,nres,sizeof(Res),cmpr);
    if(top>nres) top=nres;
    for(int r=0;r<top;r++){
        printf("%2d) score=%9.2f  ",r+1,res[r].sc);
        for(int i=0;i<n;i++) putchar('a'+res[r].key[ct[i]]);
        putchar('\n');
    }
    return 0;
}
