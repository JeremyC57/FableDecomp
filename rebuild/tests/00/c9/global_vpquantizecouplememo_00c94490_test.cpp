#include <stdio.h>
#include <string.h>
// 00c94490 _vp_quantize_couple_memo
// Leaf: @_vp_quantize_couple_memo@20
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

#define VE_BANDS 7
#define PACKETBLOBS 15
typedef struct{
  int   eighth_octave_lines;

  /* for block long/short tuning; encode only */
  float preecho_thresh[VE_BANDS];
  float postecho_thresh[VE_BANDS];
  float stretch_penalty;
  float preecho_minenergy;

  float ampmax_att_per_sec;

  /* channel coupling config */
  int   coupling_pkHz[PACKETBLOBS];  
  int   coupling_pointlimit[2][PACKETBLOBS];  
  int   coupling_prepointamp[PACKETBLOBS];  
  int   coupling_postpointamp[PACKETBLOBS];  
  int   sliding_lowpass[2][PACKETBLOBS];  

} vorbis_info_psy_global;
typedef struct vorbis_info_mapping0{
  int   submaps;  /* <= 16 */
  int   chmuxlist[256];   /* up to 256 channels in a Vorbis stream */
  
  int   floorsubmap[16];   /* [mux] submap to floors */
  int   residuesubmap[16]; /* [mux] submap to residue */

  int   coupling_steps;
  int   coupling_mag[256];
  int   coupling_ang[256];

} vorbis_info_mapping0;
struct vorbis_block;
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block*,long);
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#pragma auto_inline(off)
#endif
extern "C" float __fastcall dipole_hypot(float a, float b){
  if(a>0.){
    if(b>0.)return sqrt(a*a+b*b);
    if(a>-b)return sqrt(a*a-b*b);
    return -sqrt(b*b-a*a);
  }
  if(b<0.)return -sqrt(a*a+b*b);
  if(-a>b)return -sqrt(a*a-b*b);
  return sqrt(b*b-a*a);
}
extern "C" float __fastcall round_hypot(float a, float b){
  if(a>0.){
    if(b>0.)return sqrt(a*a+b*b);
    if(a>-b)return sqrt(a*a+b*b);
    return -sqrt(b*b+a*a);
  }
  if(b<0.)return -sqrt(a*a+b*b);
  if(-a>b)return -sqrt(a*a+b*b);
  return sqrt(b*b+a*a);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
extern "C" float** __fastcall _vp_quantize_couple_memo(vorbis_block *vb,
				 vorbis_info_psy_global *g,
				 vorbis_look_psy *p,
				 vorbis_info_mapping0 *vi,
				 float **mdct){
  
  int i,j,n=p->n;
  float **ret=(float**)_vorbis_block_alloc(vb,vi->coupling_steps*sizeof(*ret));
  int limit=g->coupling_pointlimit[p->vi->blockflag][PACKETBLOBS/2];
  
  for(i=0;i<vi->coupling_steps;i++){
    float *mdctM=mdct[vi->coupling_mag[i]];
    float *mdctA=mdct[vi->coupling_ang[i]];
    ret[i]=(float*)_vorbis_block_alloc(vb,n*sizeof(**ret));
    for(j=0;j<limit;j++)
      ret[i][j]=dipole_hypot(mdctM[j],mdctA[j]);
    for(;j<n;j++)
      ret[i][j]=round_hypot(mdctM[j],mdctA[j]);
  }

  return(ret);
}
typedef char global_offset_check[offsetof(vorbis_info_psy_global,coupling_pointlimit)==132?1:-1];

#undef max
extern "C" void abort();
static unsigned cases,used,allocations;static unsigned arena[300];static int ownerTag;
static void check(bool p){if(!p){fprintf(stderr,"couple memo case %u failed\n",cases);abort();}}
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block*owner,long bytes){check(owner==(vorbis_block*)&ownerTag&&bytes>=0&&bytes%4==0&&used+bytes/4+2<300);void*result=arena+used+1;used+=bytes/4+2;++allocations;return result;}
static double absolute(double x){return x<0?-x:x;}
// Independent sign-and-energy formulation; actual original helpers execute in the pasted source.
static float expected(float a,float b,bool dipole){double energy;int sign;if(!dipole){energy=(double)a*a+(double)b*b;sign=absolute(a)>absolute(b)?(a<0?-1:1):(b<0?-1:1);}else if(a>0&&b>0){energy=(double)a*a+(double)b*b;sign=1;}else if(a<=0&&b<0){energy=(double)a*a+(double)b*b;sign=-1;}else{double difference=(double)a*a-(double)b*b;energy=absolute(difference);sign=a>0?(difference>0?1:-1):(difference>0?-1:1);}return (float)(sign*sqrt(energy));}
int main(){const int sizes[]={0,1,2,7,16,32};const float values[]={-5.0f,-4.0f,-3.0f,0.0f,3.0f,4.0f,5.0f};for(int t=0;t<6;++t){int n=sizes[t];for(int limit=0;limit<=n;++limit)for(int steps=0;steps<=3;++steps)for(int flag=0;flag<2;++flag)for(int seed=0;seed<3;++seed){
 vorbis_info_psy info;vorbis_look_psy look;vorbis_info_psy_global global;vorbis_info_mapping0 mapping;memset(&info,0,sizeof info);memset(&look,0,sizeof look);memset(&global,0,sizeof global);memset(&mapping,0,sizeof mapping);info.blockflag=flag;look.vi=&info;look.n=n;global.coupling_pointlimit[flag][7]=limit;mapping.coupling_steps=steps;float data[4][32],original[4][32],*channels[4];for(int c=0;c<4;++c){channels[c]=data[c];for(int i=0;i<32;++i)data[c][i]=original[c][i]=values[(i*3+c*2+seed)%7];}for(int c=0;c<3;++c){mapping.coupling_mag[c]=c;mapping.coupling_ang[c]=(c+seed+1)%4;}
 used=allocations=0;for(int i=0;i<300;++i)arena[i]=0x4e7b95adU;float**result=_vp_quantize_couple_memo((vorbis_block*)&ownerTag,&global,&look,&mapping,channels);check(result==(float**)(arena+1)&&allocations==(unsigned)(steps+1));for(int c=0;c<steps;++c)for(int i=0;i<n;++i){float want=expected(data[mapping.coupling_mag[c]][i],data[mapping.coupling_ang[c]][i],i<limit);check(result[c][i]==result[c][i]&&absolute((double)result[c][i]-want)<1.0/65536.0);}check(memcmp(data,original,sizeof data)==0&&arena[0]==0x4e7b95adU&&arena[used-1]==0x4e7b95adU&&arena[used]==0x4e7b95adU);++cases;
 }}check(cases==1536);puts("00c94490_TEST PASS");return 0;}