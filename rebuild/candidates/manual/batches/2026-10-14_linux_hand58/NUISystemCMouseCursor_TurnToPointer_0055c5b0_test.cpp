#include <cstdio>
#include <cstring>
#include <cstdlib>

static unsigned long g_set = 0; static int g_calls = 0;
struct CSprite { virtual ~CSprite() { } virtual void SetGraphicIndex(unsigned long i) { g_set = i; ++g_calls; } unsigned long def; unsigned long GetDefaultGraphicIndex() { return def; } };
struct CMouseCursor : CSprite { void TurnToPointer() { SetGraphicIndex(GetDefaultGraphicIndex()); } };
struct CDerived : CMouseCursor { virtual void SetGraphicIndex(unsigned long i) { g_set = i + 100; ++g_calls; } };
int main()
{
    CMouseCursor c; c.def = 12; c.TurnToPointer(); if (g_set != 12 || g_calls != 1) { std::printf("0055c5b0_TEST FAIL\n"); return 1; }
    CDerived d; d.def = 5; d.TurnToPointer(); if (g_set != 105 || g_calls != 2) { std::printf("0055c5b0_TEST FAIL\n"); return 1; }
    std::printf("0055c5b0_TEST PASS\n");
    return 0;
}
