#include <stdio.h>
#include <assert.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
// floor1_encode(vorbis_block*,vorbis_look_floor1*,int*,int*) -- Encode quantized floor posts and render their integer curve.
// Leaf: @floor1_encode@16
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

#include <string.h>
#include <stdlib.h>
void __fastcall oggpack_write(oggpack_buffer*,unsigned int,int);
int __fastcall vorbis_book_encode(codebook*,int,oggpack_buffer*);
// Fidelity: Original C file-local helpers retain their source names and local linkage.
static __declspec(noinline) int ilog(unsigned int v){
  int ret=0;
  while(v){
    ret++;
    v>>=1;
  }
  return(ret);
}
static __declspec(noinline) int render_point(int x0,int x1,int y0,int y1,int x){
  y0&=0x7fff; /* mask off flag */
  y1&=0x7fff;
    
  {
    int dy=y1-y0;
    int adx=x1-x0;
    int ady=abs(dy);
    int err=ady*(x-x0);
    
    int off=err/adx;
    if(dy<0)return(y0-off);
    return(y0+off);
  }
}
static void render_line0(int x0,int x1,int y0,int y1,int *d){
  int dy=y1-y0;
  int adx=x1-x0;
  int ady=abs(dy);
  int base=dy/adx;
  int sy=(dy<0?base-1:base+1);
  int x=x0;
  int y=y0;
  int err=0;

  ady-=abs(base*adx);

  d[x]=y;
  while(++x<x1){
    err=err+ady;
    if(err>=adx){
      err-=adx;
      y+=sy;
    }else{
      y+=base;
    }
    d[x]=y;
  }
}
int floor1_encode(vorbis_block *vb,vorbis_look_floor1 *look,
		  int *post,int *ilogmask){

  int i,j;
  vorbis_info_floor1 *info=look->vi;
  int n=look->n;
  int posts=look->posts;
  codec_setup_info *ci=(codec_setup_info*)vb->vd->vi->codec_setup;
  int out[VIF_POSIT+2];
  static_codebook **sbooks=ci->book_param;
  codebook *books=ci->fullbooks;
  static int seq=0; 

  /* quantize values to multiplier spec */
  if(post){
    for(i=0;i<posts;i++){
      int val=post[i]&0x7fff;
      switch(info->mult){
      case 1: /* 1024 -> 256 */
	val>>=2;
	break;
      case 2: /* 1024 -> 128 */
	val>>=3;
	break;
      case 3: /* 1024 -> 86 */
	val/=12;
	break;
      case 4: /* 1024 -> 64 */
	val>>=4;
	break;
      }
      post[i]=val | (post[i]&0x8000);
    }

    out[0]=post[0];
    out[1]=post[1];

    /* find prediction values for each post and subtract them */
    for(i=2;i<posts;i++){
      int ln=look->loneighbor[i-2];
      int hn=look->hineighbor[i-2];
      int x0=info->postlist[ln];
      int x1=info->postlist[hn];
      int y0=post[ln];
      int y1=post[hn];
      
      int predicted=render_point(x0,x1,y0,y1,info->postlist[i]);
      
      if((post[i]&0x8000) || (predicted==post[i])){
	post[i]=predicted|0x8000; /* in case there was roundoff jitter
				     in interpolation */
	out[i]=0;
      }else{
	int headroom=(look->quant_q-predicted<predicted?
		      look->quant_q-predicted:predicted);
	
	int val=post[i]-predicted;
	
	/* at this point the 'deviation' value is in the range +/- max
	   range, but the real, unique range can always be mapped to
	   only [0-maxrange).  So we want to wrap the deviation into
	   this limited range, but do it in the way that least screws
	   an essentially gaussian probability distribution. */
	
	if(val<0)
	  if(val<-headroom)
	    val=headroom-val-1;
	  else
	    val=-1-(val<<1);
	else
	  if(val>=headroom)
	    val= val+headroom;
	  else
	    val<<=1;
	
	out[i]=val;
	post[ln]&=0x7fff;
	post[hn]&=0x7fff;
      }
    }
    
    /* we have everything we need. pack it out */
    /* mark nontrivial floor */
    oggpack_write(&vb->opb,1,1);
      
    /* beginning/end post */
    look->frames++;
    look->postbits+=ilog(look->quant_q-1)*2;
    oggpack_write(&vb->opb,out[0],ilog(look->quant_q-1));
    oggpack_write(&vb->opb,out[1],ilog(look->quant_q-1));
      
      
    /* partition by partition */
    for(i=0,j=2;i<info->partitions;i++){
      int classification=info->partitionclass[i];
      int cdim=info->class_dim[classification];
      int csubbits=info->class_subs[classification];
      int csub=1<<csubbits;
      int bookas[8]={0,0,0,0,0,0,0,0};
      int cval=0;
      int cshift=0;
      int k,l;

      /* generate the partition's first stage cascade value */
      if(csubbits){
	int maxval[8];
	for(k=0;k<csub;k++){
	  int booknum=info->class_subbook[classification][k];
	  if(booknum<0){
	    maxval[k]=1;
	  }else{
	    maxval[k]=sbooks[info->class_subbook[classification][k]]->entries;
	  }
	}
	for(k=0;k<cdim;k++){
	  for(l=0;l<csub;l++){
	    int val=out[j+k];
	    if(val<maxval[l]){
	      bookas[k]=l;
	      break;
	    }
	  }
	  cval|= bookas[k]<<cshift;
	  cshift+=csubbits;
	}
	/* write it */
	look->phrasebits+=
	  vorbis_book_encode(books+info->class_book[classification],cval,&vb->opb);
	

      }
	
      /* write post values */
      for(k=0;k<cdim;k++){
	int book=info->class_subbook[classification][bookas[k]];
	if(book>=0){
	  /* hack to allow training with 'bad' books */
	  if(out[j+k]<(books+book)->entries)
	    look->postbits+=vorbis_book_encode(books+book,
					       out[j+k],&vb->opb);
	  /*else
	    fprintf(stderr,"+!");*/
	  

	}
      }
      j+=cdim;
    }
    
    {
      /* generate quantized floor equivalent to what we'd unpack in decode */
      /* render the lines */
      int hx=0;
      int lx=0;
      int ly=post[0]*info->mult;
      for(j=1;j<look->posts;j++){
	int current=look->forward_index[j];
	int hy=post[current]&0x7fff;
	if(hy==post[current]){
	  
	  hy*=info->mult;
	  hx=info->postlist[current];
	
	  render_line0(lx,hx,ly,hy,ilogmask);
	
	  lx=hx;
	  ly=hy;
	}
      }
      for(j=hx;j<vb->pcmend/2;j++)ilogmask[j]=ly; /* be certain */    
      seq++;
      return(1);
    }
  }else{
    oggpack_write(&vb->opb,0,1);
    memset(ilogmask,0,vb->pcmend/2*sizeof(*ilogmask));
    seq++;
    return(0);
  }
}

