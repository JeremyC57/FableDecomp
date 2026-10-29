#include <stdio.h>
// 00c8fba0 vorbis_analysis_blockout
// Leaf: @vorbis_analysis_blockout@8
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
#ifdef _MSC_VER
#pragma intrinsic(memcpy)
#endif
extern "C" void* __cdecl memcpy(void*,const void*,unsigned int);
extern "C" __declspec(dllimport) void* __cdecl memmove(void*,const void*,unsigned int);
extern "C" {
long __fastcall _ve_envelope_search(vorbis_dsp_state*);
int __fastcall _ve_envelope_mark(vorbis_dsp_state*);
void __fastcall _vorbis_block_ripcord(vorbis_block*);
void* __fastcall _vorbis_block_alloc(vorbis_block*,long);
float __fastcall _vp_ampmax_decay(float,vorbis_dsp_state*);
void __fastcall _ve_envelope_shift(envelope_lookup*,long);
}
extern "C" int __fastcall vorbis_analysis_blockout(vorbis_dsp_state *v,vorbis_block *vb){
  int i;
  vorbis_info *vi=v->vi;
  codec_setup_info *ci=(codec_setup_info*)vi->codec_setup;
  private_state *b=(private_state*)v->backend_state;
  vorbis_look_psy_global *g=b->psy_g_look;
  long beginW=v->centerW-ci->blocksizes[v->W]/2,centerNext;
  vorbis_block_internal *vbi=(vorbis_block_internal *)vb->internal;

  /* check to see if we're started... */
  if(!v->preextrapolate)return(0);

  /* check to see if we're done... */
  if(v->eofflag==-1)return(0);

  /* By our invariant, we have lW, W and centerW set.  Search for
     the next boundary so we can determine nW (the next window size)
     which lets us compute the shape of the current block's window */

  /* we do an envelope search even on a single blocksize; we may still
     be throwing more bits at impulses, and envelope search handles
     marking impulses too. */
  {  
    long bp=_ve_envelope_search(v);
    if(bp==-1){

      if(v->eofflag==0)return(0); /* not enough data currently to search for a
				     full long block */
      v->nW=0;
    }else{

      if(ci->blocksizes[0]==ci->blocksizes[1])
	v->nW=0;
      else
	v->nW=bp;
    }
  }

  centerNext=v->centerW+ci->blocksizes[v->W]/4+ci->blocksizes[v->nW]/4;

  {
    /* center of next block + next block maximum right side. */

    long blockbound=centerNext+ci->blocksizes[v->nW]/2;
    if(v->pcm_current<blockbound)return(0); /* not enough data yet;
                                               although this check is
                                               less strict that the
                                               _ve_envelope_search,
                                               the search is not run
                                               if we only use one
                                               block size */


  }
  
  /* fill in the block.  Note that for a short window, lW and nW are *short*
     regardless of actual settings in the stream */

  _vorbis_block_ripcord(vb);
  vb->lW=v->lW;
  vb->W=v->W;
  vb->nW=v->nW;

  if(v->W){
    if(!v->lW || !v->nW){
      vbi->blocktype=BLOCKTYPE_TRANSITION;
      /*fprintf(stderr,"-");*/
    }else{
      vbi->blocktype=BLOCKTYPE_LONG;
      /*fprintf(stderr,"_");*/
    }
  }else{
    if(_ve_envelope_mark(v)){
      vbi->blocktype=BLOCKTYPE_IMPULSE;
      /*fprintf(stderr,"|");*/

    }else{
      vbi->blocktype=BLOCKTYPE_PADDING;
      /*fprintf(stderr,".");*/

    }
  }
 
  vb->vd=v;
  vb->sequence=v->sequence++;
  vb->granulepos=v->granulepos;
  vb->pcmend=ci->blocksizes[v->W];
  
  /* copy the vectors; this uses the local storage in vb */

  /* this tracks 'strongest peak' for later psychoacoustics */
  /* moved to the global psy state; clean this mess up */
  if(vbi->ampmax>g->ampmax)g->ampmax=vbi->ampmax;
  g->ampmax=_vp_ampmax_decay(g->ampmax,v);
  vbi->ampmax=g->ampmax;
  
  vb->pcm=(float**)_vorbis_block_alloc(vb,sizeof(*vb->pcm)*vi->channels);
  vbi->pcmdelay=(float**)_vorbis_block_alloc(vb,sizeof(*vbi->pcmdelay)*vi->channels);
  for(i=0;i<vi->channels;i++){
    vbi->pcmdelay[i]=
      (float*)_vorbis_block_alloc(vb,(vb->pcmend+beginW)*sizeof(*vbi->pcmdelay[i]));
    memcpy(vbi->pcmdelay[i],v->pcm[i],(vb->pcmend+beginW)*sizeof(*vbi->pcmdelay[i]));
    vb->pcm[i]=vbi->pcmdelay[i]+beginW;
    
    /* before we added the delay 
       vb->pcm[i]=_vorbis_block_alloc(vb,vb->pcmend*sizeof(*vb->pcm[i]));
       memcpy(vb->pcm[i],v->pcm[i]+beginW,ci->blocksizes[v->W]*sizeof(*vb->pcm[i]));
    */
    
  }
  
  /* handle eof detection: eof==0 means that we've not yet received EOF
                           eof>0  marks the last 'real' sample in pcm[]
                           eof<0  'no more to do'; doesn't get here */

  if(v->eofflag){
    if(v->centerW>=v->eofflag){
      v->eofflag=-1;
      vb->eofflag=1;
      return(1);
    }
  }

  /* advance storage vectors and clean up */
  {
    int new_centerNext=ci->blocksizes[1]/2;
    int movementW=centerNext-new_centerNext;

    if(movementW>0){

      _ve_envelope_shift(b->ve,movementW);
      v->pcm_current-=movementW;
      
      for(i=0;i<vi->channels;i++)
	memmove(v->pcm[i],v->pcm[i]+movementW,
		v->pcm_current*sizeof(*v->pcm[i]));
      
      
      v->lW=v->W;
      v->W=v->nW;
      v->centerW=new_centerNext;
      
      if(v->eofflag){
	v->eofflag-=movementW;
	if(v->eofflag<=0)v->eofflag=-1;
	/* do not add padding to end of stream! */
	if(v->centerW>=v->eofflag){
	  v->granulepos+=movementW-(v->centerW-v->eofflag);
	}else{
	  v->granulepos+=movementW;
	}
      }else{
	v->granulepos+=movementW;
      }
    }
  }

  /* done */
  return(1);
}
typedef char dsp_size_check[sizeof(vorbis_dsp_state)==112?1:-1];

