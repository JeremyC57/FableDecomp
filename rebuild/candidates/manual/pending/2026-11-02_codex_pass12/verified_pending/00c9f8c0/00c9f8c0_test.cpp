#include <stdio.h>
#include <assert.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
// mapping0_inverse(vorbis_block*,vorbis_info_mapping*) -- Decode residue, reverse channel coupling, apply floor and transform channels.
// Leaf: @mapping0_inverse@8
// Flags: /TC /Gr
#ifndef FABLE_VORBIS_101_H
#define FABLE_VORBIS_101_H
// Original Xiph Vorbis 1.0.1 public and internal structures, preserving complete layouts.
// The matched mapping0_unpack neighbor provides the same full structure definitions.
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
#include <stdint.h>
typedef int64_t ogg_int64_t;
#endif
typedef unsigned int ogg_uint32_t;
#define DATA_TYPE float
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
  int endbyte;
  int  endbit;

  unsigned char *buffer;
  unsigned char *ptr;
  int storage;
} oggpack_buffer;
typedef struct {
  unsigned char *packet;
  int  bytes;
  int  b_o_s;
  int  e_o_s;

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
  int rate;

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

  int bitrate_upper;
  int bitrate_nominal;
  int bitrate_lower;
  int bitrate_window;

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

  int lW;
  int W;
  int nW;
  int centerW;

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
  int  *octave;             /* in n.ocshift format */
  int  *bark;

  int  firstoc;
  int  shiftoc;
  int   eighth_octave_lines; /* power of two, please */
  int   total_octave_lines;  
  int  rate; /* cache it */
} vorbis_look_psy;
typedef struct bitrate_manager_state {
  ogg_uint32_t  *queue_binned;
  ogg_uint32_t  *queue_actual;
  int            queue_size;

  int            queue_head;
  int            queue_bins;

  int          *avg_binacc;
  int            avg_center;
  int            avg_tail;
  ogg_uint32_t   avg_centeracc;
  ogg_uint32_t   avg_sampleacc;
  ogg_uint32_t   avg_sampledesired;
  ogg_uint32_t   avg_centerdesired;

  int          *minmax_binstack;
  int          *minmax_posstack;
  int          *minmax_limitstack;
  int           minmax_stackptr;

  int           minmax_acctotal;
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
  int   dim;            /* codebook dimensions (elements per vector) */
  int   entries;        /* codebook entries */
  int  *lengthlist;     /* codeword lengths in bits */

  /* mapping ***************************************************************/
  int    maptype;        /* 0=none
			    1=implicitly populated values from map column 
			    2=listed arbitrary values */

  /* The below does a linear, single monotonic sequence mapping. */
  int     q_min;       /* packed 32 bit float; quant value 0 maps to minval */
  int     q_delta;     /* packed 32 bit float; val 1 - val 0 == delta */
  int      q_quant;     /* bits: 0 < quant <= 16 */
  int      q_sequencep; /* bitflag */

  int     *quantlist;  /* map == 1: (int)(entries^(1/dim)) element column map
			   map == 2: list of dim*entries quantized entry vals
			*/

  /* encode helpers ********************************************************/
  struct encode_aux_nearestmatch *nearest_tree;
  struct encode_aux_threshmatch  *thresh_tree;
  struct encode_aux_pigeonhole  *pigeon_tree;

  int allocedp;
} static_codebook;
typedef struct codebook{
  int dim;           /* codebook dimensions (elements per vector) */
  int entries;       /* codebook entries */
  int used_entries;  /* populated codebook entries */
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

  /* for block int/short tuning; encode only */
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
  int   bitrate_min;
  int   bitrate_av_lo;
  int   bitrate_av_hi;
  int   bitrate_max;
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
  
  highlevel_byblocktype block[4]; /* padding, impulse, transition, int */

} highlevel_encode_setup;
typedef struct codec_setup_info {

  /* Vorbis supports only short and int blocks, but allows the
     encoder to choose the sizes */

  int blocksizes[2];

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
  int **(__fastcall *classification)      (struct vorbis_block *,vorbis_look_residue *,
			float **,int *,int);
  int  (__fastcall *forward)      (struct vorbis_block *,vorbis_look_residue *,
			float **,float **,int *,int,int **);
  int  (__fastcall *inverse)      (struct vorbis_block *,vorbis_look_residue *,
			float **,int *,int);
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
  
  int  lW;
  int  W;
  int  nW;
  int   pcmend;
  int   mode;

  int         eofflag;
  ogg_int64_t granulepos;
  ogg_int64_t sequence;
  vorbis_dsp_state *vd; /* For read-only access of configuration */

  /* local storage to avoid remallocing; it's up to the mapping to
     structure it */
  void               *localstore;
  int                localtop;
  int                localalloc;
  int                totaluse;
  struct alloc_chain *reap;

  /* bitmetrics for the frame */
  int glue_bits;
  int time_bits;
  int floor_bits;
  int res_bits;

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

#define VIF_POSIT 63
#define VIF_CLASS 16
#define VIF_PARTS 31
typedef struct{
  int   partitions;                /* 0 to 31 */
  int   partitionclass[VIF_PARTS]; /* 0 to 15 */

  int   class_dim[VIF_CLASS];        /* 1 to 8 */
  int   class_subs[VIF_CLASS];       /* 0,1,2,3 (bits: 1<<n poss) */
  int   class_book[VIF_CLASS];       /* subs ^ dim entries */
  int   class_subbook[VIF_CLASS][8]; /* [VIF_CLASS][subs] */


  int   mult;                      /* 1 2 3 or 4 */ 
  int   postlist[VIF_POSIT+2];    /* first two implicit */ 


  /* encode side analysis parameters */
  float maxover;     
  float maxunder;  
  float maxerr;    

  float twofitweight;  
  float twofitatten;

  int   n;

} vorbis_info_floor1;


typedef struct {
  int sorted_index[VIF_POSIT+2];
  int forward_index[VIF_POSIT+2];
  int reverse_index[VIF_POSIT+2];
  
  int hineighbor[VIF_POSIT];
  int loneighbor[VIF_POSIT];
  int posts;

  int n;
  int quant_q;
  vorbis_info_floor1 *vi;

  int phrasebits;
  int postbits;
  int frames;
} vorbis_look_floor1;


#endif

#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#endif
#include <string.h>
#ifdef _MSC_VER
#define alloca _alloca
#endif
extern const vorbis_func_floor* _floor_P[];
extern const vorbis_func_residue* _residue_P[];
void __fastcall mdct_backward(mdct_lookup*,float*,float*);
int mapping0_inverse(vorbis_block *vb,vorbis_info_mapping *l){
  vorbis_dsp_state     *vd=vb->vd;
  vorbis_info          *vi=vd->vi;
  codec_setup_info     *ci=(codec_setup_info*)vi->codec_setup;
  private_state        *b=(private_state*)vd->backend_state;
  vorbis_info_mapping0 *info=(vorbis_info_mapping0 *)l;
  int hs=ci->halfrate_flag; 

  int                   i,j;
  int                  n=vb->pcmend=ci->blocksizes[vb->W];

  float **pcmbundle=(float**)alloca(sizeof(*pcmbundle)*vi->channels);
  int    *zerobundle=(int*)alloca(sizeof(*zerobundle)*vi->channels);

  int   *nonzero  =(int*)alloca(sizeof(*nonzero)*vi->channels);
  void **floormemo=(void**)alloca(sizeof(*floormemo)*vi->channels);
  
  /* recover the spectral envelope; store it in the PCM vector for now */
  for(i=0;i<vi->channels;i++){
    int submap=info->chmuxlist[i];
    floormemo[i]=_floor_P[ci->floor_type[info->floorsubmap[submap]]]->
      inverse1(vb,b->flr[info->floorsubmap[submap]]);
    if(floormemo[i])
      nonzero[i]=1;
    else
      nonzero[i]=0;      
    memset(vb->pcm[i],0,sizeof(*vb->pcm[i])*n/2);
  }

  /* channel coupling can 'dirty' the nonzero listing */
  for(i=0;i<info->coupling_steps;i++){
    if(nonzero[info->coupling_mag[i]] ||
       nonzero[info->coupling_ang[i]]){
      nonzero[info->coupling_mag[i]]=1; 
      nonzero[info->coupling_ang[i]]=1; 
    }
  }

  /* recover the residue into our working vectors */
  for(i=0;i<info->submaps;i++){
    int ch_in_bundle=0;
    for(j=0;j<vi->channels;j++){
      if(info->chmuxlist[j]==i){
	if(nonzero[j])
	  zerobundle[ch_in_bundle]=1;
	else
	  zerobundle[ch_in_bundle]=0;
	pcmbundle[ch_in_bundle++]=vb->pcm[j];
      }
    }

    _residue_P[ci->residue_type[info->residuesubmap[i]]]->
      inverse(vb,b->residue[info->residuesubmap[i]],
	      pcmbundle,zerobundle,ch_in_bundle);
  }

  /* channel coupling */
  for(i=info->coupling_steps-1;i>=0;i--){
    float *pcmM=vb->pcm[info->coupling_mag[i]];
    float *pcmA=vb->pcm[info->coupling_ang[i]];

    for(j=0;j<n/2;j++){
      float mag=pcmM[j];
      float ang=pcmA[j];

      if(mag>0)
	if(ang>0){
	  pcmM[j]=mag;
	  pcmA[j]=mag-ang;
	}else{
	  pcmA[j]=mag;
	  pcmM[j]=mag+ang;
	}
      else
	if(ang>0){
	  pcmM[j]=mag;
	  pcmA[j]=mag+ang;
	}else{
	  pcmA[j]=mag;
	  pcmM[j]=mag-ang;
	}
    }
  }

  /* compute and apply spectral envelope */
  for(i=0;i<vi->channels;i++){
    float *pcm=vb->pcm[i];
    int submap=info->chmuxlist[i];
    _floor_P[ci->floor_type[info->floorsubmap[submap]]]->
      inverse2(vb,b->flr[info->floorsubmap[submap]],
	       floormemo[i],pcm);
  }

  /* transform the PCM data; takes PCM vector, vb; modifies PCM vector */
  /* only MDCT right now.... */
  for(i=0;i<vi->channels;i++){
    float *pcm=vb->pcm[i];
    mdct_backward((mdct_lookup*)b->transform[vb->W][0],pcm,pcm);
  }

  /* all done! */
  return(0);
}

static int active_channels,active_mask,phase,channel_index,transform_calls,active_groups,applied_floors;static int propagated[4],floorids[4];static float* active_pcm[4];static float seeded[4][16];static mdct_lookup transform_object;
static void* __fastcall read_floor(vorbis_block*,vorbis_look_floor* marker) {assert(phase==0);int index=channel_index++;assert(*(int*)marker==index%active_groups);return active_mask&(1<<index)?&floorids[index]:0;}
static int __fastcall residue(vorbis_block*,vorbis_look_residue*,float** pcm,int* nonzero,int channels) {
 assert(channel_index==active_channels);phase=1;
 for(int i=0;i<channels;++i){int index=0;while(index<active_channels&&active_pcm[index]!=pcm[i])++index;assert(index<active_channels);assert(nonzero[i]==propagated[index]);for(int j=0;j<8;++j){assert(pcm[i][j]==0);pcm[i][j]=seeded[index][j];}}
 return 0;
}
static int __fastcall apply_floor(vorbis_block*,vorbis_look_floor* marker,void* memo,float* pcm) {
 assert(phase==1||phase==2);phase=2;int index=applied_floors++;assert(*(int*)marker==index%active_groups);assert(pcm==active_pcm[index]);assert(memo==((active_mask&(1<<index))?&floorids[index]:0));for(int j=0;j<8;++j)pcm[j]=memo?pcm[j]*2:0;return memo!=0;
}
void __fastcall mdct_backward(mdct_lookup* lookup,float* in,float* out) {assert(phase==2||phase==3);phase=3;assert(lookup==&transform_object&&in==out);assert(in==active_pcm[transform_calls++]);for(int j=0;j<8;++j)out[j]*=0.5f;}
static vorbis_func_floor floor_backend={0,0,0,0,0,read_floor,apply_floor};
static vorbis_func_residue residue_backend={0,0,0,0,0,0,0,residue};
const vorbis_func_floor* _floor_P[]={&floor_backend};const vorbis_func_residue* _residue_P[]={&residue_backend};
int main() {
 vorbis_block block;vorbis_dsp_state dsp;vorbis_info vi;codec_setup_info setup;private_state state;vorbis_info_mapping0 info;
 memset(&block,0,sizeof(block));memset(&dsp,0,sizeof(dsp));memset(&vi,0,sizeof(vi));memset(&setup,0,sizeof(setup));memset(&state,0,sizeof(state));memset(&info,0,sizeof(info));
 vi.codec_setup=&setup;dsp.vi=&vi;dsp.backend_state=&state;block.vd=&dsp;block.pcm=active_pcm;setup.blocksizes[0]=16;
 void* floor_look[4];void* residue_look[2]={0,0};void* transform_look[1]={&transform_object};state.flr=floor_look;state.residue=residue_look;state.transform[0]=transform_look;
 float pcm[4][16],expected[4][16];
 for(int channels=1;channels<=4;++channels)for(int mask=0;mask<(1<<channels);++mask)for(int groups=1;groups<=2;++groups)for(int couple=0;couple<2;++couple) {
  vi.channels=active_channels=channels;active_mask=mask;active_groups=groups;info.submaps=groups;info.coupling_steps=couple?(channels>=3?2:channels==2?1:0):0;info.coupling_mag[0]=0;info.coupling_ang[0]=1;info.coupling_mag[1]=1;info.coupling_ang[1]=2;
  for(int i=0;i<groups;++i)info.floorsubmap[i]=i;
  for(int i=0;i<channels;++i){floorids[i]=i;floor_look[i]=&floorids[i];info.chmuxlist[i]=i%groups;active_pcm[i]=pcm[i];propagated[i]=(mask>>i)&1;for(int j=0;j<16;++j)pcm[i][j]=99;for(int j=0;j<8;++j)seeded[i][j]=expected[i][j]=(i+1)*((j&1)?-0.5f:0.25f);}
  for(int step=0;step<info.coupling_steps;++step){int a=info.coupling_mag[step],b=info.coupling_ang[step];if(propagated[a]||propagated[b])propagated[a]=propagated[b]=1;}
  for(int step=info.coupling_steps-1;step>=0;--step){int a=info.coupling_mag[step],b=info.coupling_ang[step];for(int j=0;j<8;++j){float magnitude=expected[a][j],angle=expected[b][j];if(angle>0){expected[b][j]=magnitude>0?magnitude-angle:magnitude+angle;}else{expected[b][j]=magnitude;expected[a][j]=magnitude>0?magnitude+angle:magnitude-angle;}}}
  phase=channel_index=transform_calls=applied_floors=0;assert(mapping0_inverse(&block,&info)==0);assert(block.pcmend==16&&transform_calls==channels);
  for(int i=0;i<channels;++i)for(int j=0;j<8;++j)assert(pcm[i][j]==((mask&(1<<i))?expected[i][j]:0));for(int i=0;i<channels;++i)assert(pcm[i][8]==99);
 }
 puts("00c9f8c0_TEST PASS");return 0;
}
