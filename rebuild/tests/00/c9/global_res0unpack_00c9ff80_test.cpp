#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define calloc test_calloc
#endif
// 00c9ff80 res0_unpack
// Leaf: @res0_unpack@8
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


typedef struct vorbis_info_residue0{
/* block-partitioned VQ coded straight residue */
  long  begin;
  long  end;

  /* first stage (lossless partitioning) */
  int    grouping;         /* group n vectors per partition */
  int    partitions;       /* possible codebooks for a partition */
  int    groupbook;        /* huffbook for partitioning */
  int    secondstages[64]; /* expanded out to pointers in lookup */
  int    booklist[256];    /* list of second stage books */

  float  classmetric1[64];  
  float  classmetric2[64];  

} vorbis_info_residue0;
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int,unsigned int);
#define _ogg_calloc calloc
extern "C" long __fastcall oggpack_read(oggpack_buffer*,int);
extern "C" void __fastcall res0_free_info(vorbis_info_residue*);
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
extern "C" int __fastcall icount(unsigned int v){
  int ret=0;
  while(v){
    ret+=v&1;
    v>>=1;
  }
  return(ret);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
extern "C" vorbis_info_residue* __fastcall res0_unpack(vorbis_info *vi,oggpack_buffer *opb){
  int j,acc=0;
  vorbis_info_residue0 *info=(vorbis_info_residue0*)_ogg_calloc(1,sizeof(*info));
  codec_setup_info     *ci=(codec_setup_info*)vi->codec_setup;

  info->begin=oggpack_read(opb,24);
  info->end=oggpack_read(opb,24);
  info->grouping=oggpack_read(opb,24)+1;
  info->partitions=oggpack_read(opb,6)+1;
  info->groupbook=oggpack_read(opb,8);

  for(j=0;j<info->partitions;j++){
    int cascade=oggpack_read(opb,3);
    if(oggpack_read(opb,1))
      cascade|=(oggpack_read(opb,5)<<3);
    info->secondstages[j]=cascade;

    acc+=icount(cascade);
  }
  for(j=0;j<acc;j++)
    info->booklist[j]=oggpack_read(opb,8);

  if(info->groupbook>=ci->books)goto errout;
  for(j=0;j<acc;j++)
    if(info->booklist[j]>=ci->books)goto errout;

  return(info);
 errout:
  res0_free_info(info);
  return(NULL);
}

#ifndef _MSC_VER
#undef calloc
#endif
extern "C" void __cdecl abort();
static unsigned cases,allocated,released,position,length;
static int tokens[600],widths[600],faultAt,faultValue,firstBook;
static union{double alignment;unsigned char bytes[0x740];}memory;
static oggpack_buffer*currentBuffer;
static void check(bool p){if(!p){fprintf(stderr,"residue unpack case %u failed\n",cases);abort();}}
extern "C" void* __cdecl test_calloc(unsigned n,unsigned bytes){check(n==1&&bytes==0x714&&allocated==0);++allocated;memset(memory.bytes+8,0,bytes);return memory.bytes+8;}
#ifdef _MSC_VER
extern "C" {void*(__cdecl* _imp__calloc)(unsigned,unsigned)=test_calloc;}
#endif
extern "C" void __fastcall res0_free_info(vorbis_info_residue*p){check(p==memory.bytes+8&&allocated==1&&released==0);++released;memset(p,0,0x714);}
extern "C" long __fastcall oggpack_read(oggpack_buffer*p,int bits){check(p==currentBuffer&&position<length&&bits==widths[position]);unsigned at=position++;return (int)at==faultAt?faultValue:tokens[at];}
static void add(int value,int bits){check(length<600);tokens[length]=value;widths[length++]=bits;}
static void run(vorbis_info*vi,const vorbis_info_residue0*expected,int fault,int value,bool error){oggpack_buffer buffer;memset(&buffer,0,sizeof buffer);currentBuffer=&buffer;faultAt=fault;faultValue=value;position=allocated=released=0;memset(memory.bytes,0xc5,sizeof memory.bytes);void*result=res0_unpack(vi,&buffer);check(allocated==1&&position==length);if(error){check(!result&&released==1);for(int i=0;i<0x714;++i)check(memory.bytes[i+8]==0);}else{check(result==memory.bytes+8&&!released&&memcmp(result,expected,sizeof(*expected))==0);}for(int i=0;i<8;++i)check(memory.bytes[i]==0xc5&&memory.bytes[8+0x714+i]==0xc5);++cases;}
static void configuration(int parts,unsigned mask,int seed){vorbis_info info;codec_setup_info setup;vorbis_info_residue0 expected;memset(&info,0,sizeof info);memset(&setup,0,sizeof setup);memset(&expected,0,sizeof expected);setup.books=16;info.codec_setup=&setup;length=0;expected.begin=mask*257;expected.end=0xffffff;expected.grouping=seed?8:1;expected.partitions=parts;expected.groupbook=seed?15:0;add(expected.begin,24);add(expected.end,24);add(expected.grouping-1,24);add(parts-1,6);add(expected.groupbook,8);int books=0;for(int i=0;i<parts;++i){unsigned shift=i%8;unsigned stage=((mask<<shift)|(mask>>(8-shift)))&255U;expected.secondstages[i]=stage;add(stage&7,3);add(stage>7,1);if(stage>7)add(stage>>3,5);for(int k=0;k<8;++k)books+=(stage>>k)&1;}check(books<=256);firstBook=length;for(int i=0;i<books;++i){expected.booklist[i]=(i*7+seed*3)%16;add(expected.booklist[i],8);}run(&info,&expected,-1,0,false);run(&info,&expected,4,16,true);if(books){run(&info,&expected,firstBook,16,true);run(&info,&expected,length-1,16,true);expected.booklist[books-1]=-1;run(&info,&expected,length-1,-1,false);}}
int main(){for(int parts=1;parts<=16;++parts)for(unsigned mask=0;mask<256;++mask)for(int seed=0;seed<2;++seed)configuration(parts,mask,seed);for(int seed=0;seed<2;++seed){configuration(32,255,seed);configuration(64,15,seed);configuration(64,240,seed);configuration(64,0,seed);}check(cases==40898);puts("00c9ff80_TEST PASS");return 0;}