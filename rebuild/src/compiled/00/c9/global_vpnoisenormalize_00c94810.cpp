// 00c94810 _vp_noise_normalize
// Leaf: @_vp_noise_normalize@16
// Flags: /Gr
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

#ifndef _MSC_VER
#define __cdecl
#define __declspec(x)
#endif
typedef unsigned int ogg_uint32_t;
extern "C" __declspec(dllimport) double __cdecl floor(double);
#define rint(x) (floor((x)+0.5f))
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
extern "C" float __fastcall unitnorm(float x){
  ogg_uint32_t *ix=(ogg_uint32_t *)&x;
  *ix=(*ix&0x80000000UL)|(0x3f800000UL);
  return(x);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
extern "C" void __fastcall _vp_noise_normalize(vorbis_look_psy *p,
			 float *in,float *out,int *sortedindex){
  int flag=0,i,j=0,n=p->n;
  vorbis_info_psy *vi=p->vi;
  int partition=vi->normal_partition;
  int start=vi->normal_start;

  if(start>n)start=n;

  if(vi->normal_channel_p){
    for(;j<start;j++)
      out[j]=rint(in[j]);
    
    for(;j+partition<=n;j+=partition){
      float acc=0.;
      int k;
      
      for(i=j;i<j+partition;i++)
	acc+=in[i]*in[i];
      
      for(i=0;i<partition;i++){
	k=sortedindex[i+j-start];
	
	if(in[k]*in[k]>=.25f){
	  out[k]=rint(in[k]);
	  acc-=in[k]*in[k];
	  flag=1;
	}else{
	  if(acc<vi->normal_thresh)break;
	  out[k]=unitnorm(in[k]);
	  acc-=1.;
	}
      }
      
      for(;i<partition;i++){
	k=sortedindex[i+j-start];
	out[k]=0.;
      }
    }
  }
  
  for(;j<n;j++)
    out[j]=rint(in[j]);
  
}