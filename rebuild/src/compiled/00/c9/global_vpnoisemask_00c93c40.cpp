// 00c93c40 _vp_noisemask
// Leaf: @_vp_noisemask@12
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

#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#define __stdcall
#endif
extern "C" void __stdcall bark_noise_hybridmp(int,const long*,const float*,float*,float,int);
extern "C" void __fastcall _vp_noisemask(vorbis_look_psy *p,
		   float *logmdct, 
		   float *logmask){

  int i,n=p->n;
  float *work=(float*)alloca(n*sizeof(*work));

  bark_noise_hybridmp(n,p->bark,logmdct,logmask,
		      140.,-1);

  for(i=0;i<n;i++)work[i]=logmdct[i]-logmask[i];

  bark_noise_hybridmp(n,p->bark,work,logmask,0.,
		      p->vi->noisewindowfixed);

  for(i=0;i<n;i++)work[i]=logmdct[i]-work[i];
  
#if 0
  {
    static int seq=0;

    float work2[n];
    for(i=0;i<n;i++){
      work2[i]=logmask[i]+work[i];
    }
    
    if(seq&1)
      _analysis_output("median2R",seq/2,work,n,1,0,0);
    else
      _analysis_output("median2L",seq/2,work,n,1,0,0);
    
    if(seq&1)
      _analysis_output("envelope2R",seq/2,work2,n,1,0,0);
    else
      _analysis_output("envelope2L",seq/2,work2,n,1,0,0);
    seq++;
  }
#endif

  for(i=0;i<n;i++){
    int dB=logmask[i]+.5;
    if(dB>=NOISE_COMPAND_LEVELS)dB=NOISE_COMPAND_LEVELS-1;
    if(dB<0)dB=0;
    logmask[i]= work[i]+p->vi->noisecompand[dB];
  }

}