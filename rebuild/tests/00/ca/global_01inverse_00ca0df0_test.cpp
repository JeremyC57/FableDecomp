#include <stdio.h>
#include <string.h>
// 00ca0df0 _01inverse
// Leaf: __01inverse@20
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
typedef struct {
  vorbis_info_residue0 *info;
  
  int         parts;
  int         stages;
  codebook   *fullbooks;
  codebook   *phrasebook;
  codebook ***partbooks;

  int         partvals;
  int       **decodemap;

  long      postbits;
  long      phrasebits;
  long      frames;

#ifdef TRAIN_RES
  int        train_seq;
  long      *training_data[8][64];
  float      training_max[8][64];
  float      training_min[8][64];
  float     tmin;
  float     tmax;
#endif

} vorbis_look_residue0;

#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#define __stdcall
#endif
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block*,long);
extern "C" long __fastcall vorbis_book_decode(codebook*,oggpack_buffer*);
extern "C" int __stdcall _01inverse(vorbis_block *vb,vorbis_look_residue *vl,
		      float **in,int ch,
		      long (__fastcall *decodepart)(codebook *, float *, 
					 oggpack_buffer *,int)){

  long i,j,k,l,s;
  vorbis_look_residue0 *look=(vorbis_look_residue0 *)vl;
  vorbis_info_residue0 *info=look->info;

  /* move all this setup out later */
  int samples_per_partition=info->grouping;
  int partitions_per_word=look->phrasebook->dim;
  int n=info->end-info->begin;
  
  int partvals=n/samples_per_partition;
  int partwords=(partvals+partitions_per_word-1)/partitions_per_word;
  int ***partword=(int***)alloca(ch*sizeof(*partword));

  for(j=0;j<ch;j++)
    partword[j]=(int**)_vorbis_block_alloc(vb,partwords*sizeof(*partword[j]));

  for(s=0;s<look->stages;s++){

    /* each loop decodes on partition codeword containing 
       partitions_pre_word partitions */
    for(i=0,l=0;i<partvals;l++){
      if(s==0){
	/* fetch the partition word for each channel */
	for(j=0;j<ch;j++){
	  int temp=vorbis_book_decode(look->phrasebook,&vb->opb);
	  if(temp==-1)goto eopbreak;
	  partword[j][l]=look->decodemap[temp];
	  if(partword[j][l]==NULL)goto errout;
	}
      }
      
      /* now we decode residual values for the partitions */
      for(k=0;k<partitions_per_word && i<partvals;k++,i++)
	for(j=0;j<ch;j++){
	  long offset=info->begin+i*samples_per_partition;
	  if(info->secondstages[partword[j][l][k]]&(1<<s)){
	    codebook *stagebook=look->partbooks[partword[j][l][k]][s];
	    if(stagebook){
	      if(decodepart(stagebook,in[j]+offset,&vb->opb,
			    samples_per_partition)==-1)goto eopbreak;
	    }
	  }
	}
    } 
  }
  
 errout:
 eopbreak:
  return(0);
}

