#include <stdio.h>
#include <string.h>
// 00c8eda0 vorbis_synthesis
// Leaf: @vorbis_synthesis@8
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

#define OV_ENOTAUDIO -135
#define OV_EBADPACKET -136
extern "C" {
extern vorbis_func_mapping *_mapping_P[];
void __fastcall _vorbis_block_ripcord(vorbis_block*);
void* __fastcall _vorbis_block_alloc(vorbis_block*,long);
void __fastcall oggpack_readinit(oggpack_buffer*,unsigned char*,int);
long __fastcall oggpack_read(oggpack_buffer*,int);
}
extern "C" int __fastcall vorbis_synthesis(vorbis_block *vb,ogg_packet *op){
  vorbis_dsp_state     *vd=vb->vd;
  private_state        *b=(private_state*)vd->backend_state;
  vorbis_info          *vi=vd->vi;
  codec_setup_info     *ci=(codec_setup_info*)vi->codec_setup;
  oggpack_buffer       *opb=&vb->opb;
  int                   type,mode,i;
 
  /* first things first.  Make sure decode is ready */
  _vorbis_block_ripcord(vb);
  oggpack_readinit(opb,op->packet,op->bytes);

  /* Check the packet type */
  if(oggpack_read(opb,1)!=0){
    /* Oops.  This is not an audio data packet */
    return(OV_ENOTAUDIO);
  }

  /* read our mode and pre/post windowsize */
  mode=oggpack_read(opb,b->modebits);
  if(mode==-1)return(OV_EBADPACKET);
  
  vb->mode=mode;
  vb->W=ci->mode_param[mode]->blockflag;
  if(vb->W){

    /* this doesn;t get mapped through mode selection as it's used
       only for window selection */
    vb->lW=oggpack_read(opb,1);
    vb->nW=oggpack_read(opb,1);
    if(vb->nW==-1)   return(OV_EBADPACKET);
  }else{
    vb->lW=0;
    vb->nW=0;
  }
  
  /* more setup */
  vb->granulepos=op->granulepos;
  vb->sequence=op->packetno;
  vb->eofflag=op->e_o_s;

  /* alloc pcm passback storage */
  vb->pcmend=ci->blocksizes[vb->W];
  vb->pcm=(float**)_vorbis_block_alloc(vb,sizeof(*vb->pcm)*vi->channels);
  for(i=0;i<vi->channels;i++)
    vb->pcm[i]=(float*)_vorbis_block_alloc(vb,vb->pcmend*sizeof(*vb->pcm[i]));

  /* unpack_header enforces range checking */
  type=ci->map_type[ci->mode_param[mode]->mapping];

  return(_mapping_P[type]->inverse(vb,ci->map_param[ci->mode_param[mode]->
						   mapping]));
}

