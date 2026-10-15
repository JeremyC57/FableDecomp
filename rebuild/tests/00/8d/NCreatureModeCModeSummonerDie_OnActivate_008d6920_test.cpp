#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_base = 0, g_susp = 0; static bool g_suspArg = false; static void* g_suspMode = 0;
struct CModeBase;
struct CThing { unsigned char pad[0x10]; unsigned char type; void SetSuspendAnimation(bool s, CModeBase* m) { ++g_susp; g_suspArg = s; g_suspMode = m; } };
struct CModeBase { virtual ~CModeBase() { } CThing* thing; };
struct CModeStandBase : CModeBase { void OnActivate() { ++g_base; } void OnDeactivate() { ++g_base; } };

static long g_fps = 15;
struct CMode : CModeStandBase { long frames; virtual void OnActivate() { CModeStandBase::OnActivate(); if (thing->type == 1) thing->SetSuspendAnimation(true, 0); frames = (long)(g_fps * 2.0f); } };
int main()
{
    CThing t; t.type = 1; CMode m; m.thing = &t; m.frames = 0;
    m.OnActivate(); if (g_base != 1 || g_susp != 1 || !g_suspArg || g_suspMode != 0 || m.frames != 30) { std::printf("008d6920_TEST FAIL\n"); return 1; }
    t.type = 2; g_fps = 60; m.OnActivate(); if (g_susp != 1 || m.frames != 120) { std::printf("008d6920_TEST FAIL\n"); return 1; }
    std::printf("008d6920_TEST PASS\n");
    return 0;
}