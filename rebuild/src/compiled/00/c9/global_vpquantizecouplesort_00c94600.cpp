// 00c94600 _vp_quantize_couple_sort
// Leaf: @_vp_quantize_couple_sort@16
// Flags: /Gr /Oy-
// Original libvorbis psy.c and psy.h; BSD-style terms in reference COPYING.
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
typedef double FableAlignedDouble __attribute__((aligned(8)));
#else
typedef double FableAlignedDouble;
#endif
#define P_BANDS 17
#define P_NOISECURVES 3
#define NOISE_COMPAND_LEVELS 40
#define max(x,y) ((x)>(y)?(x):(y))
typedef struct vorbis_info_psy{
  int   blockflag;

  float ath_adjatt;
  float ath_maxatt;

  float tone_masteratt[P_NOISECURVES];
  float tone_centerboost;
  float tone_decay;
  float tone_abs_limit;
  float toneatt[P_BANDS];

  int noisemaskp;
  float noisemaxsupp;
  float noisewindowlo;
  float noisewindowhi;
  int   noisewindowlomin;
  int   noisewindowhimin;
  int   noisewindowfixed;
  float noiseoff[P_NOISECURVES][P_BANDS];
  float noisecompand[NOISE_COMPAND_LEVELS];

  float max_curve_dB;

  int normal_channel_p;
  int normal_point_p;
  int normal_start;
  int normal_partition;
  FableAlignedDouble normal_thresh;
} vorbis_info_psy;
typedef struct {
  int n;
  struct vorbis_info_psy *vi;

  float ***tonecurves;
  float **noiseoffset;

  float *ath;
  long  *octave;             /* in n.ocshift format */
  long  *bark;

  long  firstoc;
  long  shiftoc;
  int   eighth_octave_lines; /* power of two, please */
  int   total_octave_lines;  
  long  rate; /* cache it */
} vorbis_look_psy;

#include <math.h>
#ifdef _MSC_VER
#include <malloc.h>
#pragma intrinsic(fabs)
#else
#include <alloca.h>
#define __cdecl
#endif
struct vorbis_block;
typedef struct vorbis_info_mapping0{
  int   submaps;  /* <= 16 */
  int   chmuxlist[256];   /* up to 256 channels in a Vorbis stream */
  
  int   floorsubmap[16];   /* [mux] submap to floors */
  int   residuesubmap[16]; /* [mux] submap to residue */

  int   coupling_steps;
  int   coupling_mag[256];
  int   coupling_ang[256];

} vorbis_info_mapping0;
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block*,long);
#ifdef _MSC_VER
extern "C" __declspec(dllimport) void __cdecl qsort(void*,unsigned int,unsigned int,int (__cdecl*)(const void*,const void*));
#else
#include <stdlib.h>
#endif
extern "C" {
static int __cdecl apsort(const void *a, const void *b){
  float f1=fabs(**(float**)a);
  float f2=fabs(**(float**)b);
  return (f1<f2)-(f1>f2);
}
}
extern "C" int** __fastcall _vp_quantize_couple_sort(vorbis_block *vb,
			       vorbis_look_psy *p,
			       vorbis_info_mapping0 *vi,
			       float **mags){


  if(p->vi->normal_point_p){
    int i,j,k,n=p->n;
    int **ret=(int**)_vorbis_block_alloc(vb,vi->coupling_steps*sizeof(*ret));
    int partition=p->vi->normal_partition;
    float **work=(float**)alloca(sizeof(*work)*partition);
    
    for(i=0;i<vi->coupling_steps;i++){
      ret[i]=(int*)_vorbis_block_alloc(vb,n*sizeof(**ret));
      
      for(j=0;j<n;j+=partition){
	for(k=0;k<partition;k++)work[k]=mags[i]+k+j;
	qsort(work,partition,sizeof(*work),apsort);
	for(k=0;k<partition;k++)ret[i][k+j]=work[k]-mags[i];
      }
    }
    return(ret);
  }
  return(NULL);
}
typedef char mapping_offset_check[offsetof(vorbis_info_mapping0,coupling_steps)==0x484?1:-1];