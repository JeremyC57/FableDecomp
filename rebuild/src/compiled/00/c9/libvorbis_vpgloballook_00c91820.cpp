// libvorbis::_vp_global_look(vorbis_info* vi) -- allocate the encoder's global psychoacoustic look: channel count, ampmax primed to -9999 and a pointer to the setup's psy_g_param
// Leaf: _vp_global_look
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C91820, 44 bytes. libvorbis 1.0.1 lib/psy.c, compiled as C with /Gr:
//   `__fastcall`, vi in ecx, plain `ret`.
// Callees: calloc (MSVCR71 import [0x01440058]) through _ogg_calloc.
// Layout: libvorbis 1.0.1 vorbis_info (+0x04 channels, +0x1c codec_setup), codec_setup_info (psy_g_param at
//   +0xb34, after book_param[256], fullbooks and psy_param[4]) and vorbis_look_psy_global (0x24 bytes:
//   ampmax, channels, gi, coupling_pointlimit[2][3]).
// Shape: the shipped source; VC schedules the three stores after the call in reverse (gi, channels, then the
//   -9999.0f immediate 0xC61C3C00).
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // [0x01440058]
#define _ogg_calloc calloc

#define VE_BANDS 7
#define P_NOISECURVES 3
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

typedef struct vorbis_look_psy_global {
    float ampmax;
    int   channels;
    vorbis_info_psy_global* gi;
    int   coupling_pointlimit[2][P_NOISECURVES];
} vorbis_look_psy_global;

extern "C" vorbis_look_psy_global* __fastcall _vp_global_look(vorbis_info* vi)
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    vorbis_info_psy_global* gi = &ci->psy_g_param;
    vorbis_look_psy_global* look = (vorbis_look_psy_global*)_ogg_calloc(1, sizeof(*look));

    look->channels = vi->channels;

    look->ampmax = -9999.f;
    look->gi = gi;
    return look;
}