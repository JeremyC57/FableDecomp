// Behaviour test for 0x009E20F0 (source body copied from src/009e20f0.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
struct C2DExtentsI
{
    long X;
    long Y;
};

class C2DBoxI
{
public:
    long TLX;
    long TLY;
    long BRX;
    long BRY;
};

class CDisplayManager
{
public:
    C2DExtentsI GetRenderTargetDimensions() const;     // 0x009BEDC0
};

class CSystemManager
{
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* m_pDisplayManager;                // +0x60
};

CSystemManager* __fastcall GFGetSystemManager();       // 0x009A4EC0
extern double g_VirtualWidth;                          // 0x013961E8 (65536.0)
extern double g_VirtualHeight;                         // 0x013961F0 (65536.0)

inline long ScaleToVirtual(double virtualSize, long x, const C2DExtentsI& size)
{
    return (long)((float)virtualSize * x / size.X);
}

inline long __fastcall GFAXToVX(long x)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return ScaleToVirtual(g_VirtualWidth, x, display->GetRenderTargetDimensions());
}

inline long __fastcall GFAYToVY(long y)
{
    CSystemManager* system = GFGetSystemManager();
    float scaled = (float)g_VirtualHeight * y;
    CDisplayManager* display = system->m_pDisplayManager;
    return (long)(scaled / display->GetRenderTargetDimensions().Y);
}

void __fastcall GFABoxToVBox(const C2DBoxI& a, C2DBoxI& v)
{
    v.TLX = GFAXToVX(a.TLX);
    v.TLY = GFAYToVY(a.TLY);
    v.BRX = GFAXToVX(a.BRX);
    v.BRY = GFAYToVY(a.BRY);
}

// ---- test harness ----
double g_VirtualWidth = 65536.0;
double g_VirtualHeight = 65536.0;
static CDisplayManager g_display;
static CSystemManager g_system;
static int g_systemCalls = 0, g_dimensionCalls = 0;
CSystemManager* __fastcall GFGetSystemManager() { ++g_systemCalls; g_system.m_pDisplayManager = &g_display; return &g_system; }
C2DExtentsI CDisplayManager::GetRenderTargetDimensions() const { ++g_dimensionCalls; C2DExtentsI e; e.X = 800; e.Y = 600; return e; }
int main()
{
    C2DBoxI a, v;
    a.TLX = 100; a.TLY = 150; a.BRX = 700; a.BRY = 599;
    GFABoxToVBox(a, v);
    int ok = 1;
    // 100*65536/800 = 8192, 150*65536/600 = 16384, 700*65536/800 = 57344, 599*65536/600 = 65426.77 -> 65426
    if (v.TLX != 8192 || v.TLY != 16384 || v.BRX != 57344 || v.BRY != 65426) ok = 0;
    if (g_systemCalls != 4 || g_dimensionCalls != 4) ok = 0;
    printf("009e20f0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}