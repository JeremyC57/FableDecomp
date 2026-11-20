// Behaviour test for 0x009BE340 CDisplayManager::ReadDeviceRefreshRate (links the REAL row object; the CGenericException throw goes
// through the CRT and is caught here by type).
// Fixtures (raw native layout): a 0x2CC-byte CDisplayManager image whose only field the row reads is D3D +0x54; a fake IDirect3D9
//   whose vtable has slot 8 (+0x20) GetAdapterDisplayMode and traps on every other slot.
// Reference model (independent): GetAdapterDisplayMode is called exactly once with the D3D object, adapter 0 and a mode buffer; a
//   negative HRESULT throws CGenericException, any other returns the mode's RefreshRate; the manager image is not written.
// Checks: hand-written HRESULT / rate cases (0, 1, 0x7FFFFFFF, -1, E_FAIL, 0x80000000, D3DERR_INVALIDCALL) + 4000 seeded random cases.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct _D3DDISPLAYMODE { unsigned int Width, Height, RefreshRate; int Format; };
class CExceptionBase { };
class CGenericException : public CExceptionBase { };
class CDisplayManager
{
public:
    static long Call(void* m) { return ((CDisplayManager*)m)->ReadDeviceRefreshRate(); }
private:
    long ReadDeviceRefreshRate();
};

typedef unsigned long UL;
static unsigned char g_obj[0x2CC], g_ref[0x2CC];
static void* g_vtbl[17];
static struct { void** vtbl; UL guard; } g_d3d;
static struct { long hr; _D3DDISPLAYMODE mode; } g_c;
static int g_calls, g_bad, g_fail, g_cases;

static void __stdcall Trap() { printf("009be340_TEST FAIL (unexpected IDirect3D9 slot)\n"); exit(1); }
static long __stdcall GetAdapterDisplayMode(void* self, unsigned int adapter, _D3DDISPLAYMODE* mode)
{
    ++g_calls;
    if (self != (void*)&g_d3d || adapter != 0 || mode == 0) { ++g_bad; return g_c.hr; }
    *mode = g_c.mode;
    return g_c.hr;
}

static UL g_seed = 0x9BE340u;
static UL Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }

static void Run(long hr, unsigned int width, unsigned int height, unsigned int rate, int format)
{
    ++g_cases;
    g_c.hr = hr; g_c.mode.Width = width; g_c.mode.Height = height; g_c.mode.RefreshRate = rate; g_c.mode.Format = format;
    for (int i = 0; i < 0x2CC; ++i) g_obj[i] = (unsigned char)Rnd();
    *(void**)(g_obj + 0x54) = &g_d3d;
    memcpy(g_ref, g_obj, sizeof(g_obj));
    g_calls = 0; g_bad = 0;
    bool thrown = false; long got = 0x5A5A5A5A;
    try { got = CDisplayManager::Call(g_obj); }
    catch (CGenericException&) { thrown = true; }
    bool want_throw = hr < 0;
    bool ok = g_calls == 1 && g_bad == 0 && thrown == want_throw && (want_throw ? got == 0x5A5A5A5A : got == (long)rate)
              && memcmp(g_ref, g_obj, sizeof(g_obj)) == 0 && g_d3d.guard == 0xC0FFEEu;
    if (!ok) { if (g_fail < 5) printf("  case %d: hr %08lX rate %u -> got %ld thrown %d calls %d bad %d\n", g_cases, (UL)hr, rate, got, (int)thrown, g_calls, g_bad); ++g_fail; }
}

int main()
{
    for (int i = 0; i < 17; ++i) g_vtbl[i] = (void*)Trap;
    g_vtbl[8] = (void*)GetAdapterDisplayMode;
    g_d3d.vtbl = g_vtbl; g_d3d.guard = 0xC0FFEEu;
    static const long hrs[] = { 0, 1, 0x7FFFFFFF, -1, (long)0x80004005, (long)0x80000000, (long)0x8876086C };
    static const unsigned int rates[] = { 0, 1, 60, 75, 85, 0x7FFFFFFF, 0x80000001u, 0xFFFFFFFFu };
    for (int a = 0; a < 7; ++a)
        for (int b = 0; b < 8; ++b)
            Run(hrs[a], 640 + (unsigned)a, 480 + (unsigned)b, rates[b], 22);
    for (int n = 0; n < 4000; ++n)
    {
        long hr = (Rnd() & 3) == 0 ? (long)(Rnd() << 8 | Rnd()) : (long)(Rnd() & 1 ? 0 : (Rnd() & 0xFFFF));
        if ((Rnd() & 7) == 0) hr |= (long)0x80000000;
        Run(hr, Rnd(), Rnd(), (Rnd() << 8) ^ Rnd(), (int)(Rnd() & 0xFF));
    }
    if (g_fail) { printf("009be340_TEST FAIL (%d of %d cases)\n", g_fail, g_cases); return 1; }
    printf("009be340_TEST PASS (%d cases)\n", g_cases);
    return 0;
}