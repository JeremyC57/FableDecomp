#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static unsigned cases;
static void check(bool p){if(!p){fprintf(stderr,"noise normalize case %u failed\n",cases);abort();}}
static double absval(double x){return x<0?-x:x;}
static double down(double x){int n=(int)x;return n>x?n-1:n;}
#ifdef _MSC_VER
static double __cdecl test_floor(double x){check(x==x&&x>-8192&&x<8192);return down(x);}
extern "C" {double(__cdecl* _imp__floor)(double)=test_floor;}
#endif
static float signed_unit(float x){unsigned bits;memcpy(&bits,&x,4);return bits>>31?-1.f:1.f;}
int main(){const int parts[]={1,2,4,8,16};const double thresholds[]={0.,0.25,0.5,1.,2.,4.};
for(int n=0;n<=40;++n)for(int pi=0;pi<5;++pi)for(int startcase=0;startcase<5;++startcase)for(int ti=0;ti<6;++ti)for(int flag=0;flag<2;++flag)for(int seed=0;seed<3;++seed){
 vorbis_info_psy info;vorbis_look_psy look;memset(&info,0,sizeof info);memset(&look,0,sizeof look);
 int start=startcase==0?0:(startcase==1?1:(startcase==2?n/2:(startcase==3?n:n+2))),partition=parts[pi];
 info.normal_start=start;info.normal_partition=partition;info.normal_channel_p=flag;info.normal_thresh=thresholds[ti];look.n=n;look.vi=&info;
 float input[44],before[44],output[44],expected[44];int order[44],originalOrder[44];
 for(int i=0;i<44;++i){input[i]=before[i]=((i*7+seed*3)%15-7)*0.25f;output[i]=expected[i]=4096.f;order[i]=0x123456;}
 for(int i=0;i<n;++i)expected[i+1]=(float)down(input[i+1]+0.5);
 int begin=start<n?start:n;
 for(int base=begin;base+partition<=n;base+=partition){
  int rank[16];for(int k=0;k<partition;++k)rank[k]=base+k;
  for(int k=1;k<partition;++k){int v=rank[k],at=k;while(at>0&&absval(input[rank[at-1]+1])<absval(input[v+1])){rank[at]=rank[at-1];--at;}rank[at]=v;}
  for(int k=0;k<partition;++k)order[base-begin+k+1]=rank[k];
  if(flag){
   double available=0;for(int k=0;k<partition;++k)if(absval(input[base+k+1])<0.5)available+=(double)input[base+k+1]*input[base+k+1];
   for(int k=0;k<partition;++k){int at=rank[k]+1;if(absval(input[at])<0.5){if(available>=thresholds[ti]){expected[at]=signed_unit(input[at]);available-=1;}else expected[at]=0;}}
  }
 }
 memcpy(originalOrder,order,sizeof order);vorbis_info_psy infoBefore=info;vorbis_look_psy lookBefore=look;
 _vp_noise_normalize(&look,input+1,output+1,order+1);
 for(int i=0;i<44;++i)check(output[i]==expected[i]&&input[i]==before[i]);
 check(memcmp(order,originalOrder,sizeof order)==0&&memcmp(&info,&infoBefore,sizeof info)==0&&memcmp(&look,&lookBefore,sizeof look)==0);++cases;
}
check(cases==36900);puts("00c94810_TEST PASS");return 0;}