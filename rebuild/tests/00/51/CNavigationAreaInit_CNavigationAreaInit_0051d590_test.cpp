// Behaviour test for 0x0051D590 CNavigationAreaInit::~CNavigationAreaInit (implicit): NavRegionSeeds' block, then
// DetailedAreas' block, then ~CNavigationAreaUpdate on the base.
#include <cstdio>
#include <cstdlib>
static const void* g_log[8]; static int g_n = 0; static int g_baseAt = -1;
static void Free(void* p) { if (p) { g_log[g_n++] = p; std::free(p); } }
template <class T> struct Vec { T* start; T* finish; T* eos; Vec() : start(0), finish(0), eos(0) { } ~Vec() { if (start != 0) Free(start); } };
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CNavigationPosition { public: float X, Y; long Layer; };
class CNavigationAreaUpdate { public: ~CNavigationAreaUpdate() { g_baseAt = g_n; } int vectors[12]; };
class CNavigationAreaInit : public CNavigationAreaUpdate { public: C2DBoxI WorldArea; long MapIndex; Vec<C2DBoxF> DetailedAreas; Vec<CNavigationPosition> NavRegionSeeds; };
int main()
{
    CNavigationAreaInit* a = new CNavigationAreaInit;
    a->DetailedAreas.start = (C2DBoxF*)std::malloc(sizeof(C2DBoxF));
    a->NavRegionSeeds.start = (CNavigationPosition*)std::malloc(sizeof(CNavigationPosition));
    const void* d = a->DetailedAreas.start; const void* n = a->NavRegionSeeds.start;
    delete a;
    if (g_n != 2 || g_log[0] != n || g_log[1] != d || g_baseAt != 2) { std::printf("0051d590_TEST FAIL\n"); return 1; }
    std::printf("0051d590_TEST PASS\n");
    return 0;
}