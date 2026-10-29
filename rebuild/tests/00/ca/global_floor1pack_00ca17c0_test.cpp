#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 00ca17c0 floor1_pack
// Leaf: @floor1_pack@8
// Flags: /Gr
// Original libvorbis floor1.c and backends.h; BSD-style terms in reference COPYING.
#ifndef _MSC_VER
#define __fastcall
#endif
#define VIF_POSIT 63
#define VIF_CLASS 16
#define VIF_PARTS 31
typedef void vorbis_info_floor;
typedef struct{
  int   partitions;                /* 0 to 31 */
  int   partitionclass[VIF_PARTS]; /* 0 to 15 */

  int   class_dim[VIF_CLASS];        /* 1 to 8 */
  int   class_subs[VIF_CLASS];       /* 0,1,2,3 (bits: 1<<n poss) */
  int   class_book[VIF_CLASS];       /* subs ^ dim entries */
  int   class_subbook[VIF_CLASS][8]; /* [VIF_CLASS][subs] */


  int   mult;                      /* 1 2 3 or 4 */ 
  int   postlist[VIF_POSIT+2];    /* first two implicit */ 


  /* encode side analysis parameters */
  float maxover;     
  float maxunder;  
  float maxerr;    

  float twofitweight;  
  float twofitatten;

  int   n;

} vorbis_info_floor1;
typedef struct {
  long endbyte;
  int  endbit;

  unsigned char *buffer;
  unsigned char *ptr;
  long storage;
} oggpack_buffer;
extern "C" void __fastcall oggpack_write(oggpack_buffer*,unsigned long,int);
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
extern "C" int __fastcall ilog2(unsigned int v){
  int ret=0;
  if(v)--v;
  while(v){
    ret++;
    v>>=1;
  }
  return(ret);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
extern "C" void __fastcall floor1_pack(vorbis_info_floor *i,oggpack_buffer *opb){
  vorbis_info_floor1 *info=(vorbis_info_floor1 *)i;
  int j,k;
  int count=0;
  int rangebits;
  int maxposit=info->postlist[1];
  int maxclass=-1;

  /* save out partitions */
  oggpack_write(opb,info->partitions,5); /* only 0 to 31 legal */
  for(j=0;j<info->partitions;j++){
    oggpack_write(opb,info->partitionclass[j],4); /* only 0 to 15 legal */
    if(maxclass<info->partitionclass[j])maxclass=info->partitionclass[j];
  }

  /* save out partition classes */
  for(j=0;j<maxclass+1;j++){
    oggpack_write(opb,info->class_dim[j]-1,3); /* 1 to 8 */
    oggpack_write(opb,info->class_subs[j],2); /* 0 to 3 */
    if(info->class_subs[j])oggpack_write(opb,info->class_book[j],8);
    for(k=0;k<(1<<info->class_subs[j]);k++)
      oggpack_write(opb,info->class_subbook[j][k]+1,8);
  }

  /* save out the post list */
  oggpack_write(opb,info->mult-1,2);     /* only 1,2,3,4 legal now */ 
  oggpack_write(opb,ilog2(maxposit),4);
  rangebits=ilog2(maxposit);

  for(j=0,k=0;j<info->partitions;j++){
    count+=info->class_dim[info->partitionclass[j]]; 
    for(;k<count;k++)
      oggpack_write(opb,info->postlist[k+2],rangebits);
  }
}

static unsigned cases,writeBit,readBit;
static unsigned char packet[4098];
static void check(bool p){if(!p){fprintf(stderr,"floor pack case %u failed\n",cases);abort();}}
extern "C" void __fastcall oggpack_write(oggpack_buffer*b,unsigned long value,int n){check(n>=0&&n<=32&&writeBit+n<32768);for(int k=0;k<n;++k){if((value>>k)&1U)packet[1+writeBit/8]|=(unsigned char)(1U<<(writeBit%8));++writeBit;}b->endbit=writeBit%8;b->endbyte=writeBit/8;}
static unsigned take(int n){check(readBit+n<=writeBit);unsigned value=0;for(int k=0;k<n;++k){value|=((packet[1+readBit/8]>>(readBit%8))&1U)<<k;++readBit;}return value;}
int main(){for(int exponent=6;exponent<=12;++exponent)for(int parts=0;parts<=7;++parts)for(int shape=0;shape<4;++shape)for(int mult=1;mult<=4;++mult)for(int seed=0;seed<3;++seed){vorbis_info_floor1 info;oggpack_buffer buffer;memset(&info,0,sizeof info);memset(&buffer,0,sizeof buffer);info.partitions=parts;info.mult=mult;info.postlist[1]=1<<exponent;int posts=2,maxclass=-1;for(int c=0;c<16;++c){info.class_dim[c]=shape==0?1:(shape==1?c%4+1:(shape==2?8:(c%2?7:2)));info.class_subs[c]=(c+seed)%4;info.class_book[c]=c+seed;for(int k=0;k<8;++k)info.class_subbook[c][k]=(c+k+seed)%9-1;}for(int i=0;i<parts;++i){info.partitionclass[i]=i;posts+=info.class_dim[i];maxclass=i;}for(int i=2;i<posts;++i)info.postlist[i]=1+((i-2)*37+seed*11)%(info.postlist[1]-1);vorbis_info_floor1 original=info;memset(packet,0,sizeof packet);packet[0]=0x5a;packet[4097]=0xa5;writeBit=readBit=0;floor1_pack(&info,&buffer);
check(take(5)==(unsigned)parts);for(int i=0;i<parts;++i)check(take(4)==(unsigned)info.partitionclass[i]);for(int c=0;c<=maxclass;++c){check(take(3)+1==(unsigned)info.class_dim[c]);check(take(2)==(unsigned)info.class_subs[c]);if(info.class_subs[c])check(take(8)==(unsigned)info.class_book[c]);for(int k=0;k<(1<<info.class_subs[c]);++k)check(take(8)==(unsigned)(info.class_subbook[c][k]+1));}check(take(2)+1==(unsigned)mult);unsigned range=take(4);check(range==(unsigned)exponent);for(int i=2;i<posts;++i)check(take(range)==(unsigned)info.postlist[i]);check(readBit==writeBit&&buffer.endbyte==(long)(writeBit/8)&&buffer.endbit==(int)(writeBit%8));check(memcmp(&info,&original,sizeof info)==0&&packet[0]==0x5a&&packet[4097]==0xa5);++cases;}check(cases==2688);puts("00ca17c0_TEST PASS");return 0;}