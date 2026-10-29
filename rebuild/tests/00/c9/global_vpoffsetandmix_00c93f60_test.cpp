#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#undef max
static unsigned cases;
static void check(bool p){if(!p){fprintf(stderr,"mix case %u failed\n",cases);abort();}}
int main(){
 for(int n=0;n<=64;++n)for(int choice=0;choice<3;++choice)for(int clamp=-2;clamp<=2;++clamp)for(int alias=0;alias<3;++alias)for(int seed=0;seed<3;++seed){
 vorbis_info_psy info;vorbis_look_psy look;memset(&info,0,sizeof info);memset(&look,0,sizeof look);
 float noise[68],tone[68],output[68],oldNoise[68],oldTone[68],oldOutput[68],offsets[3][68],*offsetPtrs[3];
 for(int j=0;j<3;++j){offsetPtrs[j]=offsets[j]+1;info.tone_masteratt[j]=(j-1)*0.5f;for(int i=0;i<68;++i)offsets[j][i]=((i+seed+j)%7-3)*0.25f;}
 for(int i=0;i<68;++i){noise[i]=oldNoise[i]=((i*5+seed*7)%23-11)*0.25f;tone[i]=oldTone[i]=((i*7+seed*3)%19-9)*0.25f;output[i]=oldOutput[i]=16.0f;}
 info.noisemaxsupp=clamp*0.5f;look.n=n;look.vi=&info;look.noiseoffset=offsetPtrs;
 vorbis_info_psy oldInfo=info;vorbis_look_psy oldLook=look;
 float*dest=alias==1?noise:(alias==2?tone:output);
 _vp_offset_and_mix(&look,noise+1,tone+1,choice,dest+1);
 for(int i=0;i<68;++i){bool active=i>=1&&i<=n;float expected=alias==1?oldNoise[i]:(alias==2?oldTone[i]:oldOutput[i]);if(active){float a=oldNoise[i]+offsets[choice][i];if(a>info.noisemaxsupp)a=info.noisemaxsupp;float b=oldTone[i]+info.tone_masteratt[choice];expected=a>b?a:b;}check(dest[i]==expected);if(alias!=1)check(noise[i]==oldNoise[i]);if(alias!=2)check(tone[i]==oldTone[i]);if(alias!=0)check(output[i]==oldOutput[i]);}
 check(memcmp(&info,&oldInfo,sizeof info)==0&&memcmp(&look,&oldLook,sizeof look)==0);++cases;
 }
 check(cases==8775);puts("00c93f60_TEST PASS");return 0;
}