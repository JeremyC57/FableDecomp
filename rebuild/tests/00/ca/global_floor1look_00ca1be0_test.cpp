#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#include <stdlib.h>
extern "C" void* test_calloc(size_t,size_t);
#define calloc test_calloc
#endif
// 00ca1be0 floor1_look
// Leaf: @floor1_look@8
// Flags: /Gr
// Original libvorbis floor1.c and backends.h; BSD-style terms in reference COPYING.
#include <stddef.h>
#ifdef _MSC_VER
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int,unsigned int);
extern "C" __declspec(dllimport) void __cdecl qsort(void*,unsigned int,unsigned int,int(__cdecl*)(const void*,const void*));
#else
#include <stdlib.h>
#define __fastcall
#define __cdecl
#endif
#define _ogg_calloc calloc
#define VIF_POSIT 63
#define VIF_CLASS 16
#define VIF_PARTS 31
typedef void vorbis_info_floor;
typedef void vorbis_look_floor;
typedef struct vorbis_dsp_state vorbis_dsp_state;
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
  int sorted_index[VIF_POSIT+2];
  int forward_index[VIF_POSIT+2];
  int reverse_index[VIF_POSIT+2];
  
  int hineighbor[VIF_POSIT];
  int loneighbor[VIF_POSIT];
  int posts;

  int n;
  int quant_q;
  vorbis_info_floor1 *vi;

  long phrasebits;
  long postbits;
  long frames;
} vorbis_look_floor1;
extern "C" {
static int __cdecl icomp(const void *a,const void *b){
  return(**(int **)a-**(int **)b);
}
}
extern "C" vorbis_look_floor* __fastcall floor1_look(vorbis_dsp_state *vd,
				      vorbis_info_floor *in){

  int *sortpointer[VIF_POSIT+2];
  vorbis_info_floor1 *info=(vorbis_info_floor1 *)in;
  vorbis_look_floor1 *look=(vorbis_look_floor1*)_ogg_calloc(1,sizeof(*look));
  int i,j,n=0;

  look->vi=info;
  look->n=info->postlist[1];
 
  /* we drop each position value in-between already decoded values,
     and use linear interpolation to predict each new value past the
     edges.  The positions are read in the order of the position
     list... we precompute the bounding positions in the lookup.  Of
     course, the neighbors can change (if a position is declined), but
     this is an initial mapping */

  for(i=0;i<info->partitions;i++)n+=info->class_dim[info->partitionclass[i]];
  n+=2;
  look->posts=n;

  /* also store a sorted position index */
  for(i=0;i<n;i++)sortpointer[i]=info->postlist+i;
  qsort(sortpointer,n,sizeof(*sortpointer),icomp);

  /* points from sort order back to range number */
  for(i=0;i<n;i++)look->forward_index[i]=sortpointer[i]-info->postlist;
  /* points from range order to sorted position */
  for(i=0;i<n;i++)look->reverse_index[look->forward_index[i]]=i;
  /* we actually need the post values too */
  for(i=0;i<n;i++)look->sorted_index[i]=info->postlist[look->forward_index[i]];
  
  /* quantize values to multiplier spec */
  switch(info->mult){
  case 1: /* 1024 -> 256 */
    look->quant_q=256;
    break;
  case 2: /* 1024 -> 128 */
    look->quant_q=128;
    break;
  case 3: /* 1024 -> 86 */
    look->quant_q=86;
    break;
  case 4: /* 1024 -> 64 */
    look->quant_q=64;
    break;
  }

  /* discover our neighbors for decode where we don't use fit flags
     (that would push the neighbors outward) */
  for(i=0;i<n-2;i++){
    int lo=0;
    int hi=1;
    int lx=0;
    int hx=look->n;
    int currentx=info->postlist[i+2];
    for(j=0;j<i+2;j++){
      int x=info->postlist[j];
      if(x>lx && x<currentx){
	lo=j;
	lx=x;
      }
      if(x<hx && x>currentx){
	hi=j;
	hx=x;
      }
    }
    look->loneighbor[i]=lo;
    look->hineighbor[i]=hi;
  }

  return(look);
}
typedef char look_size_check[sizeof(vorbis_look_floor1)==0x520?1:-1];

