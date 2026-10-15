#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_base = 0, g_free = 0, g_set = 0; static bool g_freeResult = false; static void* g_freeArg = 0; static const void* g_setArg = 0;
struct CAction { int x; };
struct CCounted { CAction* data; void* info; };
struct CThing { bool IsFreeToPerformAction(CAction* a) { ++g_free; g_freeArg = a; return g_freeResult; } void SetCurrentAction(const CCounted& c) { ++g_set; g_setArg = &c; } };
struct CModeBase { virtual ~CModeBase() { } virtual void FrameUpdate() { ++g_base; } CThing* thing; };

struct CMode : CModeBase { unsigned char pad[0x1c]; CCounted pending; virtual void FrameUpdate() { CModeBase::FrameUpdate(); if (pending.data) { CThing* t = thing; if (t->IsFreeToPerformAction(pending.data)) t->SetCurrentAction(pending); } } };
int main()
{
    CThing t; CMode m; m.thing = &t; m.pending.data = 0; m.pending.info = 0;
    m.FrameUpdate(); if (g_base != 1 || g_free != 0 || g_set != 0) { std::printf("008d6c80_TEST FAIL\n"); return 1; }
    CAction act; m.pending.data = &act; m.FrameUpdate(); if (g_base != 2 || g_free != 1 || g_freeArg != &act || g_set != 0) { std::printf("008d6c80_TEST FAIL\n"); return 1; }
    g_freeResult = true; m.FrameUpdate(); if (g_free != 2 || g_set != 1 || g_setArg != &m.pending) { std::printf("008d6c80_TEST FAIL\n"); return 1; }
    std::printf("008d6c80_TEST PASS\n");
    return 0;
}