extern "C" void abort();
static unsigned cases,reads,ripcords,allocations,used,mappingCalls;
static long script[4];static int callbackResult,channelCount,blockSize;
static unsigned arena[200];static vorbis_block*currentBlock;static ogg_packet*currentPacket;static void*currentMapping;
static void check(bool p){if(!p){fprintf(stderr,"synthesis case %u failed\n",cases);abort();}}
extern "C" void __fastcall _vorbis_block_ripcord(vorbis_block*b){check(b==currentBlock&&ripcords==0);++ripcords;}
extern "C" void __fastcall oggpack_readinit(oggpack_buffer*b,unsigned char*data,int bytes){check(b==&currentBlock->opb&&data==currentPacket->packet&&bytes==currentPacket->bytes&&ripcords==1);memset(b,0,sizeof(*b));b->buffer=b->ptr=data;b->storage=bytes;}
extern "C" long __fastcall oggpack_read(oggpack_buffer*b,int count){check(b==&currentBlock->opb&&reads<4&&count==(reads==1?2:1));long value=script[reads++];b->endbit+=count;return value;}
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block*b,long bytes){check(b==currentBlock&&bytes==(allocations==0?channelCount*4:blockSize*4)&&used+bytes/4+2<200);unsigned*p=arena+used+1;used+=bytes/4+2;++allocations;return p;}
static int __fastcall inverse(vorbis_block*b,vorbis_info_mapping*m){check(b==currentBlock&&m==currentMapping);++mappingCalls;return callbackResult;}
static vorbis_func_mapping mappingRecord;
extern "C" {vorbis_func_mapping*_mapping_P[1]={&mappingRecord};}
int main(){
 for(int channels=0;channels<=4;++channels)for(int mode=-1;mode<4;++mode)for(int type=-1;type<=1;++type)for(int left=0;left<2;++left)for(int next=-1;next<=1;++next)for(int answer=-1;answer<=1;++answer){
 vorbis_info info;vorbis_dsp_state dsp;private_state state;codec_setup_info setup;vorbis_block block;ogg_packet packet;vorbis_info_mode modes[4];unsigned char payload[16],mapdata[4][4];memset(&info,0,sizeof info);memset(&dsp,0,sizeof dsp);memset(&state,0,sizeof state);memset(&setup,0,sizeof setup);memset(&block,0,sizeof block);memset(&packet,0,sizeof packet);memset(modes,0,sizeof modes);memset(payload,0,sizeof payload);memset(mapdata,0,sizeof mapdata);memset(&mappingRecord,0,sizeof mappingRecord);mappingRecord.inverse=inverse;
 info.channels=channels;info.codec_setup=&setup;dsp.vi=&info;dsp.backend_state=&state;state.modebits=2;setup.blocksizes[0]=8;setup.blocksizes[1]=16;for(int i=0;i<4;++i){modes[i].blockflag=i%2;modes[i].mapping=3-i;setup.mode_param[i]=modes+i;setup.map_type[i]=0;setup.map_param[i]=mapdata[i];}
 block.vd=&dsp;block.mode=17;block.lW=19;block.W=23;block.nW=29;block.eofflag=31;block.pcmend=37;block.granulepos=41;block.sequence=43;packet.packet=payload;packet.bytes=16;packet.e_o_s=answer!=0;packet.granulepos=12345;packet.packetno=23456;
 vorbis_block expected=block;currentBlock=&block;currentPacket=&packet;currentMapping=mode<0?0:mapdata[3-mode];callbackResult=answer*17;channelCount=channels;blockSize=mode<0?0:setup.blocksizes[mode%2];script[0]=type;script[1]=mode;script[2]=left;script[3]=next;reads=ripcords=allocations=used=mappingCalls=0;for(int i=0;i<200;++i)arena[i]=0x648abcefU;
 int result=vorbis_synthesis(&block,&packet);unsigned expectedReads=1;int error=-135;bool success=false;if(type==0){expectedReads=2;error=-136;if(mode>=0){expected.mode=mode;expected.W=mode%2;if(expected.W){expected.lW=left;expected.nW=next;expectedReads=4;success=next!=-1;}else{expected.lW=expected.nW=0;success=true;}}}
 check(ripcords==1&&reads==expectedReads);expected.opb=block.opb;check(block.opb.buffer==payload&&block.opb.ptr==payload&&block.opb.storage==16&&block.opb.endbyte==0&&block.opb.endbit==(int)(expectedReads==1?1:expectedReads==2?3:5));
 if(success){check(result==callbackResult&&allocations==(unsigned)(channels+1)&&mappingCalls==1);expected.granulepos=packet.granulepos;expected.sequence=packet.packetno;expected.eofflag=packet.e_o_s;expected.pcmend=blockSize;expected.pcm=(float**)(arena+1);unsigned cursor=channels+2;for(int i=0;i<channels;++i){check(block.pcm[i]==(float*)(arena+cursor+1));cursor+=blockSize+2;}check(used==cursor);}else check(result==error&&allocations==0&&mappingCalls==0);
 check(memcmp(&block,&expected,sizeof block)==0&&arena[0]==0x648abcefU&&arena[used]==0x648abcefU);++cases;
 }check(cases==1350);puts("00c8eda0_TEST PASS");return 0;}