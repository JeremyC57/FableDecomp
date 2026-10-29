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