extern "C" void __cdecl abort();
extern "C" int __cdecl memcmp(const void*,const void*,unsigned int);
static unsigned caseNumber;static void check(bool p){if(!p){printf("case %u failed\n",caseNumber);abort();}}
static vorbis_info info;static codec_setup_info setup;static vorbis_dsp_state state;static private_state backend;static vorbis_block block;static vorbis_block_internal internal;static vorbis_look_psy_global global;
static float data[2][136],before[2][136],expectedData[2][136];static float*channels[2];static unsigned char envelopeStorage[32];
static union {double align;unsigned char bytes[16384];} arena;
static unsigned used,allocCalls,searchCalls,markCalls,ripcordCalls,decayCalls,shiftCalls;static long allocations[8],shifted;static int searchResult,markResult;static float expectedLevel;
extern "C" long __fastcall _ve_envelope_search(vorbis_dsp_state*p){check(p==&state);++searchCalls;return searchResult;}
extern "C" int __fastcall _ve_envelope_mark(vorbis_dsp_state*p){check(p==&state);++markCalls;return markResult;}
extern "C" void __fastcall _vorbis_block_ripcord(vorbis_block*p){check(p==&block);++ripcordCalls;p->localtop=0;}
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block*p,long bytes){check(p==&block&&bytes>=0&&allocCalls<8&&used+(unsigned)bytes+8<sizeof arena.bytes);allocations[allocCalls++]=bytes;void*result=arena.bytes+used;used=((used+(unsigned)bytes)+7)&~7U;p->localtop=used;return result;}
extern "C" float __fastcall _vp_ampmax_decay(float level,vorbis_dsp_state*p){check(p==&state&&level==expectedLevel);++decayCalls;return level*0.5f;}
extern "C" void __fastcall _ve_envelope_shift(envelope_lookup*p,long movement){check((void*)p==(void*)envelopeStorage);++shiftCalls;shifted=movement;}
static void run(int small,int large,int current,int previous,int centerMode,int search,int pre,int eofMode,int enough,int count){++caseNumber;memset(&info,0,sizeof info);memset(&setup,0,sizeof setup);memset(&state,0,sizeof state);memset(&backend,0,sizeof backend);memset(&block,0,sizeof block);memset(&internal,0,sizeof internal);memset(&global,0,sizeof global);memset(arena.bytes,0xA6,sizeof arena.bytes);used=allocCalls=searchCalls=markCalls=ripcordCalls=decayCalls=shiftCalls=0;shifted=0;
 setup.blocksizes[0]=small;setup.blocksizes[1]=large;info.channels=count;info.codec_setup=&setup;state.vi=&info;state.backend_state=&backend;state.pcm=channels;backend.psy_g_look=&global;backend.ve=(envelope_lookup*)envelopeStorage;block.internal=&internal;state.W=current;state.lW=previous;state.nW=1;state.centerW=centerMode?large/2:(current?large:small)/2;state.preextrapolate=pre;state.sequence=((ogg_int64_t)1<<33)-1;state.granulepos=1000;state.pcm_returned=-1;
 int nw=(search<0||small==large)?0:search;long centerNext=state.centerW+(current?large:small)/4+(nw?large:small)/4;long bound=centerNext+(nw?large:small)/2;state.pcm_current=enough?bound+8:bound-1;
 state.eofflag=eofMode==0?0:(eofMode==1?-1:(eofMode==2?state.centerW:(eofMode==3?state.centerW+1:state.pcm_current)));searchResult=search;markResult=caseNumber&1;global.ampmax=0.5f;internal.ampmax=caseNumber&1?1.0f:-1.0f;internal.blocktype=99;expectedLevel=internal.ampmax>global.ampmax?internal.ampmax:global.ampmax;
 for(int c=0;c<2;++c){channels[c]=data[c]+4;for(int i=0;i<136;++i)data[c][i]=(float)(c*1000+i)*0.25f;}memcpy(before,data,sizeof data);memcpy(expectedData,data,sizeof data);vorbis_dsp_state expected=state;vorbis_block originalBlock=block;vorbis_block_internal originalInternal=internal;bool entered=pre&&state.eofflag!=-1;bool produced=entered&&!(search<0&&state.eofflag==0)&&enough;long movement=0;long begin=state.centerW-(current?large:small)/2;long blockLength=current?large:small;long oldCenter=state.centerW;ogg_int64_t sequence=state.sequence,granule=state.granulepos;
 if(entered&&!(search<0&&state.eofflag==0))expected.nW=nw;
 bool terminal=produced&&state.eofflag&&oldCenter>=state.eofflag;
 if(produced){++expected.sequence;if(terminal){expected.eofflag=-1;}else{movement=centerNext-large/2;if(movement>0){expected.pcm_current-=movement;for(int c=0;c<count;++c)for(int i=0;i<expected.pcm_current;++i)expectedData[c][4+i]=before[c][4+i+movement];expected.lW=current;expected.W=nw;expected.centerW=large/2;if(expected.eofflag){expected.eofflag-=movement;if(expected.eofflag<=0)expected.eofflag=-1;if(expected.centerW>=expected.eofflag)expected.granulepos+=movement-(expected.centerW-expected.eofflag);else expected.granulepos+=movement;}else expected.granulepos+=movement;}}}
 int result=vorbis_analysis_blockout(&state,&block);check(result==(produced?1:0));check(searchCalls==(entered?1U:0U));check(memcmp(&state,&expected,sizeof state)==0&&memcmp(data,expectedData,sizeof data)==0);
 if(!produced){check(allocCalls==0&&ripcordCalls==0&&decayCalls==0&&shiftCalls==0&&markCalls==0);check(memcmp(&block,&originalBlock,sizeof block)==0&&memcmp(&internal,&originalInternal,sizeof internal)==0&&global.ampmax==0.5f);return;}
 check(ripcordCalls==1&&decayCalls==1&&allocCalls==(unsigned)(count+2)&&markCalls==(current?0U:1U));check(global.ampmax==expectedLevel*0.5f&&internal.ampmax==global.ampmax);int blocktype=current?((previous&&nw)?BLOCKTYPE_LONG:BLOCKTYPE_TRANSITION):(markResult?BLOCKTYPE_IMPULSE:BLOCKTYPE_PADDING);check(internal.blocktype==blocktype);check(block.lW==previous&&block.W==current&&block.nW==nw&&block.vd==&state&&block.sequence==sequence&&block.granulepos==granule&&block.pcmend==blockLength);check(block.eofflag==(terminal?1:0));check(allocations[0]==count*4&&allocations[1]==count*4);for(int c=0;c<count;++c){check(allocations[c+2]==(blockLength+begin)*4);check(block.pcm[c]==internal.pcmdelay[c]+begin);for(int i=0;i<blockLength+begin;++i)check(internal.pcmdelay[c][i]==before[c][4+i]);}check(shiftCalls==(!terminal&&movement>0?1U:0U));if(shiftCalls)check(shifted==movement);}
int main(){check(sizeof(vorbis_dsp_state)==112&&offsetof(codec_setup_info,halfrate_flag)==0xe78);const int pairs[][2]={{8,16},{8,32},{16,16}};for(int b=0;b<3;++b)for(int current=0;current<2;++current)for(int previous=0;previous<2;++previous)for(int center=0;center<2;++center)for(int search=-1;search<=1;++search)for(int pre=0;pre<2;++pre)for(int eof=0;eof<5;++eof)for(int enough=0;enough<2;++enough)for(int count=1;count<=2;++count)run(pairs[b][0],pairs[b][1],current,previous,center,search,pre,eof,enough,count);check(caseNumber==2880);puts("00c8fba0_TEST PASS");return 0;}