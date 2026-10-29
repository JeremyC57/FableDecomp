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