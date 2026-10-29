#include <stdio.h>
#include <string.h>
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

#undef max
extern "C" void abort();
static unsigned cases,passes;static int currentN,currentSeed,currentFixed;static float*originalSpectrum;static const long*currentBark;
static const float levels[]={-4.0f,-1.0f,-0.5f,0.0f,0.25f,0.5f,1.0f,1.25f,38.5f,39.5f,45.0f};
static void check(bool p){if(!p){fprintf(stderr,"noise case %u failed\n",cases);abort();}}
// Regression dependency contracts validate the two passes and supply controlled mask levels.
extern "C" void __stdcall bark_noise_hybridmp(int n,const long*b,const float*f,float*out,float offset,int fixed){check(n==currentN&&b==currentBark&&passes<2);if(passes==0){check(f==originalSpectrum&&offset==140.0f&&fixed==-1);for(int i=0;i<n;++i)out[i]=f[i]*0.5f+1.0f;}else{check(offset==0.0f&&fixed==currentFixed);for(int i=0;i<n;++i){check(f[i]==originalSpectrum[i]-(originalSpectrum[i]*0.5f+1.0f));out[i]=levels[(i+currentSeed)%11];}}++passes;}
int main(){const int fixeds[]={-1,0,4};for(int n=0;n<=64;++n)for(int seed=0;seed<4;++seed)for(int mode=0;mode<3;++mode){vorbis_info_psy info;vorbis_look_psy p;memset(&info,0,sizeof info);memset(&p,0,sizeof p);float data[68],copy[68],mask[68];long bark[64];for(int i=0;i<40;++i)info.noisecompand[i]=i*0.125f;for(int i=0;i<68;++i){data[i]=copy[i]=((i*7+seed)%31-15)*0.5f;mask[i]=64.0f;}for(int i=0;i<64;++i)bark[i]=i*65537;info.noisewindowfixed=fixeds[mode];p.n=n;p.vi=&info;p.bark=bark;currentN=n;currentSeed=seed;currentFixed=fixeds[mode];originalSpectrum=data+1;currentBark=bark;passes=0;_vp_noisemask(&p,data+1,mask+1);check(passes==2);for(int i=0;i<n;++i){int index=(int)(levels[(i+seed)%11]+0.5);if(index<0)index=0;if(index>39)index=39;float expected=copy[i+1]*0.5f+1.0f+info.noisecompand[index];check(mask[i+1]==expected);}check(mask[0]==64.0f);for(int i=n+1;i<68;++i)check(mask[i]==64.0f);check(memcmp(data,copy,sizeof data)==0&&p.n==n&&p.vi==&info);++cases;}check(cases==780);puts("00c93c40_TEST PASS");return 0;}