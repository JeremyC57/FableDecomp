// libvorbis::ov_crosslap(OggVorbis_File* vf1, OggVorbis_File* vf2) -- vorbisfile.c public API: cross-lap the end of vf1's current block into the start of vf2's next one (prime both, save vf1's lapping data with _ov_getlap into alloca'd per-channel buffers, expose vf2's lapping buffer with vorbis_synthesis_lapout, dump both channels through _analysis_output_always, then _ov_splice)
// Leaf: @ov_crosslap@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8C3E0, 416 bytes, __fastcall (vf1 in ecx, vf2 in edx), plain `ret`; ebp frame for the
//   alloca'd lapping buffers (`call __alloca_probe` 0x00BFEA30 with eax = size, `lea esp,[ebp-2Ch]` epilogues).
// Identity (two sources): (1) the body is Xiph libvorbis vorbisfile.c `ov_crosslap` verbatim (the "libVorbis I 20030909"
//   post-1.0.1 CVS that libpk links, GOTCHAS chunk 446), including the two `_analysis_output_always("pcmL"/"pcmR", 0,
//   pcm[0|1], n1*2, 0, 0, 0)` debug dumps of that CVS snapshot (strings 0x012BF228 "pcmL" / 0x012BF220 "pcmR"); vf1 == vf2
//   returns 0, OV_EINVAL (-131) below OPENED (ready_state +0x58), _ov_initset(vf1), _ov_initprime(vf2), ov_info(vf,-1) and
//   ov_halfrate_p for both, n = vorbis_info_blocksize(vi,0) >> (1+hs), vorbis_window(&vf->vd (+0x1E0), 0); (2) its place in
//   retail follows upstream file order: _ov_getlap 0x00C8C240, ov_crosslap 0x00C8C3E0, _ov_64_seek_lap 0x00C8C580.
//   The manifest label "WorldMap_CompareAndBlend" is propagated and wrong.
// Callees: _ov_initset 0x00C8C1D0 / _ov_initprime 0x00C8C200 (statics, vf in esi), ov_info 0x00C8BAB0 (`@ov_info@8`),
//   ov_halfrate_p 0x00C8A920, vorbis_info_blocksize 0x00C8DF60, vorbis_window 0x00C90C80, __alloca_probe 0x00BFEA30,
//   _ov_getlap 0x00C8C240 (static, `ret 14h`, every argument on the stack), vorbis_synthesis_lapout 0x00C90950,
//   _analysis_output_always 0x00C90D60 (`@_analysis_output_always@32`), _ov_splice 0x00C8BF90 (static: n1 ecx, w1 edx,
//   n2 eax, the rest on the stack, `ret 14h`).
// Shape: the landed vorbisfile.c slice of _ov_64_seek_lap 0x00C8C580 (same-TU noinline statics give retail's private
//   registers) with ov_crosslap added in upstream order; only the `@ov_crosslap@8` leaf is this row.
// Original libvorbis vorbisfile.c; BSD-style terms in reference COPYING.
typedef __int64 ogg_int64_t;
extern "C" void* __cdecl _alloca(unsigned int size);
#pragma intrinsic(_alloca)
#define alloca _alloca
#define NULL 0
#define OV_EOF -2
#define OV_HOLE -3
#define OV_EINVAL -131
#define OPENED 2
#define STREAMSET 3
#define INITSET 4

