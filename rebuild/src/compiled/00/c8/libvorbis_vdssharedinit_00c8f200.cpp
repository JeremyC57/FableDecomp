// libvorbis::_vds_shared_init(vorbis_dsp_state *v, vorbis_info *vi, int encp) -- libvorbis block.c: clear the DSP state, allocate the private backend (MDCT lookups, window shapes, encoder FFTs/psychoacoustics or decoder codebooks), the PCM buffers and the floor/residue lookups
// Shape (retry1): parked as an "LTCG custom register arguments" gap; the TU holds block.c's static _vds_shared_init as `extern "C" { static __declspec(noinline) int __stdcall }` with its two real callers vorbis_analysis_init / vorbis_synthesis_init, and VC7.1 /O2 picks retail's private convention (v in EBX, vi/encp on the stack, ret 8) itself.
// Leaf: __vds_shared_init@12
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8F200, 1007 bytes (`ret 8`: v in EBX -- an LTCG private register convention --,
//   vi and encp on the stack, callee-popped).
// Identity: libvorbis 1.0.1+CVS lib/block.c `static int _vds_shared_init` ("Xiph.Org libVorbis I 20030909" @0x012BF240);
//   the two callers 0x00C8F5F9 / 0x00C8FF06 are vorbis_analysis_init / vorbis_synthesis_init (encp 1 / 0); the
//   halfrate_flag read (+0xE78, `sar edx,cl` on both mdct_init sizes) is the post-1.0.1 CVS halfrate support. The
//   manifest label AudioSystem_Initialize is propagated and wrong.
// Callees: ilog2 0x00C8F060, mdct_init 0x00C9BCD0, drft_init 0x00C9BC50, vorbis_book_init_encode 0x00C954A0,
//   vorbis_book_init_decode 0x00C95580, vorbis_staticbook_destroy 0x00C95420, _vp_psy_init 0x00C92660 (all LTCG
//   __fastcall), calloc / malloc through the MSVCR71 IAT (0x01440058 / 0x01440158), _floor_P 0x013A3DC8,
//   _residue_P 0x013A3DD0 (look() is __fastcall: v in ecx, param in edx).
#include <string.h>
extern "C" {
__declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);
__declspec(dllimport) void* __cdecl malloc(unsigned int size);
typedef struct vorbis_info {
    int version;                                        // +0x00
    int channels;                                       // +0x04
    long rate;                                          // +0x08
    long bitrate_upper, bitrate_nominal, bitrate_lower, bitrate_window;
    void* codec_setup;                                  // +0x1c
} vorbis_info;
typedef struct vorbis_dsp_state {
    int analysisp;                                      // +0x00
    vorbis_info* vi;                                    // +0x04
    float** pcm;                                        // +0x08
    float** pcmret;                                     // +0x0c
    int pcm_storage;                                    // +0x10
    int pcm_current;                                    // +0x14
    int pcm_returned;                                   // +0x18
    int preextrapolate;                                 // +0x1c
    int eofflag;                                        // +0x20
    long lW;                                            // +0x24
    long W;                                             // +0x28
    long nW;                                            // +0x2c
    long centerW;                                       // +0x30
    __int64 granulepos;                                 // +0x38
    __int64 sequence;                                   // +0x40
    __int64 glue_bits, time_bits, floor_bits, res_bits; // +0x48
    void* backend_state;                                // +0x68
} vorbis_dsp_state;
typedef void vorbis_info_floor;
typedef void vorbis_info_residue;
typedef void vorbis_look_floor;
typedef void vorbis_look_residue;
typedef struct static_codebook static_codebook;
typedef struct codebook { unsigned char _body[0x2c]; } codebook;
typedef struct vorbis_info_psy { int blockflag; } vorbis_info_psy;
typedef struct vorbis_info_psy_global { int eighth_octave_lines; } vorbis_info_psy_global;
typedef struct vorbis_look_psy { unsigned char _body[0x30]; } vorbis_look_psy;
typedef struct drft_lookup { int n; float* trigcache; int* splitcache; } drft_lookup;
typedef struct mdct_lookup mdct_lookup;
typedef struct codec_setup_info {
    long blocksizes[2];                                 // +0x000
    int modes, maps, floors, residues, books, psys;     // +0x008
    void* mode_param[64];                               // +0x020
    int map_type[64];                                   // +0x120
    void* map_param[64];                                // +0x220
    int floor_type[64];                                 // +0x320
    vorbis_info_floor* floor_param[64];                 // +0x420
    int residue_type[64];                               // +0x520
    vorbis_info_residue* residue_param[64];             // +0x620
    static_codebook* book_param[256];                   // +0x720
    codebook* fullbooks;                                // +0xb20
    vorbis_info_psy* psy_param[4];                      // +0xb24
    vorbis_info_psy_global psy_g_param;                 // +0xb34
    unsigned char _bi_hi[0xe78 - 0xb38];                // bitrate_manager_info bi, highlevel_encode_setup hi
    int halfrate_flag;                                  // +0xe78
} codec_setup_info;
typedef struct private_state {
    void* ve;                                           // +0x00 envelope
    int window[2];                                      // +0x04
    void** transform[2];                                // +0x0c
    drft_lookup fft_look[2];                            // +0x14
    int modebits;                                       // +0x2c
    vorbis_look_floor** flr;                            // +0x30
    vorbis_look_residue** residue;                      // +0x34
    vorbis_look_psy* psy;                               // +0x38
    void* psy_g_look;                                   // +0x3c
    unsigned char* header;                              // +0x40
    unsigned char* header1;                             // +0x44
    unsigned char* header2;                             // +0x48
    unsigned char _pad4c[0x50 - 0x4c];
    unsigned char bms[0xb8 - 0x50];                     // +0x50 bitrate_manager_state (not modelled)
    unsigned char sample_count[8];                      // +0xb8 ogg_int64_t
} private_state;
typedef struct vorbis_func_floor {
    void* pack; void* unpack;
    vorbis_look_floor* (__fastcall* look)(vorbis_dsp_state*, vorbis_info_floor*);
} vorbis_func_floor;
typedef struct vorbis_func_residue {
    void* pack; void* unpack;
    vorbis_look_residue* (__fastcall* look)(vorbis_dsp_state*, vorbis_info_residue*);
} vorbis_func_residue;
extern vorbis_func_floor* _floor_P[];                   // 0x013A3DC8
extern vorbis_func_residue* _residue_P[];               // 0x013A3DD0
int __fastcall ilog2(unsigned int v);                   // 0x00C8F060
void __fastcall mdct_init(mdct_lookup* lookup, int n);  // 0x00C9BCD0
void __fastcall drft_init(drft_lookup* l, int n);       // 0x00C9BC50
int __fastcall vorbis_book_init_encode(codebook* dest, const static_codebook* source);   // 0x00C954A0
int __fastcall vorbis_book_init_decode(codebook* dest, const static_codebook* source);   // 0x00C95580
void __fastcall vorbis_staticbook_destroy(static_codebook* b);                           // 0x00C95420
void __fastcall _vp_psy_init(vorbis_look_psy* p, vorbis_info_psy* vi, vorbis_info_psy_global* gi, int n, long rate);   // 0x00C92660
#define _ogg_calloc calloc
#define _ogg_malloc malloc
#define VI_TRANSFORMB 1

static __declspec(noinline) int __stdcall _vds_shared_init(vorbis_dsp_state* v, vorbis_info* vi, int encp)
{
    int i;
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    private_state* b = 0;
    int hs;

    if (ci == 0) return 1;
    hs = ci->halfrate_flag;

    memset(v, 0, sizeof(*v));
    b = (private_state*)(v->backend_state = _ogg_calloc(1, sizeof(*b)));

    v->vi = vi;
    b->modebits = ilog2(ci->modes);

    b->transform[0] = (void**)_ogg_calloc(VI_TRANSFORMB, sizeof(*b->transform[0]));
    b->transform[1] = (void**)_ogg_calloc(VI_TRANSFORMB, sizeof(*b->transform[1]));

    /* MDCT is tranform 0 */

    b->transform[0][0] = _ogg_calloc(1, 0x14);
    b->transform[1][0] = _ogg_calloc(1, 0x14);
    mdct_init((mdct_lookup*)b->transform[0][0], ci->blocksizes[0] >> hs);
    mdct_init((mdct_lookup*)b->transform[1][0], ci->blocksizes[1] >> hs);

    /* Vorbis I uses only window type 0 */
    b->window[0] = ilog2(ci->blocksizes[0]) - 6;
    b->window[1] = ilog2(ci->blocksizes[1]) - 6;

    if (encp) { /* encode/decode differ here */

        /* analysis always needs an fft */
        drft_init(&b->fft_look[0], ci->blocksizes[0]);
        drft_init(&b->fft_look[1], ci->blocksizes[1]);

        /* finish the codebooks */
        if (!ci->fullbooks) {
            ci->fullbooks = (codebook*)_ogg_calloc(ci->books, sizeof(*ci->fullbooks));
            for (i = 0; i < ci->books; i++)
                vorbis_book_init_encode(ci->fullbooks + i, ci->book_param[i]);
        }

        b->psy = (vorbis_look_psy*)_ogg_calloc(ci->psys, sizeof(*b->psy));
        for (i = 0; i < ci->psys; i++) {
            _vp_psy_init(b->psy + i,
                         ci->psy_param[i],
                         &ci->psy_g_param,
                         ci->blocksizes[ci->psy_param[i]->blockflag] / 2,
                         vi->rate);
        }

        v->analysisp = 1;
    } else {
        /* finish the codebooks */
        if (!ci->fullbooks) {
            ci->fullbooks = (codebook*)_ogg_calloc(ci->books, sizeof(*ci->fullbooks));
            for (i = 0; i < ci->books; i++) {
                vorbis_book_init_decode(ci->fullbooks + i, ci->book_param[i]);
                /* decode codebooks are now standalone after init */
                vorbis_staticbook_destroy(ci->book_param[i]);
                ci->book_param[i] = 0;
            }
        }
    }

    /* initialize the storage vectors. blocksize[1] is small for encode,
       but the correct size for decode */
    v->pcm_storage = ci->blocksizes[1];
    v->pcm = (float**)_ogg_malloc(vi->channels * sizeof(*v->pcm));
    v->pcmret = (float**)_ogg_malloc(vi->channels * sizeof(*v->pcmret));
    {
        int i;
        for (i = 0; i < vi->channels; i++)
            v->pcm[i] = (float*)_ogg_calloc(v->pcm_storage, sizeof(*v->pcm[i]));
    }

    /* all 1 (large block) or 0 (small block) */
    /* explicitly set for the sake of clarity */
    v->lW = 0; /* previous window size */
    v->W = 0;  /* current window size */

    /* all vector indexes */
    v->centerW = ci->blocksizes[1] / 2;

    v->pcm_current = v->centerW;

    /* initialize all the backend lookups */
    b->flr = (vorbis_look_floor**)_ogg_calloc(ci->floors, sizeof(*b->flr));
    b->residue = (vorbis_look_residue**)_ogg_calloc(ci->residues, sizeof(*b->residue));

    for (i = 0; i < ci->floors; i++)
        b->flr[i] = _floor_P[ci->floor_type[i]]->look(v, ci->floor_param[i]);

    for (i = 0; i < ci->residues; i++)
        b->residue[i] = _residue_P[ci->residue_type[i]]->look(v, ci->residue_param[i]);

    return 0;
}

void* __fastcall _vp_global_look(vorbis_info* vi);                    // 0x00C91820
void __fastcall _ve_envelope_init(void* e, vorbis_info* vi);          // 0x00C9DB40
void __fastcall vorbis_bitrate_init(vorbis_info* vi, void* bm);       // 0x00C9CD20
int __fastcall vorbis_synthesis_restart(vorbis_dsp_state* v);         // 0x00C8FEA0

// vorbis_analysis_init 0x00C8F5F0 (block.c, LTCG-API __fastcall)
int __fastcall vorbis_analysis_init(vorbis_dsp_state* v, vorbis_info* vi)
{
    private_state* b = 0;

    if (_vds_shared_init(v, vi, 1)) return 1;
    b = (private_state*)v->backend_state;
    b->psy_g_look = _vp_global_look(vi);

    /* Initialize the envelope state storage */
    b->ve = calloc(1, 0xb4);
    _ve_envelope_init(b->ve, vi);

    vorbis_bitrate_init(vi, &b->bms);

    return (0);
}

// vorbis_synthesis_init 0x00C8FF00 (block.c, LTCG-API __fastcall)
int __fastcall vorbis_synthesis_init(vorbis_dsp_state* v, vorbis_info* vi)
{
    if (_vds_shared_init(v, vi, 0)) return 1;
    vorbis_synthesis_restart(v);

    return 0;
}
}