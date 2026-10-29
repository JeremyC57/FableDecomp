#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define free test_free
#endif
// 00c8dfb0 vorbis_info_clear
// Leaf: @vorbis_info_clear@4
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
extern "C" {
extern vorbis_func_floor* _floor_P[2];
extern vorbis_func_residue* _residue_P[3];
extern vorbis_func_mapping* _mapping_P[1];
void __fastcall vorbis_staticbook_destroy(static_codebook*);
void __fastcall vorbis_book_clear(codebook*);
void __fastcall _vi_psy_free(vorbis_info_psy*);
}
extern "C" void __fastcall vorbis_info_clear(vorbis_info *vi){
  codec_setup_info     *ci=(codec_setup_info*)vi->codec_setup;
  int i;

  if(ci){

    for(i=0;i<ci->modes;i++)
      if(ci->mode_param[i])_ogg_free(ci->mode_param[i]);

    for(i=0;i<ci->maps;i++) /* unpack does the range checking */
      _mapping_P[ci->map_type[i]]->free_info(ci->map_param[i]);

    for(i=0;i<ci->floors;i++) /* unpack does the range checking */
      _floor_P[ci->floor_type[i]]->free_info(ci->floor_param[i]);
    
    for(i=0;i<ci->residues;i++) /* unpack does the range checking */
      _residue_P[ci->residue_type[i]]->free_info(ci->residue_param[i]);

    for(i=0;i<ci->books;i++){
      if(ci->book_param[i]){
	/* knows if the book was not alloced */
	vorbis_staticbook_destroy(ci->book_param[i]);
      }
      if(ci->fullbooks)
	vorbis_book_clear(ci->fullbooks+i);
    }
    if(ci->fullbooks)
	_ogg_free(ci->fullbooks);
    
    for(i=0;i<ci->psys;i++)
      _vi_psy_free(ci->psy_param[i]);

    _ogg_free(ci);
  }

  memset(vi,0,sizeof(*vi));
}
typedef char info_size_check[sizeof(vorbis_info)==32?1:-1];
typedef char book_size_check[sizeof(codebook)==44?1:-1];

