// Behaviour test for 0x009E2240 (source body copied from src/009e2240.cpp; out-of-line callees stubbed below).
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

class C2DCoordI
{
public:
    long X;
    long Y;
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

inline float ScaleToVirtual(double virtualSize, float x, const C2DExtentsI& size)
{
    return (float)virtualSize * x / size.X;
}

inline float __fastcall GFAXToVX(float x)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    return ScaleToVirtual(g_VirtualWidth, x, display->GetRenderTargetDimensions());
}

inline float __fastcall GFAYToVY(float y)
{
    CSystemManager* system = GFGetSystemManager();
    y *= (float)g_VirtualHeight;
    CDisplayManager* display = system->m_pDisplayManager;
    return y / display->GetRenderTargetDimensions().Y;
}

void __fastcall GFACoordToVCoord(const C2DCoordI& a, C2DCoordI& v)
{
    v.X = (long)GFAXToVX((float)a.X);
    v.Y = (long)GFAYToVY((float)a.Y);
}

// ---- test harness ----
double g_VirtualWidth = 65536.0;
double g_VirtualHeight = 65536.0;
static CDisplayManager g_display;
static CSystemManager g_system;
static int g_systemCalls = 0, g_dimensionCalls = 0;
CSystemManager* __fastcall GFGetSystemManager() { ++g_systemCalls; g_system.m_pDisplayManager = &g_display; return &g_system; }
C2DExtentsI CDisplayManager::GetRenderTargetDimensions() const { ++g_dimensionCalls; C2DExtentsI e; e.X = 640; e.Y = 480; return e; }
int main()
{
    C2DCoordI a, v;
    a.X = 320; a.Y = 120;
    GFACoordToVCoord(a, v);
    int ok = 1;
    if (v.X != 32768 || v.Y != 16384) ok = 0;            // 320 * 65536 / 640, 120 * 65536 / 480
    if (g_systemCalls != 2 || g_dimensionCalls != 2) ok = 0;
    a.X = 641; a.Y = 1;                                     // truncation: 65638.4 -> 65638, 136.53 -> 136
    GFACoordToVCoord(a, v);
    if (v.X != 65638 || v.Y != 136) ok = 0;
    printf("009e2240_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}