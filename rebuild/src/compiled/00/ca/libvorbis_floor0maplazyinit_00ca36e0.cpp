// libvorbis::floor0_map_lazy_init(vorbis_block* vb, vorbis_info_floor* infoX, vorbis_look_floor0* look) -- libvorbis floor0.c: on first use of a block size build the linear-bin to bark-band map (n = blocksize/2 entries scaled so bark(rate/2) lands on ln, clamped to ln-1, terminated by -1) and record n
// Leaf: @floor0_map_lazy_init@12
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA36E0, 361 bytes. libpk's statically linked Xiph libvorbis ("Xiph.Org libVorbis I
//   20030909"), lib/floor0.c static floor0_map_lazy_init. Private register convention (no /GL): vb in eax, look in edi,
//   infoX on the stack, `ret 4`; the only caller is floor0_inverse2 0x00CA39C0 (`push ebx ; mov eax,esi ; call`).
//   The static body is defined here with a same-TU caller (floor0_inverse2, its real 2003 body) so VC7.1 gives it that
//   convention itself. Land the leaf only.
// Identity (proven): the CVS-2003 floor0.c body verbatim against the binary: look->linearmap (+8)[vb->W (+0x1C)] null test,
//   vb->vd (+0x40)->vi (+4)->codec_setup (+0x1C)->blocksizes[W] / 2, the toBARK macro constants 1.85e-8f (0x012BF310),
//   2.24f (0x012BF298, double), .00074f (0x012BF30C), 13.1f (0x012BF288, double), 1e-4f (0x0129F0BC), rate/2.f as *0.5f
//   (0x0122F59C), malloc / floor through their MSVCR71 imports ([0x01440158] / [0x01440100]), __ftol2 0x00BFEA70, the
//   ln-1 clamp, the -1 terminator and look->n[W] (+0xC). Manifest label "Math_PrecomputeAngleLookupTable" is wrong.
// Layout: the libvorbis codec.h / codec_internal.h / floor0.c structures as the landed floor0_look 0x00CA3850 uses them.
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);   // [0x01440158]
extern "C" __declspec(dllimport) double __cdecl floor(double x);           // [0x01440100]
extern "C" double __cdecl atan(double x);
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(atan, memset)
#define _ogg_malloc malloc
#define toBARK(n)   (13.1f*atan(.00074f*(n))+2.24f*atan((n)*(n)*1.85e-8f)+1e-4f*(n))

typedef __int64 ogg_int64_t;                       // ogg/os_types.h (_WIN32)
typedef void vorbis_info_floor;
typedef void vorbis_look_floor;
typedef struct { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; } oggpack_buffer;
typedef struct vorbis_info {
    int version; int channels; long rate;
    long bitrate_upper; long bitrate_nominal; long bitrate_lower; long bitrate_window;
    void* codec_setup;                 // +0x1c
} vorbis_info;
typedef struct vorbis_dsp_state { int analysisp; vorbis_info* vi; } vorbis_dsp_state;
typedef struct vorbis_block {
    float** pcm;                       // +0x00
    oggpack_buffer opb;                // +0x04
    long lW; long W; long nW;          // +0x18
    int pcmend; int mode;              // +0x24
    int eofflag;                       // +0x2c
    ogg_int64_t granulepos;            // +0x30
    ogg_int64_t sequence;              // +0x38
    vorbis_dsp_state* vd;              // +0x40
} vorbis_block;
typedef struct codec_setup_info { long blocksizes[2]; } codec_setup_info;

typedef struct {
    int   order;
    long  rate;
    long  barkmap;

    int   ampbits;
    int   ampdB;

    int   numbooks; /* <= 16 */
    int   books[16];

    float lessthan;     /* encode-only config setting hacks for libvorbis */
    float greaterthan;  /* encode-only config setting hacks for libvorbis */
} vorbis_info_floor0;

typedef struct {
    int ln;
    int  m;
    int **linearmap;
    int  n[2];

    vorbis_info_floor0 *vi;

    long bits;
    long frames;
} vorbis_look_floor0;

extern "C" {
void __fastcall vorbis_lsp_to_curve(float *curve,int *map,int n,int ln,float *lsp,int m,
                                    float amp,float ampoffset);   // 0x00CA4570

static __declspec(noinline) void floor0_map_lazy_init(vorbis_block      *vb,
                                                      vorbis_info_floor *infoX,
                                                      vorbis_look_floor0 *look){
  if(!look->linearmap[vb->W]){
    vorbis_dsp_state   *vd=vb->vd;
    vorbis_info        *vi=vd->vi;
    codec_setup_info   *ci=(codec_setup_info*)vi->codec_setup;
    vorbis_info_floor0 *info=(vorbis_info_floor0 *)infoX;
    int W=vb->W;
    int n=ci->blocksizes[W]/2,j;

    /* we choose a scaling constant so that:
       floor(bark(rate/2-1)*C)=mapped-1
     floor(bark(rate/2)*C)=mapped */
    float scale=look->ln/toBARK(info->rate/2.f);

    /* the mapping from a linear scale to a smaller bark scale is
       straightforward.  We do *not* make sure that the linear mapping
       does not skip bark-scale bins; the decoder simply skips them and
       the encoder may do what it wishes in filling them.  They're
       necessary in some mapping combinations to keep the scale spacing
       accurate */
    look->linearmap[W]=(int*)_ogg_malloc((n+1)*sizeof(**look->linearmap));
    for(j=0;j<n;j++){
      int val=(int)floor( toBARK((info->rate/2.f)/n*j)
                     *scale); /* bark numbers represent band edges */
      if(val>=look->ln)val=look->ln-1; /* guard against the approximation */
      look->linearmap[W][j]=val;
    }
    look->linearmap[W][j]=-1;
    look->n[W]=n;
  }
}

int floor0_inverse2(vorbis_block *vb,vorbis_look_floor *i,
                    void *memo,float *out){
  vorbis_look_floor0 *look=(vorbis_look_floor0 *)i;
  vorbis_info_floor0 *info=look->vi;

  floor0_map_lazy_init(vb,info,look);

  if(memo){
    float *lsp=(float *)memo;
    float amp=lsp[look->m];

    /* take the coefficients back to a spectral envelope curve */
    vorbis_lsp_to_curve(out,
                        look->linearmap[vb->W],
                        look->n[vb->W],
                        look->ln,
                        lsp,look->m,amp,(float)info->ampdB);
    return(1);
  }

  memset(out,0,sizeof(*out)*look->n[vb->W]);
  return(0);
}
}