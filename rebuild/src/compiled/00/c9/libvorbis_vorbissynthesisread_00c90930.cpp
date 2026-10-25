// libvorbis::vorbis_synthesis_read(vorbis_dsp_state* v, int n) -- mark n decoded samples as consumed; OV_EINVAL (-131) when more are claimed than are ready
// Leaf: vorbis_synthesis_read
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C90930, 30 bytes. libvorbis 1.0.1 lib/block.c, compiled as C with /Gr:
//   `__fastcall`, v in ecx, n in edx, plain `ret`. No callees.
// Layout: libvorbis 1.0.1 vorbis_dsp_state (+0x14 pcm_current, +0x18 pcm_returned).
// Shape: the shipped source; the error return (`mov eax,0FFFFFF7Dh`) is the fall-through of the range test.
typedef __int64 ogg_int64_t;
typedef struct vorbis_dsp_state {
    int          analysisp;
    void*        vi;
    float**      pcm;
    float**      pcmret;
    int          pcm_storage;
    int          pcm_current;
    int          pcm_returned;
    int          preextrapolate;
    int          eofflag;
    long         lW;
    long         W;
    long         nW;
    long         centerW;
    ogg_int64_t  granulepos;
    ogg_int64_t  sequence;
    ogg_int64_t  glue_bits;
    ogg_int64_t  time_bits;
    ogg_int64_t  floor_bits;
    ogg_int64_t  res_bits;
    void*        backend_state;
} vorbis_dsp_state;

#define OV_EINVAL -131

extern "C" int __fastcall vorbis_synthesis_read(vorbis_dsp_state* v, int n)
{
    if (n && v->pcm_returned + n > v->pcm_current) return OV_EINVAL;
    v->pcm_returned += n;
    return 0;
}