extern "C" void abort();
struct Event{int kind,channel,partition,entry;codebook*book;};
static Event events[256];static int eventCount,eventAt,failAt,channelsNow,wordsNow,groupNow,beginNow;
static unsigned cases,expectedRuns,allocated,used;static unsigned arena[100];static void*rows[3];static unsigned rowSizes[3];
static vorbis_block*blockNow;static codebook*phraseNow;static codebook*booksNow;static float**inputNow;
static void check(bool p){if(!p){fprintf(stderr,"residue inverse case %u event%d/%d failed\n",cases,eventAt,eventCount);abort();}}
static void addEvent(int kind,int channel,int part,int entry,codebook*book){check(eventCount<256);Event&e=events[eventCount++];e.kind=kind;e.channel=channel;e.partition=part;e.entry=entry;e.book=book;}
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block*b,long bytes){check(b==blockNow&&allocated<(unsigned)channelsNow&&bytes==wordsNow*4);unsigned begin=(used+3)&~1U;check(begin+bytes/4+2<100);void*p=arena+begin;rows[allocated]=p;rowSizes[allocated++]=bytes;used=begin+bytes/4+2;return p;}
extern "C" long __fastcall vorbis_book_decode(codebook*b,oggpack_buffer*p){check(eventAt<eventCount&&events[eventAt].kind==0&&b==phraseNow&&p==&blockNow->opb);int at=eventAt++;return at==failAt?-1:events[at].entry;}
static long __fastcall stageDecode(codebook*b,float*out,oggpack_buffer*p,int n){check(eventAt<eventCount&&events[eventAt].kind==1);Event&e=events[eventAt];check(b==e.book&&out==inputNow[e.channel]+beginNow+e.partition*groupNow&&p==&blockNow->opb&&n==groupNow);int at=eventAt++;if(at==failAt)return -1;float marker=(b-booksNow+1)*0.25f;for(int k=0;k<n;++k)out[k]+=marker;return 0;}
static void buildTrace(vorbis_look_residue0*look,int channels,int partitions,int dim,int seed){eventCount=0;int mapCount=1;for(int k=0;k<dim;++k)mapCount*=3;for(int stage=0;stage<look->stages;++stage)for(int part=0;part<partitions;++part){int word=part/dim;if(stage==0&&part%dim==0)for(int c=0;c<channels;++c){int entry=(word*channels+c+seed)%mapCount;addEvent(0,c,part,entry,0);if(!look->decodemap[entry])return;}for(int c=0;c<channels;++c){int entry=(word*channels+c+seed)%mapCount;int cls=look->decodemap[entry][part%dim];if((look->info->secondstages[cls]&(1<<stage))&&look->partbooks[cls][stage])addEvent(1,c,part,entry,look->partbooks[cls][stage]);}}}
static void run(vorbis_block*block,vorbis_look_residue0*look,int channels,int partitions,int dim,int grouping,int seed,int failure){float data[3][32],expected[3][32],*input[3];for(int c=0;c<3;++c){input[c]=data[c];for(int k=0;k<32;++k)data[c][k]=expected[c][k]=(c*5+k%7-seed)*0.125f;}blockNow=block;phraseNow=look->phrasebook;booksNow=look->fullbooks;inputNow=input;channelsNow=channels;wordsNow=(partitions+dim-1)/dim;groupNow=grouping;beginNow=look->info->begin;eventAt=0;failAt=failure;allocated=used=0;for(int i=0;i<100;++i)arena[i]=0x5eed0a11U;
int completed=failure<0?eventCount:failure;for(int i=0;i<completed;++i)if(events[i].kind){Event&e=events[i];float marker=(e.book-booksNow+1)*0.25f;for(int k=0;k<grouping;++k)expected[e.channel][beginNow+e.partition*grouping+k]+=marker;}
int result=_01inverse(block,look,input,channels,stageDecode);check(result==0&&eventAt==(failure<0?eventCount:failure+1)&&allocated==(unsigned)channels);check(memcmp(data,expected,sizeof data)==0);for(unsigned i=0;i<allocated;++i){unsigned*p=(unsigned*)rows[i];check(p[-1]==0x5eed0a11U&&p[rowSizes[i]/4]==0x5eed0a11U);}++cases;}
int main(){for(int channels=1;channels<=3;++channels)for(int dim=1;dim<=3;++dim)for(int count=0;count<=6;++count)for(int gi=0;gi<2;++gi)for(int stages=0;stages<=3;++stages)for(int seed=0;seed<3;++seed){vorbis_block block;vorbis_info_residue0 info;vorbis_look_residue0 look;codebook books[16];int digits[27][3],*map[27];codebook*stageBooks[3][3],**partbooks[3];memset(&block,0,sizeof block);memset(&info,0,sizeof info);memset(&look,0,sizeof look);memset(books,0,sizeof books);int grouping=gi?4:1;info.begin=seed%3;info.end=info.begin+count*grouping;info.grouping=grouping;look.info=&info;look.phrasebook=books;look.fullbooks=books;look.stages=stages;look.partbooks=partbooks;look.decodemap=map;books[0].dim=dim;int entries=1;for(int k=0;k<dim;++k)entries*=3;for(int entry=0;entry<entries;++entry){map[entry]=digits[entry];int value=entry;for(int k=dim-1;k>=0;--k){digits[entry][k]=value%3;value/=3;}}for(int cls=0;cls<3;++cls){partbooks[cls]=stageBooks[cls];info.secondstages[cls]=cls==0?(1<<stages)-1:(cls==1?5:2);for(int s=0;s<3;++s)stageBooks[cls][s]=(cls+s+seed)%4?books+1+cls*3+s:0;}
for(int nullmap=0;nullmap<2;++nullmap){map[0]=nullmap?0:digits[0];buildTrace(&look,channels,count,dim,seed);expectedRuns+=eventCount+1;for(int failure=-1;failure<eventCount;++failure)run(&block,&look,channels,count,dim,grouping,seed,failure);}}
check(cases==expectedRuns&&cases>10000);puts("00ca0df0_TEST PASS");return 0;}