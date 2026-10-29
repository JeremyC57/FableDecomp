// 00c93f60 _vp_offset_and_mix
// Leaf: @_vp_offset_and_mix@20
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
extern "C" void __fastcall _vp_offset_and_mix(vorbis_look_psy *p,
			float *noise,
			float *tone,
			int offset_select,
			float *logmask){
  int i,n=p->n;
  float toneatt=p->vi->tone_masteratt[offset_select];
  
  for(i=0;i<n;i++){
    float val= noise[i]+p->noiseoffset[offset_select][i];
    if(val>p->vi->noisemaxsupp)val=p->vi->noisemaxsupp;
    logmask[i]=max(val,tone[i]+toneatt);
  }
}
typedef char info_offset_check[offsetof(vorbis_info_psy,normal_thresh)==520?1:-1];
typedef char look_size_check[sizeof(vorbis_look_psy)==48?1:-1];