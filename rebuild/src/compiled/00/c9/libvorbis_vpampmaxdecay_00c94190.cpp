// libvorbis::_vp_ampmax_decay(float amp, vorbis_dsp_state* vd) -- decay the running peak amplitude by ampmax_att_per_sec over one block's half length, floored at -9999 dB
// Leaf: _vp_ampmax_decay
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C94190, 68 bytes. libvorbis 1.0.1 lib/psy.c, compiled as C with /Gr:
//   `__fastcall`, vd in ecx (the first integer-sized argument), amp on the stack (`ret 4`). No callees.
// Data: the float -9999.0f @ 0x012BF314.
// Layout: libvorbis 1.0.1 vorbis_dsp_state (+0x04 vi, +0x28 W), vorbis_info (+0x08 rate, +0x1c codec_setup),
//   codec_setup_info (+0x00 blocksizes[2], psy_g_param at +0xb34 whose ampmax_att_per_sec is +0x44).
// Shape: the shipped source; `(float)n / vi->rate` is `fild` of the frame word + `fidiv`; the clamp keeps amp
//   in st0 (`fcom ; jp` then `fstp st0 ; fld [-9999]`).
typedef __int64 ogg_int64_t;
#define VE_BANDS 7
#define PACKETBLOBS 15

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

typedef struct vorbis_info_psy_global {
    int   eighth_octave_lines;
    float preecho_thresh[VE_BANDS];
    float postecho_thresh[VE_BANDS];
    float stretch_penalty;
    float preecho_minenergy;
    float ampmax_att_per_sec;
    int   coupling_pkHz[PACKETBLOBS];
    int   coupling_pointlimit[2][PACKETBLOBS];
    int   coupling_prepointamp[PACKETBLOBS];
    int   coupling_postpointamp[PACKETBLOBS];
    int   sliding_lowpass[2][PACKETBLOBS];
} vorbis_info_psy_global;

typedef struct codec_setup_info {
    long  blocksizes[2];
    int   modes;
    int   maps;
    int   floors;
    int   residues;
    int   books;
    int   psys;
    void* mode_param[64];
    int   map_type[64];
    void* map_param[64];
    int   floor_type[64];
    void* floor_param[64];
    int   residue_type[64];
    void* residue_param[64];
    void* book_param[256];
    void* fullbooks;
    void* psy_param[4];
    vorbis_info_psy_global psy_g_param;
} codec_setup_info;

extern "C" float __fastcall _vp_ampmax_decay(float amp, vorbis_dsp_state* vd)
{
    vorbis_info* vi = vd->vi;
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    vorbis_info_psy_global* gi = &ci->psy_g_param;

    int n = ci->blocksizes[vd->W] / 2;
    float secs = (float)n / vi->rate;

    amp += secs * gi->ampmax_att_per_sec;
    if (amp < -9999) amp = -9999;
    return amp;
}