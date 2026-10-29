#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#include <stdlib.h>
#define malloc test_malloc
#define calloc test_calloc
#endif
// 00c9db40 _ve_envelope_init
// Leaf: @_ve_envelope_init@8
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
#define VE_AMP 17
#define VE_NEARDC 15
#define VE_BANDS 7
typedef struct {
  float ampbuf[VE_AMP];
  int   ampptr;

  float nearDC[VE_NEARDC];
  float nearDC_acc;
  float nearDC_partialacc;
  int   nearptr;

} envelope_filter_state;

typedef struct {
  int begin;
  int end;
  float *window;
  float total;
} envelope_band;

typedef struct {
  int ch;
  int winlength;
  int searchstep;
  float minenergy;

  mdct_lookup  mdct;
  float       *mdct_win;

  envelope_band          band[VE_BANDS];
  envelope_filter_state *filter;
  int   stretch;

  int                   *mark;

  long storage;
  long current;
  long curmark;
  long cursor;
} envelope_lookup;

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


#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sin)
#endif
#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.1415926536f
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int);
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int,unsigned int);
#define _ogg_malloc malloc
#define _ogg_calloc calloc
extern "C" void __fastcall mdct_init(mdct_lookup*,int);
extern "C" void __fastcall _ve_envelope_init(envelope_lookup *e,vorbis_info *vi){
  codec_setup_info *ci=(codec_setup_info*)vi->codec_setup;
  vorbis_info_psy_global *gi=&ci->psy_g_param;
  int ch=vi->channels;
  int i,j;
  int n=e->winlength=128;
  e->searchstep=64; /* not random */

  e->minenergy=gi->preecho_minenergy;
  e->ch=ch;
  e->storage=128;
  e->cursor=ci->blocksizes[1]/2;
  e->mdct_win=(float*)_ogg_calloc(n,sizeof(*e->mdct_win));
  mdct_init(&e->mdct,n);

  for(i=0;i<n;i++){
    e->mdct_win[i]=sin(i/(n-1.)*M_PI);
    e->mdct_win[i]*=e->mdct_win[i];
  }

  /* magic follows */
  e->band[0].begin=2;  e->band[0].end=4;
  e->band[1].begin=4;  e->band[1].end=5;
  e->band[2].begin=6;  e->band[2].end=6;
  e->band[3].begin=9;  e->band[3].end=8;
  e->band[4].begin=13;  e->band[4].end=8;
  e->band[5].begin=17;  e->band[5].end=8;
  e->band[6].begin=22;  e->band[6].end=8;

  for(j=0;j<VE_BANDS;j++){
    n=e->band[j].end;
    e->band[j].window=(float*)_ogg_malloc(n*sizeof(*e->band[0].window));
    for(i=0;i<n;i++){
      e->band[j].window[i]=sin((i+.5)/n*M_PI);
      e->band[j].total+=e->band[j].window[i];
    }
    e->band[j].total=1./e->band[j].total;
  }
  
  e->filter=(envelope_filter_state*)_ogg_calloc(VE_BANDS*ch,sizeof(*e->filter));
  e->mark=(int*)_ogg_calloc(e->storage,sizeof(*e->mark));

}
typedef char envelope_size_check[sizeof(envelope_lookup)==180?1:-1];
typedef char filter_size_check[sizeof(envelope_filter_state)==144?1:-1];

