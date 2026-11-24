/* Q29: SHA-3, block (rate) r = 1024 bits => 16 lanes of 64 bits; capacity c = 576 bits = 9 lanes.
   Rate lanes (A[x][y] for index x+5y = 0..15) are non-zero after absorbing P0; capacity lanes (16..24)
   are zero. In the theta step each lane is XORed with column parities, and EVERY column already
   contains non-zero lanes (rows y=0,1,2 are full), so every capacity lane becomes non-zero in the
   very first theta step: i.e. within ONE round (the first round of the first permutation). */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t u64;
static u64 A[5][5];
static u64 rotl(u64 v,int s){ return (v<<s)|(v>>(64-s)); }
static u64 rnd(void){ u64 r=0; for(int i=0;i<4;i++) r=(r<<16)^(u64)(rand()&0xFFFF); return r|1; }
static int zero_lanes(void){ int z=0; for(int x=0;x<5;x++) for(int y=0;y<5;y++) if(!A[x][y]) z++; return z; }
static void show(void){
    for(int y=0;y<5;y++){ for(int x=0;x<5;x++) printf("%c ",A[x][y]?'#':'.'); printf("\n"); }
}
int main(void){
    srand(7);
    for(int i=0;i<25;i++) A[i%5][i/5]= (i<16)? rnd():0;
    printf("After absorbing P0 ('#' = lane has a non-zero bit, '.' = all zero)\nzero lanes = %d (the 9 capacity lanes)\n",zero_lanes());
    show();
    /* theta step */
    u64 C[5],D[5];
    for(int x=0;x<5;x++) C[x]=A[x][0]^A[x][1]^A[x][2]^A[x][3]^A[x][4];
    for(int x=0;x<5;x++) D[x]=C[(x+4)%5]^rotl(C[(x+1)%5],1);
    for(int x=0;x<5;x++) for(int y=0;y<5;y++) A[x][y]^=D[x];
    printf("\nAfter theta of round 1:\nzero lanes = %d\n",zero_lanes());
    show();
    printf("\nAnswer: all capacity lanes contain a non-zero bit after the first round (theta step).\n");
    return 0;
}
