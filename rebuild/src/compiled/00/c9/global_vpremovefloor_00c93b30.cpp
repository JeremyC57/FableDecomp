// 00c93b30 _vp_remove_floor
// Leaf: @_vp_remove_floor@20
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

extern "C" float FLOOR1_fromdB_INV_LOOKUP[256];
extern "C" void __fastcall _vp_remove_floor(vorbis_look_psy *p,
		      float *mdct,
		      int *codedflr,
		      float *residue,
		      int sliding_lowpass){ 

  int i,n=p->n;
 
  if(sliding_lowpass>n)sliding_lowpass=n;
  
  for(i=0;i<sliding_lowpass;i++){
    residue[i]=
      mdct[i]*FLOOR1_fromdB_INV_LOOKUP[codedflr[i]];
  }

  for(;i<n;i++)
    residue[i]=0.;
}