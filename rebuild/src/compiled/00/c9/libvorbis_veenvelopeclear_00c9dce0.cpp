// libvorbis::_ve_envelope_clear(envelope_lookup* e) -- release an envelope detector: clear its MDCT, free the band windows, the MDCT window, the filter states and the mark array, then wipe it
// Leaf: @_ve_envelope_clear@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9DCE0, 87 bytes, e in ecx, plain `ret`. libvorbis 1.0.1 lib/envelope.c.
//   Callees: mdct_clear 0x00C9C4C0 (landed, agent447) on &e->mdct (+0x10), free (MSVCR71 import [0x01440154], hoisted
//   into ebx) x10. Caller: vorbis_dsp_clear's backend teardown 0x00C8F685.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): Xiph libvorbis 1.0.1 inside libpk
//   ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240), compiled as C with /Gr (`__fastcall`); the free is the MSVCR71
//   import [0x01440154] through _ogg_free, as in the landed agent447 rows (mdct_clear 0x00C9C4C0, res0_free_look 0x00C9FDF0).
// Label: catalog "SBufferRecord00c9dce0::FreeBuffers" / manifest "CRenderManager2D::SetupD3DUVBoxFromGraphic" are
//   descriptive/propagated (its "table buffers" are the seven envelope_band windows at +0x30 step 0x10, "m_pAux" of the
//   last entry is filter +0x98, "m_pBlockA0" is mark).
// Shape: the shipped source; the band loop is a pointer walk with a down-counter, the three loose frees share one
//   `add esp,0Ch`, the 0xB4-byte memset is `rep stosd` x0x2d.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef struct mdct_lookup {                       // libvorbis 1.0.1 mdct.h, 0x14 bytes
    int    n;
    int    log2n;
    float* trig;
    int*   bitrev;
    float  scale;
} mdct_lookup;
#define VE_PRE    16
#define VE_POST   2
#define VE_AMP    (VE_PRE + VE_POST - 1)
#define VE_BANDS  7
#define VE_NEARDC 15
typedef struct {
    float ampbuf[VE_AMP];
    int   ampptr;
    float nearDC[VE_NEARDC];
    float nearDC_acc;
    float nearDC_partialacc;
    int   nearptr;
} envelope_filter_state;
typedef struct {
    int    begin;
    int    end;
    float* window;
    float  total;
} envelope_band;
typedef struct {                                   // libvorbis 1.0.1 envelope.h, 0xB4 bytes
    int                    ch;                     // +0x00
    int                    winlength;
    int                    searchstep;
    float                  minenergy;
    mdct_lookup            mdct;                   // +0x10
    float*                 mdct_win;               // +0x24
    envelope_band          band[VE_BANDS];         // +0x28
    envelope_filter_state* filter;                 // +0x98
    int                    stretch;
    int*                   mark;                   // +0xa0
    long                   storage;
    long                   current;
    long                   curmark;
    long                   cursor;
} envelope_lookup;

extern "C" void __fastcall mdct_clear(mdct_lookup* l);   // 0x00C9C4C0

extern "C" void __fastcall _ve_envelope_clear(envelope_lookup* e)
{
    int i;
    mdct_clear(&e->mdct);
    for (i = 0; i < VE_BANDS; i++)
        _ogg_free(e->band[i].window);
    _ogg_free(e->mdct_win);
    _ogg_free(e->filter);
    _ogg_free(e->mark);
    memset(e, 0, sizeof(*e));
}
