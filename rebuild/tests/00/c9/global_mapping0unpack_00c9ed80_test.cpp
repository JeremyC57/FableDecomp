#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define calloc test_calloc
#endif
// 00c9ed80 mapping0_unpack
// Leaf: @mapping0_unpack@8
// Flags: /Gr
// Original libvorbis 1.0.1 cleanup body and public/internal layouts.
#include <stddef.h>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#pragma intrinsic(memset)
#else
#include <stdint.h>
typedef int64_t ogg_int64_t __attribute__((aligned(8)));
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
typedef unsigned int ogg_uint32_t;
#define DATA_TYPE float
extern "C" void* __cdecl memset(void*,int,unsigned int);
extern "C" __declspec(dllimport) void __cdecl free(void*);
#define _ogg_free free
typedef struct envelope_lookup envelope_lookup;
typedef struct vorbis_info_psy vorbis_info_psy;
#define P_NOISECURVES 3
typedef struct vorbis_info_psy_global vorbis_info_psy_global;
typedef struct {
  float ampmax;
  int   channels;

  vorbis_info_psy_global *gi;
  int   coupling_pointlimit[2][P_NOISECURVES];  
} vorbis_look_psy_global;
typedef void vorbis_look_transform;
typedef void vorbis_look_floor;
typedef void vorbis_look_residue;
typedef void vorbis_info_floor;
typedef void vorbis_info_residue;
typedef void vorbis_info_mapping;
typedef struct vorbis_block vorbis_block;
typedef struct {
  long endbyte;
  int  endbit;

  unsigned char *buffer;
  unsigned char *ptr;
  long storage;
} oggpack_buffer;
typedef struct {
  unsigned char *packet;
  long  bytes;
  long  b_o_s;
  long  e_o_s;

  ogg_int64_t  granulepos;
  
  ogg_int64_t  packetno;     /* sequence number for decode; the framing
				knows where there's a hole in the data,
				but we need coupling so that the codec
				(which is in a seperate abstraction
				layer) also knows about the gap */
} ogg_packet;
typedef struct vorbis_info{
  int version;
  int channels;
  long rate;

  /* The below bitrate declarations are *hints*.
     Combinations of the three values carry the following implications:
     
     all three set to the same value: 
       implies a fixed rate bitstream
     only nominal set: 
       implies a VBR stream that averages the nominal bitrate.  No hard 
       upper/lower limit
     upper and or lower set: 
       implies a VBR bitstream that obeys the bitrate limits. nominal 
       may also be set to give a nominal rate.
     none set:
       the coder does not care to speculate.
  */

  long bitrate_upper;
  long bitrate_nominal;
  long bitrate_lower;
  long bitrate_window;

  void *codec_setup;
} vorbis_info;
typedef struct vorbis_dsp_state{
  int analysisp;
  vorbis_info *vi;

  float **pcm;
  float **pcmret;
  int      pcm_storage;
  int      pcm_current;
  int      pcm_returned;

  int  preextrapolate;
  int  eofflag;

  long lW;
  long W;
  long nW;
  long centerW;

  ogg_int64_t granulepos;
  ogg_int64_t sequence;

  ogg_int64_t glue_bits;
  ogg_int64_t time_bits;
  ogg_int64_t floor_bits;
  ogg_int64_t res_bits;

  void       *backend_state;
} vorbis_dsp_state;
typedef struct {
  int n;
  float *trigcache;
  int *splitcache;
} drft_lookup;
typedef struct {
  int n;
  int log2n;
  
  DATA_TYPE *trig;
  int       *bitrev;

  DATA_TYPE scale;
} mdct_lookup;
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
typedef struct bitrate_manager_state {
  ogg_uint32_t  *queue_binned;
  ogg_uint32_t  *queue_actual;
  int            queue_size;

  int            queue_head;
  int            queue_bins;

  long          *avg_binacc;
  int            avg_center;
  int            avg_tail;
  ogg_uint32_t   avg_centeracc;
  ogg_uint32_t   avg_sampleacc;
  ogg_uint32_t   avg_sampledesired;
  ogg_uint32_t   avg_centerdesired;

  long          *minmax_binstack;
  long          *minmax_posstack;
  long          *minmax_limitstack;
  long           minmax_stackptr;

  long           minmax_acctotal;
  int            minmax_tail;
  ogg_uint32_t   minmax_sampleacc;
  ogg_uint32_t   minmax_sampledesired;

  int            next_to_flush;
  int            last_to_flush;
  
  double         avgfloat;

  /* unfortunately, we need to hold queued packet data somewhere */
  oggpack_buffer *packetbuffers;
  ogg_packet     *packets;

} bitrate_manager_state;
typedef struct private_state {
  /* local lookup storage */
  envelope_lookup        *ve; /* envelope lookup */    
  int                     window[2];
  vorbis_look_transform **transform[2];    /* block, type */
  drft_lookup             fft_look[2];

  int                     modebits;
  vorbis_look_floor     **flr;
  vorbis_look_residue   **residue;
  vorbis_look_psy        *psy;
  vorbis_look_psy_global *psy_g_look;

  /* local storage, only used on the encoding side.  This way the
     application does not need to worry about freeing some packets'
     memory and not others'; packet storage is always tracked.
     Cleared next call to a _dsp_ function */
  unsigned char *header;
  unsigned char *header1;
  unsigned char *header2;

  bitrate_manager_state bms;

  ogg_int64_t sample_count;
} private_state;

