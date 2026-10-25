// libvorbis::_ve_envelope_shift(envelope_lookup* e, long shift) -- slide the envelope detector's mark array and its sample cursors back by `shift` samples
// Leaf: _ve_envelope_shift
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9E560, 109 bytes. libvorbis 1.0.1 lib/envelope.c, compiled as C with
//   /Gr: `__fastcall`, e in ecx, shift in edx, plain `ret`.
// Callees: memmove (MSVCR71 import [0x01440144]).
// Layout: libvorbis 1.0.1 envelope_lookup (+0x08 searchstep, +0xa0 mark, +0xa8 current, +0xac curmark,
//   +0xb0 cursor). VE_POST 2.
// Shape: the shipped source; smallshift is divided first, `(smallsize - smallshift) * 4` folds VE_POST into
//   `lea eax,[eax*4+8]`; curmark is only moved when non-negative.
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // [0x01440144]

#define VE_BANDS 7
#define VE_PRE 16
#define VE_POST 2
#define VE_AMP (VE_PRE + VE_POST - 1)
#define VE_NEARDC 15

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

extern "C" void __fastcall _ve_envelope_shift(envelope_lookup* e, long shift)
{
    int smallsize = e->current / e->searchstep + VE_POST; /* adjust for placing marks ahead of ve->current */
    int smallshift = shift / e->searchstep;

    memmove(e->mark, e->mark + smallshift, (smallsize - smallshift) * sizeof(*e->mark));

    e->current -= shift;
    if (e->curmark >= 0)
        e->curmark -= shift;
    e->cursor -= shift;
}