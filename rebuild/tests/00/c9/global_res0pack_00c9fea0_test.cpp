#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 00c9fea0 res0_pack
// Leaf: @res0_pack@8
// Flags: /Gr
// Original libvorbis res0.c and backends.h; BSD-style terms in reference COPYING.
#ifndef _MSC_VER
#define __fastcall
#endif
typedef void vorbis_info_residue;
typedef struct vorbis_info_residue0{
/* block-partitioned VQ coded straight residue */
  long  begin;
  long  end;

  /* first stage (lossless partitioning) */
  int    grouping;         /* group n vectors per partition */
  int    partitions;       /* possible codebooks for a partition */
  int    groupbook;        /* huffbook for partitioning */
  int    secondstages[64]; /* expanded out to pointers in lookup */
  int    booklist[256];    /* list of second stage books */

  float  classmetric1[64];  
  float  classmetric2[64];  

} vorbis_info_residue0;
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
extern "C" int __fastcall ilog(unsigned int v){
  int ret=0;
  while(v){
    ret++;
    v>>=1;
  }
  return(ret);
}
extern "C" int __fastcall icount(unsigned int v){
  int ret=0;
  while(v){
    ret+=v&1;
    v>>=1;
  }
  return(ret);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
extern "C" void __fastcall res0_pack(vorbis_info_residue *vr,oggpack_buffer *opb){
  vorbis_info_residue0 *info=(vorbis_info_residue0 *)vr;
  int j,acc=0;
  oggpack_write(opb,info->begin,24);
  oggpack_write(opb,info->end,24);

  oggpack_write(opb,info->grouping-1,24);  /* residue vectors to group and 
					     code with a partitioned book */
  oggpack_write(opb,info->partitions-1,6); /* possible partition choices */
  oggpack_write(opb,info->groupbook,8);  /* group huffman book */

  /* secondstages is a bitmask; as encoding progresses pass by pass, a
     bitmask of one indicates this partition class has bits to write
     this pass */
  for(j=0;j<info->partitions;j++){
    if(ilog(info->secondstages[j])>3){
      /* yes, this is a minor hack due to not thinking ahead */
      oggpack_write(opb,info->secondstages[j],3); 
      oggpack_write(opb,1,1);
      oggpack_write(opb,info->secondstages[j]>>3,5); 
    }else
      oggpack_write(opb,info->secondstages[j],4); /* trailing zero */
    acc+=icount(info->secondstages[j]);
  }
  for(j=0;j<acc;j++)
    oggpack_write(opb,info->booklist[j],8);

}

static unsigned cases,writeBit,readBit;
static unsigned char packet[1026];
static void check(bool p){if(!p){fprintf(stderr,"residue pack case %u failed\n",cases);abort();}}
extern "C" void __fastcall oggpack_write(oggpack_buffer*b,unsigned long value,int n){check(n>=0&&n<=32&&writeBit+n<8192);for(int k=0;k<n;++k){if((value>>k)&1U)packet[1+writeBit/8]|=(unsigned char)(1U<<(writeBit%8));++writeBit;}b->endbit=writeBit%8;b->endbyte=writeBit/8;}
static unsigned take(int n){check(readBit+n<=writeBit);unsigned value=0;for(int k=0;k<n;++k){value|=((packet[1+readBit/8]>>(readBit%8))&1U)<<k;++readBit;}return value;}
static void run(int parts,unsigned mask,int seed){vorbis_info_residue0 info;oggpack_buffer buffer;memset(&info,0,sizeof info);memset(&buffer,0,sizeof buffer);info.begin=mask*257;info.end=0xffffff;info.grouping=seed?8:1;info.partitions=parts;info.groupbook=seed?255:0;int books=0;for(int i=0;i<parts;++i){unsigned shift=i%8;info.secondstages[i]=((mask<<shift)|(mask>>(8-shift)))&255U;for(int b=0;b<8;++b)books+=(info.secondstages[i]>>b)&1;}check(books<=256);for(int i=0;i<books;++i)info.booklist[i]=(i*37+seed*11)%256;vorbis_info_residue0 original=info;memset(packet,0,sizeof packet);packet[0]=0x5a;packet[1025]=0xa5;writeBit=readBit=0;res0_pack(&info,&buffer);check(take(24)==(unsigned)info.begin&&take(24)==(unsigned)info.end&&take(24)+1==(unsigned)info.grouping);check(take(6)+1==(unsigned)parts&&take(8)==(unsigned)info.groupbook);int decodedBooks=0;for(int i=0;i<parts;++i){unsigned stage=take(3);if(take(1))stage|=take(5)<<3;check(stage==(unsigned)info.secondstages[i]);for(int b=0;b<8;++b)decodedBooks+=(stage>>b)&1;}check(decodedBooks==books);for(int i=0;i<books;++i)check(take(8)==(unsigned)info.booklist[i]);check(readBit==writeBit&&buffer.endbyte==(long)(writeBit/8)&&buffer.endbit==(int)(writeBit%8));check(memcmp(&info,&original,sizeof info)==0&&packet[0]==0x5a&&packet[1025]==0xa5);++cases;}
int main(){for(int parts=1;parts<=16;++parts)for(unsigned mask=0;mask<256;++mask)for(int seed=0;seed<2;++seed)run(parts,mask,seed);for(int seed=0;seed<2;++seed){run(32,255,seed);run(64,15,seed);run(64,240,seed);run(64,0,seed);}check(cases==8200);puts("00c9fea0_TEST PASS");return 0;}