#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_base = 0, g_susp = 0; static bool g_suspArg = false; static void* g_suspMode = 0;
struct CModeBase;
struct CThing { unsigned char pad[0x10]; unsigned char type; void SetSuspendAnimation(bool s, CModeBase* m) { ++g_susp; g_suspArg = s; g_suspMode = m; } };
struct CModeBase { virtual ~CModeBase() { } CThing* thing; };
struct CModeStandBase : CModeBase { void OnActivate() { ++g_base; } void OnDeactivate() { ++g_base; } };

struct CMode : CModeStandBase { long beamState; bool beamFlag; virtual void OnActivate() { CModeStandBase::OnActivate(); if (thing->type == 1) thing->SetSuspendAnimation(true, this); beamState = 0; beamFlag = false; } };
int main()
{
    CThing t; t.type = 1; CMode m; m.thing = &t; m.beamState = 5; m.beamFlag = true;
    m.OnActivate(); if (g_base != 1 || g_susp != 1 || !g_suspArg || g_suspMode != &m || m.beamState != 0 || m.beamFlag) { std::printf("008d6a50_TEST FAIL\n"); return 1; }
    t.type = 2; m.beamState = 5; m.OnActivate(); if (g_base != 2 || g_susp != 1 || m.beamState != 0) { std::printf("008d6a50_TEST FAIL\n"); return 1; }
    std::printf("008d6a50_TEST PASS\n");
    return 0;
}