#ifndef _MSC_VER
#undef malloc
#undef calloc
#endif
extern "C" void __cdecl abort();
static unsigned cases,used,count,mdctCalls;
static union {double alignment;unsigned char bytes[16384];} arena;
static void*ptrs[12];static unsigned sizes[12];static bool zeros[12];
static mdct_lookup*expectedMDCT;
static void check(bool p){if(!p){fprintf(stderr,"envelope init case %u failed\n",cases);abort();}}
static void*allocate(unsigned bytes,bool zero){check(count<12);unsigned begin=(used+15)&~7U;check(begin+bytes+8<sizeof arena.bytes);ptrs[count]=arena.bytes+begin;sizes[count]=bytes;zeros[count]=zero;++count;if(zero)memset(arena.bytes+begin,0,bytes);used=begin+bytes+8;return arena.bytes+begin;}
extern "C" void* __cdecl test_malloc(unsigned bytes){return allocate(bytes,false);}
extern "C" void* __cdecl test_calloc(unsigned n,unsigned bytes){return allocate(n*bytes,true);}
#ifdef _MSC_VER
extern "C" {void* (__cdecl* _imp__malloc)(unsigned)=test_malloc;void* (__cdecl* _imp__calloc)(unsigned,unsigned)=test_calloc;}
#endif
extern "C" void __fastcall mdct_init(mdct_lookup*p,int n){check(p==expectedMDCT&&n==128&&!mdctCalls);++mdctCalls;p->n=n;p->log2n=7;p->trig=0;p->bitrev=0;p->scale=0.03125f;}
static double absolute(double x){return x<0?-x:x;}
static void closeValue(float actual,double expected){check(actual==actual&&absolute(actual-expected)<1.0/1048576.0);}
int main(){const int largeBlocks[]={64,128,256,1024,129};const float energies[]={-128.0f,-64.0f,0.0f,16.0f};const int starts[]={2,4,6,9,13,17,22};const int spans[]={4,5,6,8,8,8,8};for(int channels=0;channels<=6;++channels)for(int b=0;b<5;++b)for(int e=0;e<4;++e){
 envelope_lookup lookup;vorbis_info info;codec_setup_info setup;memset(&lookup,0,sizeof lookup);memset(&info,0,sizeof info);memset(&setup,0,sizeof setup);memset(arena.bytes,0xad,sizeof arena.bytes);used=count=mdctCalls=0;info.channels=channels;info.codec_setup=&setup;setup.blocksizes[1]=largeBlocks[b];setup.psy_g_param.preecho_minenergy=energies[e];lookup.stretch=3;lookup.current=19;lookup.curmark=23;expectedMDCT=&lookup.mdct;
 _ve_envelope_init(&lookup,&info);check(count==10&&mdctCalls==1&&lookup.ch==channels&&lookup.winlength==128&&lookup.searchstep==64&&lookup.minenergy==energies[e]&&lookup.storage==128&&lookup.cursor==largeBlocks[b]/2);check(lookup.stretch==3&&lookup.current==19&&lookup.curmark==23&&lookup.mdct.n==128&&lookup.mdct.log2n==7);check(lookup.mdct_win==ptrs[0]&&sizes[0]==128*4&&zeros[0]);
 for(int i=0;i<128;++i){float s=(float)sin((double)i/127.0*(double)M_PI);closeValue(lookup.mdct_win[i],s*s);}
 for(int band=0;band<7;++band){check(lookup.band[band].begin==starts[band]&&lookup.band[band].end==spans[band]&&lookup.band[band].window==ptrs[band+1]&&sizes[band+1]==(unsigned)spans[band]*4&&!zeros[band+1]);double sum=0;for(int i=0;i<spans[band];++i){float s=(float)sin(((double)i+0.5)/spans[band]*(double)M_PI);closeValue(lookup.band[band].window[i],s);sum+=s;}closeValue(lookup.band[band].total,1.0/sum);}
 check(lookup.filter==ptrs[8]&&sizes[8]==7U*channels*144&&zeros[8]);check(lookup.mark==ptrs[9]&&sizes[9]==128*4&&zeros[9]);for(unsigned k=0;k<count;++k){unsigned char*p=(unsigned char*)ptrs[k];for(int guard=1;guard<=8;++guard)check(p[-guard]==0xad&&p[sizes[k]+guard-1]==0xad);if(k>=8)for(unsigned i=0;i<sizes[k];++i)check(p[i]==0);}++cases;
 }check(cases==140);puts("00c9db40_TEST PASS");return 0;}