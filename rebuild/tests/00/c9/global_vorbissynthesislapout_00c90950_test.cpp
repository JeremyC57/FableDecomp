#include <stdio.h>
#include <string.h>
// 00c90950 vorbis_synthesis_lapout
// Leaf: @vorbis_synthesis_lapout@8
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
typedef struct vorbis_look_psy_global vorbis_look_psy_global;
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

extern "C" int __fastcall vorbis_synthesis_lapout(vorbis_dsp_state *v,float ***pcm){
  vorbis_info *vi=v->vi;
  codec_setup_info *ci=(codec_setup_info*)vi->codec_setup;
  int hs=ci->halfrate_flag; 
  
  int n=ci->blocksizes[v->W]>>(hs+1);
  int n0=ci->blocksizes[0]>>(hs+1);
  int n1=ci->blocksizes[1]>>(hs+1);
  int i,j;

  if(v->pcm_returned<0)return 0;

  /* our returned data ends at pcm_returned; because the synthesis pcm
     buffer is a two-fragment ring, that means our data block may be
     fragmented by buffering, wrapping or a short block not filling
     out a buffer.  To simplify things, we unfragment if it's at all
     possibly needed. Otherwise, we'd need to call lapout more than
     once as well as hold additional dsp state.  Opt for
     simplicity. */

  /* centerW was advanced by blockin; it would be the center of the
     *next* block */
  if(v->centerW==n1){
    /* the data buffer wraps; swap the halves */
    /* slow, sure, small */
    for(j=0;j<vi->channels;j++){
      float *p=v->pcm[j];
      for(i=0;i<n1;i++){
	float temp=p[i];
	p[i]=p[i+n1];
	p[i+n1]=temp;
      }
    }

    v->pcm_current-=n1;
    v->pcm_returned-=n1;
    v->centerW=0;
  }
  
  /* solidify buffer into contiguous space */
  if((v->lW^v->W)==1){
    /* long/short or short/long */
    for(j=0;j<vi->channels;j++){
      float *s=v->pcm[j];
      float *d=v->pcm[j]+(n1-n0)/2;
      for(i=(n1+n0)/2-1;i>=0;--i)
	d[i]=s[i];
    }
    v->pcm_returned+=(n1-n0)/2;
    v->pcm_current+=(n1-n0)/2;
  }else{
    if(v->lW==0){
      /* short/short */
      for(j=0;j<vi->channels;j++){
	float *s=v->pcm[j];
	float *d=v->pcm[j]+n1-n0;
	for(i=n0-1;i>=0;--i)
	  d[i]=s[i];
      }
      v->pcm_returned+=n1-n0;
      v->pcm_current+=n1-n0;
    }
  }
    
  if(pcm){
    int i;
    for(i=0;i<vi->channels;i++)
      v->pcmret[i]=v->pcm[i]+v->pcm_returned;
    *pcm=v->pcmret;
  }

  return(n1+n-v->pcm_returned);

}
typedef char halfrate_offset_check[offsetof(codec_setup_info,halfrate_flag)==0xe78?1:-1];
typedef char dsp_size_check[sizeof(vorbis_dsp_state)==112?1:-1];

extern "C" void __cdecl abort();
static void check(bool p){if(!p)abort();}
static float samples[3][104],expectedSamples[3][104],afterSwap[3][104];
static float*pcmSlots[3],*retSlots[3];
static unsigned count;
static void run(int small,int large,int hs,int previous,int current,int wrap,int channels,int offset,int output){vorbis_info info;codec_setup_info setup;vorbis_dsp_state state;memset(&info,0,sizeof info);memset(&setup,0,sizeof setup);memset(&state,0,sizeof state);info.codec_setup=&setup;info.channels=channels;setup.blocksizes[0]=small;setup.blocksizes[1]=large;setup.halfrate_flag=hs;int n0=small>>(hs+1),n1=large>>(hs+1),n=(current?large:small)>>(hs+1);for(int c=0;c<3;++c){for(int i=0;i<104;++i)samples[c][i]=(float)(c*1000+i)*0.5f;pcmSlots[c]=samples[c]+4;retSlots[c]=0;}memcpy(expectedSamples,samples,sizeof samples);state.vi=&info;state.pcm=pcmSlots;state.pcmret=retSlots;state.lW=previous;state.W=current;state.centerW=wrap?n1:0;state.pcm_returned=offset<0?-1:(wrap?n1:0)+offset;state.pcm_current=(wrap?n1:0)+n0;state.analysisp=67;state.pcm_storage=96;state.sequence=123;vorbis_dsp_state expected=state;float**returned=pcmSlots;float***out=output?&returned:0;int expectedResult;
 if(offset<0){expectedResult=0;}else{if(wrap){for(int c=0;c<channels;++c)for(int i=0;i<n1;++i){float tmp=expectedSamples[c][4+i];expectedSamples[c][4+i]=expectedSamples[c][4+i+n1];expectedSamples[c][4+i+n1]=tmp;}expected.pcm_current-=n1;expected.pcm_returned-=n1;expected.centerW=0;}memcpy(afterSwap,expectedSamples,sizeof afterSwap);int shift=0,extent=0;if((previous^current)==1){shift=(n1-n0)/2;extent=(n1+n0)/2;}else if(previous==0){shift=n1-n0;extent=n0;}for(int c=0;c<channels;++c)for(int i=0;i<extent;++i)expectedSamples[c][4+shift+i]=afterSwap[c][4+i];expected.pcm_current+=shift;expected.pcm_returned+=shift;expectedResult=n1+n-expected.pcm_returned;}
 int result=vorbis_synthesis_lapout(&state,out);check(result==expectedResult&&memcmp(&state,&expected,sizeof state)==0&&memcmp(samples,expectedSamples,sizeof samples)==0);if(output&&offset>=0){check(returned==retSlots);for(int c=0;c<channels;++c)check(retSlots[c]==pcmSlots[c]+expected.pcm_returned);for(int c=channels;c<3;++c)check(retSlots[c]==0);}else{check(returned==pcmSlots);for(int c=0;c<3;++c)check(retSlots[c]==0);}check(info.channels==channels&&info.codec_setup==&setup&&setup.blocksizes[0]==small&&setup.blocksizes[1]==large&&setup.halfrate_flag==hs);++count;}
int main(){check(sizeof(vorbis_dsp_state)==112&&offsetof(codec_setup_info,halfrate_flag)==0xe78);const int blocks[][2]={{8,16},{16,32},{16,64}};for(int b=0;b<3;++b)for(int hs=0;hs<2;++hs)for(int previous=0;previous<2;++previous)for(int current=0;current<2;++current)for(int wrap=0;wrap<2;++wrap)for(int channels=0;channels<=3;++channels)for(int offset=-1;offset<=2;++offset)for(int output=0;output<2;++output)run(blocks[b][0],blocks[b][1],hs,previous,current,wrap,channels,offset,output);check(count==1536);puts("00c90950_TEST PASS");return 0;}