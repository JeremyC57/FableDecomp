// Behaviour test for 0x00780430 CTCFlammable::GetClosestFirePointTo(const C3DVector&, bool) (links the REAL row object). The class
// declarations come from the shared headers only so that the out-of-line callee defined here gets its retail name; every object is raw
// native-layout storage and every offset used is one the row reads:
//   CTCFlammable  ParentThing +0x04, FirePoints (STLport vector of 0x2C-byte NFire::CFirePoint) start +0x10 / finish +0x14
//   CThing        PTCGraphicAppearance +0x64;  CFirePoint  HelperName +0x00 (passed by reference), HelperIndex +0x04
// The scene: 0..12 fire points whose helper points (integer coordinates, so every squared distance is exact) GetHelperPoint returns
// through its out parameter. Expected: end() for no points and begin() for one (no helper lookups), else one lookup per point in order
// (name/index of that point, flags 0, interpolation 1.0f) and the first point with the strictly smallest squared distance to pos
// (half the scenes copy the nearest helper point to a later point, so the first of two equal distances must win).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include <stdio.h>
#include <string.h>
#pragma comment(linker, "/alternatename:@CallRow@16=?GetClosestFirePointTo@CTCFlammable@@AAEPAVCFirePoint@NFire@@ABVC3DVector@@_N@Z")
// thiscall with two stack arguments (`ret 8`): as __fastcall, ecx = this, edx unused, the two arguments on the stack
extern "C" void* __fastcall CallRow(void* self, int unused_edx, const C3DVector* pos, int unlit_only);

extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall OnFault(void*) { ExitProcess(3); return 0; }

template<class T> static T& At(void* p, int off) { return *(T*)((char*)p + off); }
static unsigned g_seed;
static unsigned Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }

static unsigned char g_tc[0x40], g_thing[0x100], g_app[0x40], g_points[16 * 0x2c];
static float g_helper[16][3];
static int g_calls, g_bad_call;

bool NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& point, float interpolate) const
{
    int k = g_calls++;
    unsigned char* fp = g_points + k * 0x2c;
    if ((const void*)this != (const void*)g_app || (const void*)&name != (const void*)fp || index != At<long>(fp, 4) || flags != 0 || interpolate != 1.0f)
        g_bad_call = 1;
    point.X = g_helper[k][0]; point.Y = g_helper[k][1]; point.Z = g_helper[k][2];
    return (Rnd() & 1) != 0;                                                  // the row ignores it
}

static int Scene(int n, int line)
{
    memset(g_tc, 0xCC, sizeof g_tc); memset(g_thing, 0xCC, sizeof g_thing); memset(g_points, 0xCC, sizeof g_points);
    At<void*>(g_tc, 0x04) = g_thing; At<void*>(g_thing, 0x64) = g_app;
    At<void*>(g_tc, 0x10) = g_points; At<void*>(g_tc, 0x14) = g_points + n * 0x2c; At<void*>(g_tc, 0x18) = g_points + 16 * 0x2c;
    for (int i = 0; i < n; ++i) {
        At<long>(g_points + i * 0x2c, 4) = (long)(Rnd() % 7);
        for (int c = 0; c < 3; ++c) g_helper[i][c] = (float)((int)(Rnd() % 61) - 30);
    }
    C3DVector pos; pos.X = (float)((int)(Rnd() % 61) - 30); pos.Y = (float)((int)(Rnd() % 61) - 30); pos.Z = (float)((int)(Rnd() % 61) - 30);
    if (n > 2 && (Rnd() & 1)) {                                               // a tie on the nearest point, later in the list
        int best = 0; long bestd = -1;
        for (int i = 0; i < n; ++i) {
            long dx = (long)(pos.X - g_helper[i][0]), dy = (long)(pos.Y - g_helper[i][1]), dz = (long)(pos.Z - g_helper[i][2]);
            long d = dx * dx + dy * dy + dz * dz;
            if (bestd < 0 || d < bestd) { best = i; bestd = d; }
        }
        if (best < n - 1) memcpy(g_helper[best + 1 + (int)(Rnd() % (n - 1 - best))], g_helper[best], sizeof g_helper[0]);
    }
    g_calls = 0; g_bad_call = 0;
    void* got = CallRow(g_tc, 0x5A5A5A5A, &pos, (int)(Rnd() & 1));
    void* want; int calls_want;
    if (n == 0) { want = g_points; calls_want = 0; }
    else if (n == 1) { want = g_points; calls_want = 0; }
    else {
        int best = 0; long bestd = 0;
        for (int i = 0; i < n; ++i) {
            long dx = (long)(pos.X - g_helper[i][0]), dy = (long)(pos.Y - g_helper[i][1]), dz = (long)(pos.Z - g_helper[i][2]);
            long d = dx * dx + dy * dy + dz * dz;
            if (i == 0 || d < bestd) { best = i; bestd = d; }
        }
        want = g_points + best * 0x2c; calls_want = n;
    }
    int bad = got != want || g_calls != calls_want || g_bad_call || At<void*>(g_tc, 0x10) != (void*)g_points;
    if (bad) printf("scene failed at line %d (n %d): got %d want %d, calls %d/%d bad_call %d\n", line, n,
                    (int)(((unsigned char*)got - g_points) / 0x2c), (int)(((unsigned char*)want - g_points) / 0x2c), g_calls, calls_want, g_bad_call);
    return bad;
}

int main()
{
    SetUnhandledExceptionFilter((void*)OnFault);
    int fails = 0;
    for (int n = 0; n <= 12; ++n) fails += Scene(n, __LINE__);
    for (g_seed = 7; g_seed < 30000 && fails < 5; ) fails += Scene((int)(Rnd() % 13), __LINE__);
    if (!fails) printf("00780430_TEST PASS\n");
    return fails != 0;
}