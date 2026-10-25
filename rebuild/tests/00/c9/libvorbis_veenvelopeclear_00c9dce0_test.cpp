// Behaviour test for 00c9dce0: libvorbis _ve_envelope_clear -- the MDCT cleared, all seven band windows + mdct_win + filter + mark freed, the 0xB4-byte lookup wiped
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
static int g_freed, g_mdct; static void* g_seen[16];
static void my_free(void* p) { if (g_freed < 16) g_seen[g_freed] = p; ++g_freed; std::free(p); }
static void mdct_clear(mdct_lookup* l) { ++g_mdct; std::memset(l, 0, sizeof *l); }
static void _ve_envelope_clear(envelope_lookup* e)
{
    int i;
    mdct_clear(&e->mdct);
    for (i = 0; i < VE_BANDS; i++) my_free(e->band[i].window);
    my_free(e->mdct_win); my_free(e->filter); my_free(e->mark);
    std::memset(e, 0, sizeof(*e));
}
int main()
{
    envelope_lookup e; std::memset(&e, 0x33, sizeof e);
    void* w[7]; for (int i = 0; i < 7; ++i) e.band[i].window = (float*)(w[i] = std::malloc(16));
    void* mw = e.mdct_win = (float*)std::malloc(16); void* f = e.filter = (envelope_filter_state*)std::malloc(sizeof(envelope_filter_state)); void* mk = e.mark = (int*)std::malloc(16);
    _ve_envelope_clear(&e);
    int ok = sizeof(envelope_lookup) == 0xB4 && (char*)&e.band[0].window - (char*)&e == 0x30 && (char*)&e.filter - (char*)&e == 0x98 && (char*)&e.mark - (char*)&e == 0xA0;
    ok &= g_mdct == 1 && g_freed == 10 && g_seen[7] == mw && g_seen[8] == f && g_seen[9] == mk;
    for (int i = 0; i < 7; ++i) ok &= g_seen[i] == w[i];
    const unsigned char* b = (const unsigned char*)&e; for (unsigned int k = 0; k < sizeof e; ++k) ok &= b[k] == 0;
    std::printf(ok ? "00c9dce0_TEST PASS\n" : "00c9dce0_TEST FAIL\n");
    return ok ? 0 : 1;
}
