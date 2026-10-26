#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_base = 0; static unsigned long g_old = 0x55; static long g_typeArg = -1;
class CThing { public: unsigned char pad[0x6c]; unsigned char f6c; unsigned char pad2[0x15b]; unsigned char f1c8; };
unsigned long GetOld(CThing* t, long type) { g_typeArg = type; return g_old; }
struct CModeBase { virtual ~CModeBase() { } CThing* thing; };
struct CModeStandBase : CModeBase { void OnActivate() { ++g_base; } };
struct CMode : CModeStandBase { unsigned long old; virtual void OnActivate() { CModeStandBase::OnActivate(); old = GetOld(thing, 0); if (thing->f6c & 0x10) thing->f1c8 |= 0x40; } };
int main()
{
    CThing t; std::memset(&t, 0, sizeof t); t.f6c = 0x10; t.f1c8 = 1; CMode m; m.thing = &t; m.old = 0;
    m.OnActivate(); if (g_base != 1 || g_typeArg != 0 || m.old != 0x55 || t.f1c8 != 0x41) { std::printf("008d66e0_TEST FAIL\n"); return 1; }
    t.f6c = 0; t.f1c8 = 1; m.OnActivate(); if (t.f1c8 != 1) { std::printf("008d66e0_TEST FAIL\n"); return 1; }
    std::printf("008d66e0_TEST PASS\n");
    return 0;
}