static unsigned values[64];static int widths[64],written;static int selected[64],codes[64],encoded;static codebook* active_books;
void __fastcall oggpack_write(oggpack_buffer*,unsigned value,int width) {assert(written<64);values[written]=value;widths[written++]=width;}
int __fastcall vorbis_book_encode(codebook* book,int value,oggpack_buffer*) {assert(encoded<64);selected[encoded]=(int)(book-active_books);codes[encoded++]=value;return 4;}
static int prediction(int xa,int xb,int ya,int yb,int x) {ya&=32767;yb&=32767;int difference=yb-ya;int magnitude=difference<0?-difference:difference;int offset=magnitude*(x-xa)/(xb-xa);return difference<0?ya-offset:ya+offset;}
int main() {
 vorbis_info_floor1 info;vorbis_look_floor1 look;vorbis_block block;vorbis_dsp_state dsp;vorbis_info vi;codec_setup_info setup;
 codebook books[3];static_codebook static_books[3];memset(&info,0,sizeof(info));memset(&look,0,sizeof(look));memset(&block,0,sizeof(block));memset(&dsp,0,sizeof(dsp));memset(&vi,0,sizeof(vi));memset(&setup,0,sizeof(setup));memset(books,0,sizeof(books));memset(static_books,0,sizeof(static_books));
 vi.codec_setup=&setup;dsp.vi=&vi;block.vd=&dsp;block.pcmend=32;setup.fullbooks=books;active_books=books;
 for(int i=0;i<3;++i){books[i].entries=static_books[i].entries=i==1?8:256;setup.book_param[i]=&static_books[i];}
 info.partitions=1;info.class_dim[0]=2;info.class_subs[0]=1;info.class_book[0]=0;info.class_subbook[0][0]=1;info.class_subbook[0][1]=2;
 info.postlist[0]=0;info.postlist[1]=16;info.postlist[2]=4;info.postlist[3]=12;
 look.vi=&info;look.n=16;look.posts=4;look.forward_index[0]=0;look.forward_index[1]=2;look.forward_index[2]=3;look.forward_index[3]=1;
 look.loneighbor[0]=0;look.hineighbor[0]=1;look.loneighbor[1]=2;look.hineighbor[1]=1;
 for(int multiplier=1;multiplier<=4;++multiplier)for(int pattern=0;pattern<30;++pattern)for(int flags=0;flags<4;++flags) {
  info.mult=multiplier;look.quant_q=multiplier==1?256:multiplier==2?128:multiplier==3?86:64;look.frames=look.postbits=look.phrasebits=0;
  int post[4],mask[18],decoded[4];for(int i=0;i<4;++i)post[i]=(pattern*17+i*119)%1024;
  if(flags&1)post[2]|=32768;if(flags&2)post[3]|=32768;for(int i=0;i<18;++i)mask[i]=-99;
  written=encoded=0;assert(floor1_encode(&block,&look,post,mask)==1);assert(written==3&&encoded==3);assert(values[0]==1&&widths[0]==1);
  decoded[0]=(int)values[1];decoded[1]=(int)values[2];assert(selected[0]==0);
  for(int i=2;i<4;++i) {
   int low=look.loneighbor[i-2],high=look.hineighbor[i-2];int predicted=prediction(info.postlist[low],info.postlist[high],decoded[low],decoded[high],info.postlist[i]);int value=codes[i-1];
   assert(selected[i-1]==info.class_subbook[0][(codes[0]>> (i-2))&1]);
   if(value) {int highroom=look.quant_q-predicted,lowroom=predicted,room=(highroom<lowroom?highroom:lowroom)*2;
    if(value>=room)value=highroom>lowroom?value-lowroom:-1-(value-highroom);else value=value&1?-((value+1)>>1):value>>1;
    decoded[i]=value+predicted;decoded[low]&=32767;decoded[high]&=32767;
   }else decoded[i]=predicted|32768;
  }
  for(int i=0;i<4;++i)assert(decoded[i]==post[i]);
  int previous=0;for(int order=1;order<4;++order){int next=look.forward_index[order];if(post[next]&32768)continue;int first=info.postlist[previous],last=info.postlist[next];for(int x=first;x<last;++x)assert(mask[x]==prediction(first,last,post[previous]*multiplier,post[next]*multiplier,x));previous=next;}
  assert(mask[16]==-99&&mask[17]==-99);assert(look.frames==1&&look.phrasebits==4&&look.postbits==widths[1]*2+8);
 }
 int mask[18];for(int i=0;i<18;++i)mask[i]=-99;written=encoded=0;assert(floor1_encode(&block,&look,0,mask)==0);assert(written==1&&values[0]==0&&widths[0]==1&&encoded==0);for(int i=0;i<16;++i)assert(mask[i]==0);assert(mask[16]==-99);
 puts("00ca2bb0_TEST PASS");return 0;
}