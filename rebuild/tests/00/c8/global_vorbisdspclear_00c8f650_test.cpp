#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define free test_free
#endif
// 00c8f650 vorbis_dsp_clear
// Leaf: @vorbis_dsp_clear@4
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
extern "C" {
extern vorbis_func_floor* _floor_P[2];
extern vorbis_func_residue* _residue_P[3];
void __fastcall _ve_envelope_clear(envelope_lookup*);
void __fastcall mdct_clear(mdct_lookup*);
void __fastcall _vp_psy_clear(vorbis_look_psy*);
void __fastcall _vp_global_free(vorbis_look_psy_global*);
void __fastcall vorbis_bitrate_clear(bitrate_manager_state*);
void __fastcall drft_clear(drft_lookup*);
}
extern "C" void __fastcall vorbis_dsp_clear(vorbis_dsp_state *v){
  int i;
  if(v){
    vorbis_info *vi=v->vi;
    codec_setup_info *ci=(codec_setup_info*)(vi?vi->codec_setup:NULL);
    private_state *b=(private_state*)v->backend_state;

    if(b){
	
      if(b->ve){
	_ve_envelope_clear(b->ve);
	_ogg_free(b->ve);
      }

      if(b->transform[0]){
	mdct_clear((mdct_lookup*)b->transform[0][0]);
	_ogg_free(b->transform[0][0]);
	_ogg_free(b->transform[0]);
      }
      if(b->transform[1]){
	mdct_clear((mdct_lookup*)b->transform[1][0]);
	_ogg_free(b->transform[1][0]);
	_ogg_free(b->transform[1]);
      }

      if(b->flr){
	for(i=0;i<ci->floors;i++)
	  _floor_P[ci->floor_type[i]]->
	    free_look(b->flr[i]);
	_ogg_free(b->flr);
      }
      if(b->residue){
	for(i=0;i<ci->residues;i++)
	  _residue_P[ci->residue_type[i]]->
	    free_look(b->residue[i]);
	_ogg_free(b->residue);
      }
      if(b->psy){
	for(i=0;i<ci->psys;i++)
	  _vp_psy_clear(b->psy+i);
	_ogg_free(b->psy);
      }

      if(b->psy_g_look)_vp_global_free(b->psy_g_look);
      vorbis_bitrate_clear(&b->bms);

      drft_clear(&b->fft_look[0]);
      drft_clear(&b->fft_look[1]);

    }
    
    if(v->pcm){
      for(i=0;i<vi->channels;i++)
	if(v->pcm[i])_ogg_free(v->pcm[i]);
      _ogg_free(v->pcm);
      if(v->pcmret)_ogg_free(v->pcmret);
    }

    if(b){
      /* free header, header1, header2 */
      if(b->header)_ogg_free(b->header);
      if(b->header1)_ogg_free(b->header1);
      if(b->header2)_ogg_free(b->header2);
      _ogg_free(b);
    }
    
    memset(v,0,sizeof(*v));
  }
}
typedef char dsp_size_check[sizeof(vorbis_dsp_state)==112?1:-1];
typedef char psy_size_check[sizeof(vorbis_look_psy)==48?1:-1];

