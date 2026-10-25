// libvorbis::vorbis_synthesis_pcmout(vorbis_dsp_state* v, float*** pcm) -- hand the caller per-channel pointers to the decoded, not yet returned PCM and return how many samples are ready
// Leaf: vorbis_synthesis_pcmout
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C908E0, 80 bytes. libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909",
//   string @ 0x00EBF240), lib/block.c. The library is compiled as C with /Gr: `__fastcall`, v in ecx, pcm in edx,
//   plain `ret`. No callees.
// Layout: libvorbis 1.0.1 codec.h vorbis_dsp_state (+0x04 vi, +0x08 pcm, +0x0c pcmret, +0x14 pcm_current,
//   +0x18 pcm_returned) and vorbis_info (+0x04 channels).
// Shape: the source as shipped -- `vi` is read before the range test (`mov esi,[ecx+4]` between the cmp and
//   its jle); the loop re-reads vi->channels, v->pcm, v->pcm_returned and v->pcmret every pass (no aliasing
//   guarantee for the float** stores); the early `return 0` sits after the main return.
typedef __int64 ogg_int64_t;
typedef struct vorbis_info {
    int   version;
    int   channels;
    long  rate;
    long  bitrate_upper;
    long  bitrate_nominal;
    long  bitrate_lower;
    long  bitrate_window;
    void* codec_setup;
} vorbis_info;
typedef struct vorbis_dsp_state {
    int          analysisp;
    vorbis_info* vi;
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

extern "C" int __fastcall vorbis_synthesis_pcmout(vorbis_dsp_state* v, float*** pcm)
{
    vorbis_info* vi = v->vi;
    if (v->pcm_returned > -1 && v->pcm_returned < v->pcm_current) {
        if (pcm) {
            int i;
            for (i = 0; i < vi->channels; i++)
                v->pcmret[i] = v->pcm[i] + v->pcm_returned;
            *pcm = v->pcmret;
        }
        return v->pcm_current - v->pcm_returned;
    }
    return 0;
}