#ifndef _MSC_VER
#undef calloc
#endif
extern "C" void __cdecl abort();
static unsigned cases,allocations;
static union{double alignment;unsigned char bytes[0x540];}memory;
static void check(bool p){if(!p){fprintf(stderr,"floor lookup case %u failed\n",cases);abort();}}
extern "C" void* __cdecl test_calloc(unsigned n,unsigned size){check(n==1&&size==0x520&&allocations==0);++allocations;memset(memory.bytes+8,0,size);return memory.bytes+8;}
#ifdef _MSC_VER
static void __cdecl test_qsort(void*base,unsigned n,unsigned size,int(__cdecl*compare)(const void*,const void*)){check(size==sizeof(int*));int**p=(int**)base;for(unsigned i=1;i<n;++i){int*value=p[i];unsigned j=i;while(j&&compare(p+j-1,&value)>0){p[j]=p[j-1];--j;}p[j]=value;}}
extern "C" {void*(__cdecl* _imp__calloc)(unsigned,unsigned)=test_calloc;void(__cdecl* _imp__qsort)(void*,unsigned,unsigned,int(__cdecl*)(const void*,const void*))=test_qsort;}
#endif
int main(){for(int parts=0;parts<=8;++parts)for(int shape=0;shape<5;++shape)for(int seed=0;seed<5;++seed)for(int mult=1;mult<=4;++mult){vorbis_info_floor1 info;memset(&info,0,sizeof info);info.partitions=parts;info.mult=mult;int n=2;for(int i=0;i<16;++i)info.class_dim[i]=shape==0?1:(shape==1?i%8+1:(shape==2?8:(shape==3?(i%2?7:2):(i%8==7?7:8))));for(int i=0;i<parts;++i){info.partitionclass[i]=i;n+=info.class_dim[i];}if(n>65)continue;info.postlist[0]=0;info.postlist[1]=256;for(int i=2;i<n;++i)info.postlist[i]=1+((i-2)*37+seed*17)%255;vorbis_info_floor1 original=info;memset(memory.bytes,0xa6,sizeof memory.bytes);allocations=0;vorbis_look_floor1*look=(vorbis_look_floor1*)floor1_look(0,&info);check(look==(void*)(memory.bytes+8)&&allocations==1&&look->vi==&info&&look->n==256&&look->posts==n&&look->quant_q==(256+mult-1)/mult);for(int i=0;i<n;++i){int rank=0;for(int j=0;j<n;++j)if(info.postlist[j]<info.postlist[i])++rank;check(look->forward_index[rank]==i&&look->reverse_index[i]==rank&&look->sorted_index[rank]==info.postlist[i]);}for(int i=2;i<n;++i){int lo=0,hi=1;for(int j=0;j<i;++j){int x=info.postlist[j];if(x<info.postlist[i]&&x>info.postlist[lo])lo=j;if(x>info.postlist[i]&&x<info.postlist[hi])hi=j;}check(look->loneighbor[i-2]==lo&&look->hineighbor[i-2]==hi);}for(int i=n;i<65;++i)check(!look->forward_index[i]&&!look->reverse_index[i]&&!look->sorted_index[i]);for(int i=n-2;i<63;++i)check(!look->loneighbor[i]&&!look->hineighbor[i]);check(!look->phrasebits&&!look->postbits&&!look->frames&&memcmp(&info,&original,sizeof info)==0);for(int i=0;i<8;++i)check(memory.bytes[i]==0xa6&&memory.bytes[8+0x520+i]==0xa6);++cases;}check(cases==880);puts("00ca1be0_TEST PASS");return 0;}