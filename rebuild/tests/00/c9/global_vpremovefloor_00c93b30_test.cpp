#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#undef max
extern "C" {float FLOOR1_fromdB_INV_LOOKUP[256];}
static unsigned cases;
static void check(bool p){if(!p){fprintf(stderr,"floor case %u failed\n",cases);abort();}}
int main(){for(int i=0;i<256;++i)FLOOR1_fromdB_INV_LOOKUP[i]=(i+1)*(1.0f/256.0f);for(int n=0;n<=64;++n)for(int cutoff=0;cutoff<=n+3;++cutoff)for(int alias=0;alias<2;++alias)for(int seed=0;seed<3;++seed){vorbis_look_psy p;memset(&p,0,sizeof p);p.n=n;float mdct[68],original[68],residue[68];int floor[68],oldFloor[68];for(int i=0;i<68;++i){mdct[i]=original[i]=((i*7+seed*3)%17-8)*0.125f;residue[i]=16.0f;floor[i]=oldFloor[i]=(i*19+seed*31)%256;}float*out=alias?mdct:residue;_vp_remove_floor(&p,mdct+1,floor+1,out+1,cutoff);int stop=cutoff<n?cutoff:n;for(int i=0;i<68;++i){float wanted=alias?original[i]:16.0f;if(i>=1&&i<=n)wanted=i<=stop?original[i]*FLOOR1_fromdB_INV_LOOKUP[floor[i]]:0.0f;check(out[i]==wanted);if(!alias)check(mdct[i]==original[i]);check(floor[i]==oldFloor[i]);}check(p.n==n);++cases;}check(cases==14040);puts("00c93b30_TEST PASS");return 0;}