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