extern "C" {

typedef struct vorbis_info {
  int version; int channels; long rate;
  long bitrate_upper; long bitrate_nominal; long bitrate_lower; long bitrate_window;
  void *codec_setup;
} vorbis_info;
typedef struct vorbis_comment vorbis_comment;
typedef struct ogg_packet ogg_packet;
typedef struct vorbis_dsp_state {
  int analysisp; vorbis_info *vi; float **pcm; float **pcmret; int pcm_storage; int pcm_current;
  int pcm_returned; int preextrapolate; int eofflag; long lW; long W; long nW; long centerW;
  ogg_int64_t granulepos; ogg_int64_t sequence; ogg_int64_t glue_bits; ogg_int64_t time_bits;
  ogg_int64_t floor_bits; ogg_int64_t res_bits; void *backend_state;
} vorbis_dsp_state;
typedef struct ogg_sync_state {
  unsigned char *data; int storage; int fill; int returned; int unsynced; int headerbytes; int bodybytes;
} ogg_sync_state;
typedef struct OggVorbis_File {      /* libpk's fork: no ov_callbacks member */
  void            *datasource;       /* +0x00 */
  int              seekable;         /* +0x04 */
  ogg_int64_t      offset;           /* +0x08 */
  ogg_int64_t      end;              /* +0x10 */
  ogg_sync_state   oy;               /* +0x18 */
  int              links;            /* +0x34 */
  ogg_int64_t     *offsets;          /* +0x38 */
  ogg_int64_t     *dataoffsets;      /* +0x3c */
  long            *serialnos;        /* +0x40 */
  ogg_int64_t     *pcmlengths;       /* +0x44 */
  vorbis_info     *vi;               /* +0x48 */
  vorbis_comment  *vc;               /* +0x4c */
  ogg_int64_t      pcm_offset;       /* +0x50 */
  int              ready_state;      /* +0x58 */
  long             current_serialno; /* +0x5c */
  int              current_link;     /* +0x60 */
  double           bittrack;         /* +0x68 */
  double           samptrack;        /* +0x70 */
  unsigned char    os[0x168];        /* +0x78 ogg_stream_state */
  vorbis_dsp_state vd;               /* +0x1e0 */
} OggVorbis_File;

int __fastcall vorbis_info_blocksize(vorbis_info *vi,int zo);          /* 0x00C8DF60 */
float * __fastcall vorbis_window(vorbis_dsp_state *v,int W);           /* 0x00C90C80 */
int __fastcall vorbis_synthesis_pcmout(vorbis_dsp_state *v,float ***pcm);   /* 0x00C908E0 */
int __fastcall vorbis_synthesis_read(vorbis_dsp_state *v,int samples);      /* 0x00C90930 */
int __fastcall vorbis_synthesis_lapout(vorbis_dsp_state *v,float ***pcm);   /* 0x00C90950 */
int __fastcall ov_halfrate_p(OggVorbis_File *vf);                       /* 0x00C8A920 */
int __fastcall ov_raw_seek(OggVorbis_File *vf,ogg_int64_t pos);         /* 0x00C8AB70 */
int __fastcall ov_pcm_seek(OggVorbis_File *vf,ogg_int64_t pos);         /* 0x00C8B490 */
int __fastcall ov_pcm_seek_page(OggVorbis_File *vf,ogg_int64_t pos);    /* 0x00C8AE90 */
int __stdcall _fetch_and_process_packet(OggVorbis_File *vf, ogg_packet *op_in, int readp, int spanp);   /* 0x00C8A540 (static; vf in eax) */
void __stdcall _ov_getlap(OggVorbis_File *vf,vorbis_info *vi,vorbis_dsp_state *vd,
                          float **lappcm,int lapsize);                  /* 0x00C8C240 (static; all on the stack, ret 14h) */

__declspec(noinline) vorbis_info * __fastcall ov_info(OggVorbis_File *vf,int link){         /* 0x00C8BAB0 (same TU: leaves ecx alone) */
  if(vf->seekable){
    if(link<0)
      if(vf->ready_state>=STREAMSET)
        return vf->vi+vf->current_link;
      else
        return vf->vi;
    else
      if(link>=vf->links)
        return NULL;
      else
        return vf->vi+link;
  }else{
    return vf->vi;
  }
}

static __declspec(noinline) void __stdcall _ov_splice(float **pcm,float **lappcm,
                       int n1, int n2,
                       int ch1, int ch2,
                       float *w1, float *w2){
  int i,j;
  float *w=w1;
  int n=n1;

  if(n1>n2){
    n=n2;
    w=w2;
  }

  /* splice */
  for(j=0;j<ch1 && j<ch2;j++){
    float *s=lappcm[j];
    float *d=pcm[j];

    for(i=0;i<n;i++){
      float wd=w[i]*w[i];
      float ws=1.-wd;
      d[i]=d[i]*wd + s[i]*ws;
    }
  }
  /* window from zero */
  for(;j<ch2;j++){
    float *d=pcm[j];
    for(i=0;i<n;i++){
      float wd=w[i]*w[i];
      d[i]=d[i]*wd;
    }
  }

}

/* make sure vf is INITSET */
static __declspec(noinline) int _ov_initset(OggVorbis_File *vf){
  while(1){
    if(vf->ready_state==INITSET)break;
    /* suck in another packet */
    {
      int ret=_fetch_and_process_packet(vf,NULL,1,0);
      if(ret<0 && ret!=OV_HOLE)return(ret);
    }
  }
  return 0;
}

/* make sure vf is INITSET and that we have a primed buffer; if
   we're crosslapping at a stream section boundary, this also makes
   sure we're sanity checking against the right stream information */
static __declspec(noinline) int _ov_initprime(OggVorbis_File *vf){
  vorbis_dsp_state *vd=&vf->vd;
  while(1){
    if(vf->ready_state==INITSET)
      if(vorbis_synthesis_pcmout(vd,NULL))break;

    /* suck in another packet */
    {
      int ret=_fetch_and_process_packet(vf,NULL,1,0);
      if(ret<0 && ret!=OV_HOLE)return(ret);
    }
  }
  return 0;
}

void __fastcall _analysis_output_always(char *base,int i,float *v,int n,int bark,int dB,ogg_int64_t off);   /* 0x00C90D60 */

int __fastcall ov_crosslap(OggVorbis_File *vf1,OggVorbis_File *vf2){
  vorbis_info *vi1,*vi2;
  float **lappcm;
  float **pcm;
  float *w1,*w2;
  int n1,n2,i,ret,hs1,hs2;

  if(vf1==vf2)return(0); /* degenerate case */
  if(vf1->ready_state<OPENED)return(OV_EINVAL);
  if(vf2->ready_state<OPENED)return(OV_EINVAL);

  /* the relevant overlap buffers must be pre-checked and pre-primed
     before looking at settings in the event that priming would cross
     a bitstream boundry.  So, do it now */

  ret=_ov_initset(vf1);
  if(ret)return(ret);
  ret=_ov_initprime(vf2);
  if(ret)return(ret);

  vi1=ov_info(vf1,-1);
  vi2=ov_info(vf2,-1);
  hs1=ov_halfrate_p(vf1);
  hs2=ov_halfrate_p(vf2);

  lappcm=(float **)alloca(sizeof(*lappcm)*vi1->channels);
  n1=vorbis_info_blocksize(vi1,0)>>(1+hs1);
  n2=vorbis_info_blocksize(vi2,0)>>(1+hs2);
  w1=vorbis_window(&vf1->vd,0);
  w2=vorbis_window(&vf2->vd,0);

  for(i=0;i<vi1->channels;i++)
    lappcm[i]=(float *)alloca(sizeof(**lappcm)*n1);

  _ov_getlap(vf1,vi1,&vf1->vd,lappcm,n1);

  /* have a lapping buffer from vf1; now to splice it into the lapping
     buffer of vf2 */
  /* consolidate and expose the buffer. */
  vorbis_synthesis_lapout(&vf2->vd,&pcm);
  _analysis_output_always("pcmL",0,pcm[0],n1*2,0,0,0);
  _analysis_output_always("pcmR",0,pcm[1],n1*2,0,0,0);

  /* splice */
  _ov_splice(pcm,lappcm,n1,n2,vi1->channels,vi2->channels,w1,w2);

  /* done */
  return(0);
}

static int __stdcall _ov_64_seek_lap(OggVorbis_File *vf,ogg_int64_t pos,
                           int (__fastcall *localseek)(OggVorbis_File *,ogg_int64_t)){
  vorbis_info *vi;
  float **lappcm;
  float **pcm;
  float *w1,*w2;
  int n1,n2,ch1,ch2,hs;
  int i,ret;

  if(vf->ready_state<OPENED)return(OV_EINVAL);
  ret=_ov_initset(vf);
  if(ret)return(ret);
  vi=ov_info(vf,-1);
  hs=ov_halfrate_p(vf);

  ch1=vi->channels;
  n1=vorbis_info_blocksize(vi,0)>>(1+hs);
  w1=vorbis_window(&vf->vd,0);  /* window arrays from libvorbis are
                                   persistent; even if the decode state
                                   from this link gets dumped, this
                                   window array continues to exist */

  lappcm=(float **)alloca(sizeof(*lappcm)*ch1);
  for(i=0;i<ch1;i++)
    lappcm[i]=(float *)alloca(sizeof(**lappcm)*n1);
  _ov_getlap(vf,vi,&vf->vd,lappcm,n1);

  /* have lapping data; seek and prime the buffer */
  ret=localseek(vf,pos);
  if(ret)return ret;
  ret=_ov_initprime(vf);
  if(ret)return(ret);

 /* Guard against cross-link changes; they're perfectly legal */
  vi=ov_info(vf,-1);
  ch2=vi->channels;
  n2=vorbis_info_blocksize(vi,0)>>(1+hs);
  w2=vorbis_window(&vf->vd,0);

  /* consume and process the lapping data */
  vorbis_synthesis_lapout(&vf->vd,&pcm);

  /* splice */
  _ov_splice(pcm,lappcm,n1,n2,ch1,ch2,w1,w2);

  /* done */
  return(0);
}

int __fastcall ov_raw_seek_lap(OggVorbis_File *vf,ogg_int64_t pos){
  return _ov_64_seek_lap(vf,pos,ov_raw_seek);
}

int __fastcall ov_pcm_seek_lap(OggVorbis_File *vf,ogg_int64_t pos){
  return _ov_64_seek_lap(vf,pos,ov_pcm_seek);
}

int __fastcall ov_pcm_seek_page_lap(OggVorbis_File *vf,ogg_int64_t pos){
  return _ov_64_seek_lap(vf,pos,ov_pcm_seek_page);
}

}   /* extern "C" */