#ifndef _MSC_VER
#undef free
#endif
extern "C" void __cdecl abort();
static void check(bool value){if(!value)abort();}
static codec_setup_info setup;static vorbis_info info;
static void*registered[40];static bool released[40];static unsigned allocated,freed;
static unsigned mapCalls,floorCalls,residueCalls,bookCalls,fullCalls,psyCalls;
static vorbis_info_mode modes[3];static unsigned char maps[3][16],floors[3][16],residues[3][16],psys[3][16];
static static_codebook books[3];static codebook fullbooks[3];
static vorbis_func_floor floorRecords[2];static vorbis_func_residue residueRecords[3];static vorbis_func_mapping mapRecord;
extern "C" {vorbis_func_floor*_floor_P[2]={&floorRecords[0],&floorRecords[1]};vorbis_func_residue*_residue_P[3]={&residueRecords[0],&residueRecords[1],&residueRecords[2]};vorbis_func_mapping*_mapping_P[1]={&mapRecord};}
static void reg(void*p){check(p&&allocated<40);for(unsigned i=0;i<allocated;++i)check(registered[i]!=p);registered[allocated]=p;released[allocated++]=false;}
extern "C" void __cdecl test_free(void*p){check(p);unsigned i=0;for(;i<allocated;++i)if(registered[i]==p)break;check(i<allocated&&!released[i]);if(p==&setup)for(unsigned j=0;j<allocated;++j)if(j!=i)check(released[j]);released[i]=true;++freed;}
#ifdef _MSC_VER
extern "C" {void (__cdecl* _imp__free)(void*)=test_free;}
#endif
static void __fastcall mapFree(void*p){unsigned i=mapCalls++;check(i<(unsigned)setup.maps&&p==setup.map_param[i]&&setup.map_type[i]==0);if(p)test_free(p);}
static void floorFree(void*p,int which){unsigned i=floorCalls++;check(i<(unsigned)setup.floors&&p==setup.floor_param[i]&&setup.floor_type[i]==which);if(p)test_free(p);}
static void residueFree(void*p,int which){unsigned i=residueCalls++;check(i<(unsigned)setup.residues&&p==setup.residue_param[i]&&setup.residue_type[i]==which);if(p)test_free(p);}
static void __fastcall floorZero(void*p){floorFree(p,0);}static void __fastcall floorOne(void*p){floorFree(p,1);}
static void __fastcall residueZero(void*p){residueFree(p,0);}static void __fastcall residueOne(void*p){residueFree(p,1);}static void __fastcall residueTwo(void*p){residueFree(p,2);}
extern "C" void __fastcall vorbis_staticbook_destroy(static_codebook*p){check(p>=books&&p<books+3);++bookCalls;if(p->allocedp)test_free(p);}
extern "C" void __fastcall vorbis_book_clear(codebook*p){check(p==fullbooks+fullCalls&&fullCalls<(unsigned)setup.books);++fullCalls;}
extern "C" void __fastcall _vi_psy_free(vorbis_info_psy*p){unsigned i=psyCalls++;check(i<(unsigned)setup.psys&&(void*)p==(void*)setup.psy_param[i]);if(p)test_free(p);}
static void run(unsigned mask){memset(&info,0,sizeof info);memset(&setup,0,sizeof setup);memset(books,0,sizeof books);allocated=freed=mapCalls=floorCalls=residueCalls=bookCalls=fullCalls=psyCalls=0;info.rate=8192;info.channels=2;bool active=(mask&1)!=0;if(active){info.codec_setup=&setup;int n=1+(mask%3);setup.modes=(mask&2)?n:0;setup.maps=(mask&4)?n:0;setup.floors=(mask&8)?n:0;setup.residues=(mask&16)?n:0;setup.books=(mask&32)?n:0;setup.psys=(mask&128)?n:0;bool holes=(mask&256)!=0;
 for(int i=0;i<n;++i){bool present=!holes||i%2==0;if(i<setup.modes){setup.mode_param[i]=present?&modes[i]:0;if(present)reg(&modes[i]);}if(i<setup.maps){setup.map_type[i]=0;setup.map_param[i]=present?maps[i]:0;if(present)reg(maps[i]);}if(i<setup.floors){setup.floor_type[i]=i%2;setup.floor_param[i]=present?floors[i]:0;if(present)reg(floors[i]);}if(i<setup.residues){setup.residue_type[i]=i%3;setup.residue_param[i]=present?residues[i]:0;if(present)reg(residues[i]);}if(i<setup.books){setup.book_param[i]=present?&books[i]:0;books[i].allocedp=(mask&512)?0:1;if(present&&books[i].allocedp)reg(&books[i]);}if(i<setup.psys){setup.psy_param[i]=present?(vorbis_info_psy*)psys[i]:0;if(present)reg(psys[i]);}}
 if(mask&64){setup.fullbooks=fullbooks;reg(fullbooks);}reg(&setup);}
 unsigned expectedBooks=0;for(int i=0;i<setup.books;++i)if(setup.book_param[i])++expectedBooks;vorbis_info_clear(&info);check(allocated==freed);for(unsigned i=0;i<sizeof info;++i)check(((unsigned char*)&info)[i]==0);check(mapCalls==(unsigned)setup.maps&&floorCalls==(unsigned)setup.floors&&residueCalls==(unsigned)setup.residues&&bookCalls==expectedBooks&&fullCalls==(setup.fullbooks?(unsigned)setup.books:0U)&&psyCalls==(unsigned)setup.psys);unsigned oldFreed=freed;vorbis_info_clear(&info);check(freed==oldFreed);}
int main(){check(sizeof(vorbis_info)==32&&sizeof(codebook)==44);memset(floorRecords,0,sizeof floorRecords);memset(residueRecords,0,sizeof residueRecords);memset(&mapRecord,0,sizeof mapRecord);floorRecords[0].free_info=floorZero;floorRecords[1].free_info=floorOne;residueRecords[0].free_info=residueZero;residueRecords[1].free_info=residueOne;residueRecords[2].free_info=residueTwo;mapRecord.free_info=mapFree;for(unsigned mask=0;mask<1024;++mask)run(mask);puts("00c8dfb0_TEST PASS");return 0;}