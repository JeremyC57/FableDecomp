// libvorbis::_preextrapolate_helper(vorbis_dsp_state* v) -- libvorbis block.c: once enough PCM is buffered, run a 32-order LPC extrapolation backwards over every channel to pre-roll the start of the stream
// Leaf: __preextrapolate_helper
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8F960, 246 bytes, a TU-local static (or API) of the same library file as the landed
//   row 0x00C8FA60. Shape (retry1): parked as an "LTCG custom register convention" gap; the landed host TU already holds this
//   function's real body inside `extern "C" { static __declspec(noinline) }` beside its real caller, and graded with this
//   leaf it matches retail as is (VC7.1 /O2 picks the private convention itself). The source is the host TU verbatim.
// TU host (landed row 0x00C8FA60): libvorbis::vorbis_analysis_wrote(vorbis_dsp_state* v, int vals) -- libvorbis block.c: account for vals newly written samples (OV_EINVAL past the buffer; reverse-extrapolate the stream start once enough is buffered), or at end of stream (vals <= 0) pad three long blocks and LPC-extrapolate each channel into them (zeroes when too little data)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8FA60, 307 bytes, extern "C" __fastcall (v ecx, vals edx, plain `ret`), ebp frame
//   for the 32-float LPC alloca (`mov eax,80h ; call __alloca_probe` 0x00BFEA30).
// Identity (two sources): (1) the body is Xiph libvorbis 1.0.1 lib/block.c `vorbis_analysis_wrote` verbatim (order 32,
//   OV_EINVAL -131 = 0xFFFFFF7D, ci->blocksizes[1]*3, v->eofflag / pcm_current / pcm_storage / preextrapolate / centerW at
//   +0x20 / +0x14 / +0x10 / +0x1C / +0x30 of the landed vorbis_dsp_state model), inside libpk's libVorbis block ("Xiph.Org
//   libVorbis I 20030909"); (2) its callees are the landed libvorbis API vorbis_analysis_buffer 0x00C8F880,
//   vorbis_lpc_from_data 0x00C9E5D0 (its float result dropped: `fstp st(0)`) and vorbis_lpc_predict 0x00C9E8F0, plus
//   block.c's static _preextrapolate_helper 0x00C8F960 (v in esi, the twin LPC body). The manifest label
//   "Audio_ProcessConvolutionBuffer" is descriptive.
// Shape: the public API is LTCG `extern "C" __fastcall` (GOTCHAS chunk 446). _preextrapolate_helper is written as a
//   same-TU static with its verbatim 1.0.1 body, so VC7.1 at plain /O2 gives it the private convention retail has (v in
//   esi, nothing pushed); the memset is the intrinsic (`rep stosd` / `rep stosb`).
extern "C" {

extern "C" void* __cdecl memset(void* dst, int c, unsigned int count);
#pragma intrinsic(memset)
extern "C" void* __cdecl _alloca(unsigned int size);
#pragma intrinsic(_alloca)
#define alloca _alloca
#define OV_EINVAL -131
typedef __int64 ogg_int64_t;

struct vorbis_info {                    // 0x20 bytes
    int   version;                      // +0x00
    int   channels;                     // +0x04
    long  rate;                         // +0x08
    long  bitrate_upper;                // +0x0c
    long  bitrate_nominal;              // +0x10
    long  bitrate_lower;                // +0x14
    long  bitrate_window;               // +0x18
    void* codec_setup;                  // +0x1c codec_setup_info*
};
struct codec_setup_info {               // only the head is used here
    long  blocksizes[2];                // +0x000
};
struct vorbis_dsp_state {               // 0x70 bytes
    int          analysisp;             // +0x00
    vorbis_info* vi;                    // +0x04
    float**      pcm;                   // +0x08
    float**      pcmret;                // +0x0c
    int          pcm_storage;           // +0x10
    int          pcm_current;           // +0x14
    int          pcm_returned;          // +0x18
    int          preextrapolate;        // +0x1c
    int          eofflag;               // +0x20
    long         lW;                    // +0x24
    long         W;                     // +0x28
    long         nW;                    // +0x2c
    long         centerW;               // +0x30
    ogg_int64_t  granulepos;            // +0x38
    ogg_int64_t  sequence;              // +0x40
    ogg_int64_t  glue_bits;             // +0x48
    ogg_int64_t  time_bits;             // +0x50
    ogg_int64_t  floor_bits;            // +0x58
    ogg_int64_t  res_bits;              // +0x60
    void*        backend_state;         // +0x68 private_state*
};

float** __fastcall vorbis_analysis_buffer(vorbis_dsp_state* v, int vals);                   // 0x00C8F880
float __fastcall vorbis_lpc_from_data(float *data,float *lpci,int n,int m);                  // 0x00C9E5D0
void __fastcall vorbis_lpc_predict(float *coeff,float *prime,int m,float *data,long n);      // 0x00C9E8F0

/* 0x00C8F960: block.c static (private convention: v in esi) */
static void _preextrapolate_helper(vorbis_dsp_state *v){
  int i;
  int order=32;
  float *lpc=(float*)alloca(order*sizeof(*lpc));
  float *work=(float*)alloca(v->pcm_current*sizeof(*work));
  long j;
  v->preextrapolate=1;

  if(v->pcm_current-v->centerW>order*2){ /* safety */
    for(i=0;i<v->vi->channels;i++){
      /* need to run the extrapolation in reverse! */
      for(j=0;j<v->pcm_current;j++)
        work[j]=v->pcm[i][v->pcm_current-j-1];

      /* prime as above */
      vorbis_lpc_from_data(work,lpc,v->pcm_current-v->centerW,order);

      /* run the predictor filter */
      vorbis_lpc_predict(lpc,work+v->pcm_current-v->centerW-order,
                         order,
                         work+v->pcm_current-v->centerW,
                         v->centerW);

      for(j=0;j<v->pcm_current;j++)
        v->pcm[i][v->pcm_current-j-1]=work[j];

    }
  }
}

/* call with val<=0 to set eof */

int __fastcall vorbis_analysis_wrote(vorbis_dsp_state *v, int vals){
  vorbis_info *vi=v->vi;
  codec_setup_info *ci=(codec_setup_info*)vi->codec_setup;
  /*private_state *b=v->backend_state;*/

  if(vals<=0){
    int order=32;
    int i;
    float *lpc=(float*)alloca(order*sizeof(*lpc));

    /* if it wasn't done earlier (very short sample) */
    if(!v->preextrapolate)
      _preextrapolate_helper(v);

    /* We're encoding the end of the stream.  Just make sure we have
       [at least] a few full blocks of zeroes at the end. */
    /* actually, we don't want zeroes; that could drop a large
       amplitude off a cliff, creating spread spectrum noise that will
       suck to encode.  Extrapolate for the sake of cleanliness. */

    vorbis_analysis_buffer(v,ci->blocksizes[1]*3);
    v->eofflag=v->pcm_current;
    v->pcm_current+=ci->blocksizes[1]*3;

    for(i=0;i<vi->channels;i++){
      if(v->eofflag>order*2){
        /* extrapolate with LPC to fill in */
        long n;

        /* make a predictor filter */
        n=v->eofflag;
        if(n>ci->blocksizes[1])n=ci->blocksizes[1];
        vorbis_lpc_from_data(v->pcm[i]+v->eofflag-n,lpc,n,order);

        /* run the predictor filter */
        vorbis_lpc_predict(lpc,v->pcm[i]+v->eofflag-order,order,
                           v->pcm[i]+v->eofflag,v->pcm_current-v->eofflag);
      }else{
        /* not enough data to extrapolate (unlikely to happen due to
           guarding the overlap, but bulletproof in case that
           assumtion goes away). zeroes will do. */
        memset(v->pcm[i]+v->eofflag,0,
               (v->pcm_current-v->eofflag)*sizeof(*v->pcm[i]));

      }
    }
  }else{

    if(v->pcm_current+vals>v->pcm_storage)
      return(OV_EINVAL);

    v->pcm_current+=vals;

    /* we may want to reverse extrapolate the beginning of a stream
       too... in case we're beginning on a cliff! */
    /* clumsy, but simple.  It only runs once, so simple is good. */
    if(!v->preextrapolate && v->pcm_current-v->centerW>ci->blocksizes[1])
      _preextrapolate_helper(v);

  }
  return(0);
}

}   /* extern "C" */