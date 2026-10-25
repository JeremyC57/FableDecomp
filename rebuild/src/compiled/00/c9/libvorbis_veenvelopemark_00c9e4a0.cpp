// libvorbis::_ve_envelope_mark(vorbis_dsp_state* v) -- 1 when the envelope detector marked a transient inside the current block's span (curmark or any search-step mark), else 0
// Leaf: _ve_envelope_mark
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9E4A0, 177 bytes. libvorbis 1.0.1 lib/envelope.c, compiled as C with
//   /Gr: `__fastcall`, v in ecx, plain `ret`. No callees.
// Layout: libvorbis 1.0.1 vorbis_dsp_state (+0x04 vi, +0x24 lW, +0x28 W, +0x2c nW, +0x30 centerW, +0x68
//   backend_state), private_state (+0 ve), codec_setup_info (+0 blocksizes[2]) and envelope_lookup
//   (+0x08 searchstep, +0xa0 mark, +0xac curmark).
// Shape: the shipped source; ve is fetched first into the `push ecx` frame slot; each `blocksizes[x]/4` is a
//   signed divide (`cdq ; and edx,3 ; sar 2`); the two `return 1` exits merge into one block after `return 0`.
typedef __int64 ogg_int64_t;
#define VE_BANDS 7
#define VE_PRE 16
#define VE_POST 2
#define VE_AMP (VE_PRE + VE_POST - 1)
#define VE_NEARDC 15

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

typedef struct codec_setup_info {
    long blocksizes[2];
    int  modes;
    int  maps;
    int  floors;
    int  residues;
    int  books;
    int  psys;
} codec_setup_info;

typedef struct mdct_lookup {
    int    n;
    int    log2n;
    float* trig;
    int*   bitrev;
    float  scale;
} mdct_lookup;

typedef struct envelope_filter_state {
    float ampbuf[VE_AMP];
    int   ampptr;
    float nearDC[VE_NEARDC];
    float nearDC_acc;
    float nearDC_partialacc;
    int   nearptr;
} envelope_filter_state;

typedef struct envelope_band {
    int    begin;
    int    end;
    float* window;
    float  total;
} envelope_band;

typedef struct envelope_lookup {
    int    ch;
    int    winlength;
    int    searchstep;
    float  minenergy;
    mdct_lookup mdct;
    float* mdct_win;
    envelope_band band[VE_BANDS];
    envelope_filter_state* filter;
    int    stretch;
    int*   mark;
    long   storage;
    long   current;
    long   curmark;
    long   cursor;
} envelope_lookup;

typedef struct drft_lookup {
    int    n;
    float* trigcache;
    int*   splitcache;
} drft_lookup;

typedef struct private_state {
    envelope_lookup* ve;
    int              window[2];
    void**           transform[2];
    drft_lookup      fft_look[2];
    int              modebits;
    void**           flr;
    void**           residue;
    void*            psy;
    void*            psy_g_look;
    unsigned char*   header;
    unsigned char*   header1;
    unsigned char*   header2;
} private_state;

extern "C" int __fastcall _ve_envelope_mark(vorbis_dsp_state* v)
{
    envelope_lookup* ve = ((private_state*)(v->backend_state))->ve;
    vorbis_info* vi = v->vi;
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    long centerW = v->centerW;
    long beginW = centerW - ci->blocksizes[v->W] / 4;
    long endW = centerW + ci->blocksizes[v->W] / 4;
    if (v->W) {
        beginW -= ci->blocksizes[v->lW] / 4;
        endW += ci->blocksizes[v->nW] / 4;
    } else {
        beginW -= ci->blocksizes[0] / 4;
        endW += ci->blocksizes[0] / 4;
    }

    if (ve->curmark >= beginW && ve->curmark < endW) return 1;
    {
        long first = beginW / ve->searchstep;
        long last = endW / ve->searchstep;
        long i;
        for (i = first; i < last; i++)
            if (ve->mark[i]) return 1;
    }
    return 0;
}