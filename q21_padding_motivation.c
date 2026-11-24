/* Q21: Why always add a padding block, even if the message is already a multiple of the block size?
   Padding = a 1 bit followed by 0 bits (0x80 00 .. 00).  If padding is added only "when needed",
   the receiver cannot tell whether a trailing 0x80 00.. belongs to the padding or to the data.
   Always padding makes removal unambiguous: the LAST 0x80 (after stripping zeros) is the pad marker. */
#include <stdio.h>
#include <string.h>

#define B 8
static int pad(const unsigned char*in,int n,unsigned char*out,int always){
    memcpy(out,in,n);
    if(n%B==0 && !always) return n;        /* policy "pad only if needed" */
    int m=n; out[m++]=0x80; while(m%B) out[m++]=0; return m;
}
static int unpad(const unsigned char*b,int m){
    int i=m-1; while(i>=0 && b[i]==0) i--;
    if(i>=0 && b[i]==0x80) return i;       /* strip pad */
    return m;                              /* no pad found: keep everything */
}
static void test(const unsigned char*msg,int n,int always){
    unsigned char buf[64]; int m=pad(msg,n,buf,always), r=unpad(buf,m);
    printf("  policy %-12s: padded length %2d, recovered length %2d -> %s\n",
           always?"always-pad":"pad-if-needed",m,r,r==n?"correct":"WRONG (data lost)");
}
int main(void){
    unsigned char m1[8]={'A','B','C','D','E','F','G',0x80};   /* full block that happens to end in 0x80 */
    unsigned char m2[8]={'A','B','C','D','E','F','G','H'};
    unsigned char m3[5]={'A','B','C',0x80,0x00};              /* short block that ends like a pad */
    printf("Message 1 (8 bytes, last byte 0x80):\n"); test(m1,8,0); test(m1,8,1);
    printf("Message 2 (8 plain bytes):\n");           test(m2,8,0); test(m2,8,1);
    printf("Message 3 (5 bytes ending 0x80 0x00):\n");test(m3,5,0); test(m3,5,1);
    printf("\nMotivation: the receiver must be able to remove the padding unambiguously. Without a padding block\n"
           "when the message is already complete, a message that ends with 1000...0 looks like padded data and\n"
           "the receiver would wrongly strip it. A full padding block is always present, so the last 1-bit\n"
           "followed by zeros is always the padding marker.\n");
    return 0;
}