#ifndef _MSC_VER
#undef free
#endif
extern "C" void __cdecl abort();
static void check(bool p){if(!p)abort();}
static void*allocations[48];static bool released[48];static unsigned allocated,freed;
static private_state backend;static vorbis_dsp_state dsp;static vorbis_info info;static codec_setup_info setup;
static unsigned veCalls,mdctCalls,psyCalls,globalCalls,bitrateCalls,drftCalls,floorCalls,residueCalls;
static unsigned char envelopeBytes[32],globalBytes[32],headers[3][16],floorBytes[3][16],residueBytes[3][16];
static mdct_lookup transforms[2];static void*transformSlots[2][1];static void*floorSlots[3],*residueSlots[3];static vorbis_look_psy psy[3];
static float channels[3][4];static float*pcmSlots[3],*pcmRetSlots[3];
static vorbis_func_floor floorRecords[2];static vorbis_func_residue residueRecords[3];
extern "C" {vorbis_func_floor*_floor_P[2]={&floorRecords[0],&floorRecords[1]};vorbis_func_residue*_residue_P[3]={&residueRecords[0],&residueRecords[1],&residueRecords[2]};}
static void reg(void*p){check(p&&allocated<48);for(unsigned i=0;i<allocated;++i)check(allocations[i]!=p);allocations[allocated]=p;released[allocated++]=false;}
extern "C" void __cdecl test_free(void*p){check(p!=0);unsigned i=0;for(;i<allocated;++i)if(allocations[i]==p)break;check(i<allocated&&!released[i]);if(p==&backend)for(unsigned j=0;j<allocated;++j)if(j!=i)check(released[j]);released[i]=true;++freed;}
#ifdef _MSC_VER
extern "C" {void (__cdecl* _imp__free)(void*)=test_free;}
#endif
extern "C" void __fastcall _ve_envelope_clear(envelope_lookup*p){check((void*)p==(void*)envelopeBytes);++veCalls;}
extern "C" void __fastcall mdct_clear(mdct_lookup*p){check(p==&transforms[0]||p==&transforms[1]);++mdctCalls;}
extern "C" void __fastcall _vp_psy_clear(vorbis_look_psy*p){check(p>=psy&&p<psy+setup.psys);++psyCalls;}
extern "C" void __fastcall _vp_global_free(vorbis_look_psy_global*p){check((void*)p==(void*)globalBytes);++globalCalls;test_free(p);}
extern "C" void __fastcall vorbis_bitrate_clear(bitrate_manager_state*p){check(p==&backend.bms);++bitrateCalls;}
extern "C" void __fastcall drft_clear(drft_lookup*p){check(p==&backend.fft_look[0]||p==&backend.fft_look[1]);++drftCalls;}
static void floorFree(void*p,int which){int i=0;for(;i<setup.floors;++i)if(p==floorSlots[i])break;check(i<setup.floors&&setup.floor_type[i]==which);++floorCalls;test_free(p);}
static void residueFree(void*p,int which){int i=0;for(;i<setup.residues;++i)if(p==residueSlots[i])break;check(i<setup.residues&&setup.residue_type[i]==which);++residueCalls;test_free(p);}
static void __fastcall floorZero(void*p){floorFree(p,0);}static void __fastcall floorOne(void*p){floorFree(p,1);}
static void __fastcall residueZero(void*p){residueFree(p,0);}static void __fastcall residueOne(void*p){residueFree(p,1);}static void __fastcall residueTwo(void*p){residueFree(p,2);}
static void run(unsigned mask){memset(&dsp,0,sizeof dsp);memset(&backend,0,sizeof backend);memset(&info,0,sizeof info);memset(&setup,0,sizeof setup);allocated=freed=veCalls=mdctCalls=psyCalls=globalCalls=bitrateCalls=drftCalls=floorCalls=residueCalls=0;info.codec_setup=&setup;info.channels=3;dsp.vi=&info;dsp.analysisp=83;dsp.pcm_current=47;setup.floors=1+(mask%3);setup.residues=1+((mask/3)%3);setup.psys=1+((mask/9)%3);bool hasBackend=(mask&1)!=0;
 if(hasBackend){dsp.backend_state=&backend;if(mask&2){backend.ve=(envelope_lookup*)envelopeBytes;reg(envelopeBytes);}for(unsigned k=0;k<2;++k)if(mask&(4U<<k)){backend.transform[k]=transformSlots[k];transformSlots[k][0]=&transforms[k];reg(&transforms[k]);reg(transformSlots[k]);}if(mask&16){backend.flr=floorSlots;for(int k=0;k<setup.floors;++k){floorSlots[k]=floorBytes[k];setup.floor_type[k]=k%2;reg(floorBytes[k]);}reg(floorSlots);}if(mask&32){backend.residue=residueSlots;for(int k=0;k<setup.residues;++k){residueSlots[k]=residueBytes[k];setup.residue_type[k]=k%3;reg(residueBytes[k]);}reg(residueSlots);}if(mask&64){backend.psy=psy;reg(psy);}if(mask&128){backend.psy_g_look=(vorbis_look_psy_global*)globalBytes;reg(globalBytes);}if(mask&2){backend.header=headers[0];reg(headers[0]);}if(mask&32){backend.header1=headers[1];reg(headers[1]);}if(mask&128){backend.header2=headers[2];reg(headers[2]);}reg(&backend);}
 if(mask&256){dsp.pcm=pcmSlots;for(unsigned k=0;k<3;++k){pcmSlots[k]=(mask&(2U<<k))?channels[k]:0;if(pcmSlots[k])reg(pcmSlots[k]);}reg(pcmSlots);if(mask&64){dsp.pcmret=pcmRetSlots;reg(pcmRetSlots);}}
 unsigned char savedInfo[sizeof info],savedSetup[sizeof setup];memcpy(savedInfo,&info,sizeof info);memcpy(savedSetup,&setup,sizeof setup);vorbis_dsp_clear(&dsp);check(freed==allocated);for(unsigned i=0;i<sizeof dsp;++i)check(((unsigned char*)&dsp)[i]==0);check(memcmp(&info,savedInfo,sizeof info)==0&&memcmp(&setup,savedSetup,sizeof setup)==0);check(veCalls==(hasBackend&&(mask&2)?1U:0U));check(mdctCalls==(hasBackend?((mask&4)?1U:0U)+((mask&8)?1U:0U):0U));check(floorCalls==(hasBackend&&(mask&16)?(unsigned)setup.floors:0U));check(residueCalls==(hasBackend&&(mask&32)?(unsigned)setup.residues:0U));check(psyCalls==(hasBackend&&(mask&64)?(unsigned)setup.psys:0U));check(globalCalls==(hasBackend&&(mask&128)?1U:0U));check(bitrateCalls==(hasBackend?1U:0U)&&drftCalls==(hasBackend?2U:0U));unsigned oldFreed=freed;vorbis_dsp_clear(&dsp);check(freed==oldFreed);}
int main(){check(sizeof(vorbis_dsp_state)==112&&sizeof(vorbis_look_psy)==48);memset(floorRecords,0,sizeof floorRecords);memset(residueRecords,0,sizeof residueRecords);floorRecords[0].free_look=floorZero;floorRecords[1].free_look=floorOne;residueRecords[0].free_look=residueZero;residueRecords[1].free_look=residueOne;residueRecords[2].free_look=residueTwo;vorbis_dsp_clear(0);for(unsigned mask=0;mask<512;++mask)run(mask);memset(&dsp,0,sizeof dsp);dsp.analysisp=1;vorbis_dsp_clear(&dsp);for(unsigned i=0;i<sizeof dsp;++i)check(((unsigned char*)&dsp)[i]==0);puts("00c8f650_TEST PASS");return 0;}