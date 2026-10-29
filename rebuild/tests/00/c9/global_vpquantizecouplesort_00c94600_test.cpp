#include <stdio.h>
#include <string.h>
// 00c94600 _vp_quantize_couple_sort
// Leaf: @_vp_quantize_couple_sort@16
// Flags: /Gr /Oy-
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

#include <math.h>
#ifdef _MSC_VER
#include <malloc.h>
#pragma intrinsic(fabs)
#else
#include <alloca.h>
#define __cdecl
#endif
struct vorbis_block;
typedef struct vorbis_info_mapping0{
  int   submaps;  /* <= 16 */
  int   chmuxlist[256];   /* up to 256 channels in a Vorbis stream */
  
  int   floorsubmap[16];   /* [mux] submap to floors */
  int   residuesubmap[16]; /* [mux] submap to residue */

  int   coupling_steps;
  int   coupling_mag[256];
  int   coupling_ang[256];

} vorbis_info_mapping0;
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block*,long);
#ifdef _MSC_VER
extern "C" __declspec(dllimport) void __cdecl qsort(void*,unsigned int,unsigned int,int (__cdecl*)(const void*,const void*));
#else
#include <stdlib.h>
#endif
extern "C" {
static int __cdecl apsort(const void *a, const void *b){
  float f1=fabs(**(float**)a);
  float f2=fabs(**(float**)b);
  return (f1<f2)-(f1>f2);
}
}
extern "C" int** __fastcall _vp_quantize_couple_sort(vorbis_block *vb,
			       vorbis_look_psy *p,
			       vorbis_info_mapping0 *vi,
			       float **mags){


  if(p->vi->normal_point_p){
    int i,j,k,n=p->n;
    int **ret=(int**)_vorbis_block_alloc(vb,vi->coupling_steps*sizeof(*ret));
    int partition=p->vi->normal_partition;
    float **work=(float**)alloca(sizeof(*work)*partition);
    
    for(i=0;i<vi->coupling_steps;i++){
      ret[i]=(int*)_vorbis_block_alloc(vb,n*sizeof(**ret));
      
      for(j=0;j<n;j+=partition){
	for(k=0;k<partition;k++)work[k]=mags[i]+k+j;
	qsort(work,partition,sizeof(*work),apsort);
	for(k=0;k<partition;k++)ret[i][k+j]=work[k]-mags[i];
      }
    }
    return(ret);
  }
  return(NULL);
}
typedef char mapping_offset_check[offsetof(vorbis_info_mapping0,coupling_steps)==0x484?1:-1];

#undef max
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" void __cdecl abort();
static unsigned cases,used,allocations;
static unsigned arena[1100];
static int ownerTag;
static void check(bool p){if(!p){fprintf(stderr,"couple sort case %u failed\n",cases);abort();}}
// Block owner is an opaque identity; no invented layout or dereference.
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block*owner,long bytes){check(owner==(vorbis_block*)&ownerTag&&bytes>=0&&bytes%4==0&&used+bytes/4+2<1100);unsigned*result=arena+used+1;used+=bytes/4+2;++allocations;return result;}
#ifdef _MSC_VER
// Test-only import adapter sorts pointer elements with the actual apsort callback.
static void __cdecl test_qsort(void*base,unsigned count,unsigned width,int (__cdecl*compare)(const void*,const void*)){check(width==sizeof(float*));float**v=(float**)base;for(unsigned i=1;i<count;++i){float*value=v[i];unsigned j=i;while(j&&compare(&v[j-1],&value)>0){v[j]=v[j-1];--j;}v[j]=value;}}
extern "C" {void (__cdecl* _imp__qsort)(void*,unsigned,unsigned,int (__cdecl*)(const void*,const void*))=test_qsort;}
#endif
static float magnitude(float x){return x<0?-x:x;}
int main(){
 const int sizes[]={0,1,2,8,16,32,64};const int partitions[]={1,2,4,8,16};
 for(int t=0;t<7;++t)for(int pi=0;pi<5;++pi){int n=sizes[t],part=partitions[pi];if(n%part)continue;for(int enabled=0;enabled<2;++enabled)for(int steps=0;steps<=3;++steps)for(int seed=0;seed<4;++seed){
 vorbis_info_psy info;vorbis_look_psy look;vorbis_info_mapping0 mapping;memset(&info,0,sizeof info);memset(&look,0,sizeof look);memset(&mapping,0,sizeof mapping);float input[3][64],copy[3][64],*channels[3];for(int c=0;c<3;++c){channels[c]=input[c];for(int i=0;i<64;++i)input[c][i]=copy[c][i]=((i*7+c*11+seed*3)%23-11)*0.25f;}
 look.n=n;look.vi=&info;info.normal_point_p=enabled;info.normal_partition=part;mapping.coupling_steps=steps;unsigned sentinel=0x5e7a9b0dU;for(int i=0;i<1100;++i)arena[i]=sentinel;used=allocations=0;
 int**result=_vp_quantize_couple_sort((vorbis_block*)&ownerTag,&look,&mapping,channels);
 if(!enabled)check(result==0&&allocations==0);else{check(result==(int**)(arena+1)&&allocations==(unsigned)(steps+1));for(int c=0;c<steps;++c){for(int j=0;j<n;j+=part){bool seen[16];for(int k=0;k<16;++k)seen[k]=false;float previous=1024.0f;for(int k=0;k<part;++k){int index=result[c][j+k];check(index>=j&&index<j+part&&!seen[index-j]);seen[index-j]=true;float value=magnitude(input[c][index]);check(previous>=value);previous=value;}}}check(arena[0]==sentinel&&arena[used-1]==sentinel&&arena[used]==sentinel);}
 check(memcmp(input,copy,sizeof input)==0);++cases;
 }}
 check(cases==864);puts("00c94600_TEST PASS");return 0;
}