typedef struct {
  int blockflag;
  int windowtype;
  int transformtype;
  int mapping;
} vorbis_info_mode;
// Layout: original setup prefix through residue_type; following fields are unaccessed.
typedef struct static_codebook{
  long   dim;            /* codebook dimensions (elements per vector) */
  long   entries;        /* codebook entries */
  long  *lengthlist;     /* codeword lengths in bits */

  /* mapping ***************************************************************/
  int    maptype;        /* 0=none
			    1=implicitly populated values from map column 
			    2=listed arbitrary values */

  /* The below does a linear, single monotonic sequence mapping. */
  long     q_min;       /* packed 32 bit float; quant value 0 maps to minval */
  long     q_delta;     /* packed 32 bit float; val 1 - val 0 == delta */
  int      q_quant;     /* bits: 0 < quant <= 16 */
  int      q_sequencep; /* bitflag */

  long     *quantlist;  /* map == 1: (int)(entries^(1/dim)) element column map
			   map == 2: list of dim*entries quantized entry vals
			*/

  /* encode helpers ********************************************************/
  struct encode_aux_nearestmatch *nearest_tree;
  struct encode_aux_threshmatch  *thresh_tree;
  struct encode_aux_pigeonhole  *pigeon_tree;

  int allocedp;
} static_codebook;
typedef struct codebook{
  long dim;           /* codebook dimensions (elements per vector) */
  long entries;       /* codebook entries */
  long used_entries;  /* populated codebook entries */
  const static_codebook *c;

  /* for encode, the below are entry-ordered, fully populated */
  /* for decode, the below are ordered by bitreversed codeword and only
     used entries are populated */
  float        *valuelist;  /* list of dim*entries actual entry values */  
  ogg_uint32_t *codelist;   /* list of bitstream codewords for each entry */

  int          *dec_index;  /* only used if sparseness collapsed */
  char         *dec_codelengths;
  ogg_uint32_t *dec_firsttable;
  int           dec_firsttablen;
  int           dec_maxlength;

} codebook;
// Layout: original setup prefix through psy_param; trailing encoder-only fields are unaccessed.
#define VE_BANDS 7
#define PACKETBLOBS 15
#ifdef _MSC_VER
#define VORBIS_ABI_DOUBLE double
#else
typedef double FableAlignedDouble __attribute__((aligned(8)));
#define VORBIS_ABI_DOUBLE FableAlignedDouble
#endif
typedef struct vorbis_info_psy_global{
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
typedef struct bitrate_manager_info{
  /* detailed bitrate management setup */
  VORBIS_ABI_DOUBLE queue_avg_time;
  VORBIS_ABI_DOUBLE queue_avg_center;
  VORBIS_ABI_DOUBLE queue_minmax_time;
  VORBIS_ABI_DOUBLE queue_hardmin;
  VORBIS_ABI_DOUBLE queue_hardmax;
  VORBIS_ABI_DOUBLE queue_avgmin;
  VORBIS_ABI_DOUBLE queue_avgmax;

  VORBIS_ABI_DOUBLE avgfloat_downslew_max;
  VORBIS_ABI_DOUBLE avgfloat_upslew_max;

} bitrate_manager_info;
typedef struct highlevel_byblocktype {
  VORBIS_ABI_DOUBLE tone_mask_setting;
  VORBIS_ABI_DOUBLE tone_peaklimit_setting;
  VORBIS_ABI_DOUBLE noise_bias_setting;
  VORBIS_ABI_DOUBLE noise_compand_setting;
} highlevel_byblocktype;
typedef struct highlevel_encode_setup {
  void *setup;
  int   set_in_stone;

  VORBIS_ABI_DOUBLE base_setting;
  VORBIS_ABI_DOUBLE long_setting;
  VORBIS_ABI_DOUBLE short_setting;
  VORBIS_ABI_DOUBLE impulse_noisetune;

  int    managed;
  long   bitrate_min;
  long   bitrate_av_lo;
  long   bitrate_av_hi;
  long   bitrate_max;
  VORBIS_ABI_DOUBLE bitrate_limit_window;
  VORBIS_ABI_DOUBLE bitrate_av_window;
  VORBIS_ABI_DOUBLE bitrate_av_window_center;
  
  int impulse_block_p;
  int noise_normalize_p;

  VORBIS_ABI_DOUBLE stereo_point_setting;
  VORBIS_ABI_DOUBLE lowpass_kHz;

  VORBIS_ABI_DOUBLE ath_floating_dB;
  VORBIS_ABI_DOUBLE ath_absolute_dB;

  VORBIS_ABI_DOUBLE amplitude_track_dBpersec;
  VORBIS_ABI_DOUBLE trigger_setting;
  
  highlevel_byblocktype block[4]; /* padding, impulse, transition, long */

} highlevel_encode_setup;
typedef struct codec_setup_info {

  /* Vorbis supports only short and long blocks, but allows the
     encoder to choose the sizes */

  long blocksizes[2];

  /* modes are the primary means of supporting on-the-fly different
     blocksizes, different channel mappings (LR or M/A),
     different residue backends, etc.  Each mode consists of a
     blocksize flag and a mapping (along with the mapping setup */

  int        modes;
  int        maps;
  int        floors;
  int        residues;
  int        books;
  int        psys;     /* encode only */

  vorbis_info_mode       *mode_param[64];
  int                     map_type[64];
  vorbis_info_mapping    *map_param[64];
  int                     floor_type[64];
  vorbis_info_floor      *floor_param[64];
  int                     residue_type[64];
  vorbis_info_residue    *residue_param[64];
  static_codebook        *book_param[256];
  codebook               *fullbooks;

  vorbis_info_psy        *psy_param[4]; /* encode only */
  vorbis_info_psy_global psy_g_param;

  bitrate_manager_info   bi;
  highlevel_encode_setup hi; /* used only by vorbisenc.c.  It's a
                                highly redundant structure, but
                                improves clarity of program flow. */
  int         halfrate_flag; /* painless downsample for decode */  
} codec_setup_info;
typedef struct{
  void                   (__fastcall *pack)  (vorbis_info_floor *,oggpack_buffer *);
  vorbis_info_floor     *(__fastcall *unpack)(vorbis_info *,oggpack_buffer *);
  vorbis_look_floor     *(__fastcall *look)  (vorbis_dsp_state *,vorbis_info_floor *);
  void (__fastcall *free_info) (vorbis_info_floor *);
  void (__fastcall *free_look) (vorbis_look_floor *);
  void *(__fastcall *inverse1)  (struct vorbis_block *,vorbis_look_floor *);
  int   (__fastcall *inverse2)  (struct vorbis_block *,vorbis_look_floor *,
		     void *buffer,float *);
} vorbis_func_floor;
typedef struct{
  void                 (__fastcall *pack)  (vorbis_info_residue *,oggpack_buffer *);
  vorbis_info_residue *(__fastcall *unpack)(vorbis_info *,oggpack_buffer *);
  vorbis_look_residue *(__fastcall *look)  (vorbis_dsp_state *,
				 vorbis_info_residue *);
  void (__fastcall *free_info)    (vorbis_info_residue *);
  void (__fastcall *free_look)    (vorbis_look_residue *);
} vorbis_func_residue;
typedef struct{
  void                 (__fastcall *pack)  (vorbis_info *,vorbis_info_mapping *,
				 oggpack_buffer *);
  vorbis_info_mapping *(__fastcall *unpack)(vorbis_info *,oggpack_buffer *);
  void (__fastcall *free_info)    (vorbis_info_mapping *);
  int  (__fastcall *forward)      (struct vorbis_block *vb);
  int  (__fastcall *inverse)      (struct vorbis_block *vb,vorbis_info_mapping *);
} vorbis_func_mapping;


typedef struct vorbis_block{
  /* necessary stream state for linking to the framing abstraction */
  float  **pcm;       /* this is a pointer into local storage */ 
  oggpack_buffer opb;
  
  long  lW;
  long  W;
  long  nW;
  int   pcmend;
  int   mode;

  int         eofflag;
  ogg_int64_t granulepos;
  ogg_int64_t sequence;
  vorbis_dsp_state *vd; /* For read-only access of configuration */

  /* local storage to avoid remallocing; it's up to the mapping to
     structure it */
  void               *localstore;
  long                localtop;
  long                localalloc;
  long                totaluse;
  struct alloc_chain *reap;

  /* bitmetrics for the frame */
  long glue_bits;
  long time_bits;
  long floor_bits;
  long res_bits;

  void *internal;

} vorbis_block;
#define BLOCKTYPE_IMPULSE 0
#define BLOCKTYPE_PADDING 1
#define BLOCKTYPE_TRANSITION 0
#define BLOCKTYPE_LONG 1
typedef struct vorbis_block_internal{
  float  **pcmdelay;  /* this is a pointer into local storage */ 
  float  ampmax;
  int    blocktype;

  ogg_uint32_t   packetblob_markers[PACKETBLOBS];
} vorbis_block_internal;


typedef struct vorbis_info_mapping0{
  int   submaps;  /* <= 16 */
  int   chmuxlist[256];   /* up to 256 channels in a Vorbis stream */
  
  int   floorsubmap[16];   /* [mux] submap to floors */
  int   residuesubmap[16]; /* [mux] submap to residue */

  int   coupling_steps;
  int   coupling_mag[256];
  int   coupling_ang[256];

} vorbis_info_mapping0;
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int,unsigned int);
#define _ogg_calloc calloc
extern "C" long __fastcall oggpack_read(oggpack_buffer*,int);
extern "C" void __fastcall mapping0_free_info(vorbis_info_mapping*);
// Fidelity: This original C file-local ilog is distinct from the residue helper.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
namespace {
int __fastcall ilog(unsigned int v){
  int ret=0;
  if(v)--v;
  while(v){
    ret++;
    v>>=1;
  }
  return(ret);
}
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
extern "C" vorbis_info_mapping* __fastcall mapping0_unpack(vorbis_info *vi,oggpack_buffer *opb){
  int i;
  vorbis_info_mapping0 *info=(vorbis_info_mapping0*)_ogg_calloc(1,sizeof(*info));
  codec_setup_info     *ci=(codec_setup_info*)vi->codec_setup;
  memset(info,0,sizeof(*info));

  if(oggpack_read(opb,1))
    info->submaps=oggpack_read(opb,4)+1;
  else
    info->submaps=1;

  if(oggpack_read(opb,1)){
    info->coupling_steps=oggpack_read(opb,8)+1;

    for(i=0;i<info->coupling_steps;i++){
      int testM=info->coupling_mag[i]=oggpack_read(opb,ilog(vi->channels));
      int testA=info->coupling_ang[i]=oggpack_read(opb,ilog(vi->channels));

      if(testM<0 || 
	 testA<0 || 
	 testM==testA || 
	 testM>=vi->channels ||
	 testA>=vi->channels) goto err_out;
    }

  }

  if(oggpack_read(opb,2)>0)goto err_out; /* 2,3:reserved */
    
  if(info->submaps>1){
    for(i=0;i<vi->channels;i++){
      info->chmuxlist[i]=oggpack_read(opb,4);
      if(info->chmuxlist[i]>=info->submaps)goto err_out;
    }
  }
  for(i=0;i<info->submaps;i++){
    oggpack_read(opb,8); /* time submap unused */
    info->floorsubmap[i]=oggpack_read(opb,8);
    if(info->floorsubmap[i]>=ci->floors)goto err_out;
    info->residuesubmap[i]=oggpack_read(opb,8);
    if(info->residuesubmap[i]>=ci->residues)goto err_out;
  }

  return info;

 err_out:
  mapping0_free_info(info);
  return(NULL);
}

#ifndef _MSC_VER
#undef calloc
#endif
extern "C" void __cdecl abort();
static unsigned cases,allocated,released,position,length;
static int values[2048],widths[2048],kinds[2048],indices[2048],faultIndex,faultValue;
static union{double alignment;unsigned char bytes[0xca0];}memory;
static oggpack_buffer*currentBuffer;
static void check(bool p){if(!p){fprintf(stderr,"mapping unpack case %u pos %u fault %d value %d failed\n",cases,position,faultIndex,faultValue);abort();}}
extern "C" void* __cdecl test_calloc(unsigned n,unsigned bytes){check(n==1&&bytes==0xc88&&allocated==0);++allocated;memset(memory.bytes+8,0,bytes);return memory.bytes+8;}
#ifdef _MSC_VER
extern "C" {void*(__cdecl* _imp__calloc)(unsigned,unsigned)=test_calloc;}
#endif
extern "C" void __fastcall mapping0_free_info(vorbis_info_mapping*p){check(p==memory.bytes+8&&allocated==1&&released==0);++released;memset(p,0,0xc88);}
extern "C" long __fastcall oggpack_read(oggpack_buffer*p,int n){check(p==currentBuffer&&position<length&&n==widths[position]);unsigned at=position++;return (int)at==faultIndex?faultValue:values[at];}
static void add(int value,int width,int kind,int index){check(length<2048);values[length]=value;widths[length]=width;kinds[length]=kind;indices[length++]=index;}
static void run(vorbis_info*vi,const vorbis_info_mapping0*expected,int fault,int value,bool succeeds){oggpack_buffer buffer;memset(&buffer,0,sizeof buffer);currentBuffer=&buffer;faultIndex=fault;faultValue=value;position=allocated=released=0;memset(memory.bytes,0xb5,sizeof memory.bytes);vorbis_info viBefore=*vi;codec_setup_info setupBefore=*(codec_setup_info*)vi->codec_setup;void*result=mapping0_unpack(vi,&buffer);check(allocated==1);if(succeeds){check(result==memory.bytes+8&&released==0&&position==length&&memcmp(result,expected,sizeof(*expected))==0);}else{check(result==0&&released==1);check(position==(unsigned)fault+(kinds[fault]==1?2:1));for(int i=0;i<0xc88;++i)check(memory.bytes[i+8]==0);}check(memcmp(vi,&viBefore,sizeof viBefore)==0&&memcmp(vi->codec_setup,&setupBefore,sizeof setupBefore)==0);for(int i=0;i<8;++i)check(memory.bytes[i]==0xb5&&memory.bytes[8+0xc88+i]==0xb5);++cases;}
int main(){const int channelList[]={1,2,3,8,31,256};const int mapList[]={1,2,7,16};const int pairList[]={0,1,3,16,256};
for(int ci=0;ci<6;++ci)for(int mi=0;mi<4;++mi)for(int pi=0;pi<5;++pi)for(int seed=0;seed<2;++seed){
 int ch=channelList[ci],maps=mapList[mi],pairs=pairList[pi];if(ch==1&&pairs)continue;
 vorbis_info vi;codec_setup_info setup;vorbis_info_mapping0 expected;
 memset(&vi,0,sizeof vi);memset(&setup,0,sizeof setup);memset(&expected,0,sizeof expected);
 vi.channels=ch;vi.codec_setup=&setup;setup.floors=8;setup.residues=9;expected.submaps=maps;expected.coupling_steps=pairs;length=0;
 add(maps>1,1,0,0);if(maps>1)add(maps-1,4,0,0);
 add(pairs>0,1,0,0);if(pairs){add(pairs-1,8,0,0);int width=0;while((1U<<width)<(unsigned)ch)++width;for(int p=0;p<pairs;++p){expected.coupling_mag[p]=(p+seed)%ch;expected.coupling_ang[p]=(p+seed+1)%ch;add(expected.coupling_mag[p],width,1,p);add(expected.coupling_ang[p],width,2,p);}}
 add(0,2,3,0);
 if(maps>1)for(int c=0;c<ch;++c){expected.chmuxlist[c]=(c+seed)%maps;add(expected.chmuxlist[c],4,4,c);}
 for(int m=0;m<maps;++m){add(0,8,7,m);expected.floorsubmap[m]=(m+seed)%8;add(expected.floorsubmap[m],8,5,m);expected.residuesubmap[m]=(m+2*seed)%9;add(expected.residuesubmap[m],8,6,m);}
 run(&vi,&expected,-1,0,true);
 for(unsigned f=0;f<length;++f){int kind=kinds[f],index=indices[f];
  if(kind==1||kind==2){run(&vi,&expected,f,-1,false);if((1U<<widths[f])>(unsigned)ch)run(&vi,&expected,f,ch,false);run(&vi,&expected,f,kind==1?expected.coupling_ang[index]:expected.coupling_mag[index],false);}
  if(kind==3){run(&vi,&expected,f,1,false);run(&vi,&expected,f,3,false);run(&vi,&expected,f,-1,true);}
  if(kind>=4&&kind<=6){int bound=kind==4?maps:(kind==5?setup.floors:setup.residues);if(bound<(1<<widths[f]))run(&vi,&expected,f,bound,false);vorbis_info_mapping0 changed=expected;if(kind==4)changed.chmuxlist[index]=-1;else if(kind==5)changed.floorsubmap[index]=-1;else changed.residuesubmap[index]=-1;run(&vi,&changed,f,-1,true);}
 }
}
check(cases>10000);puts("00c9ed80_TEST PASS");return 0;}