#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_base = 0, g_susp = 0; static bool g_suspArg = false; static void* g_suspMode = 0;
struct CModeBase;
class CThing { public: unsigned char pad[0x10]; unsigned char type; void SetSuspendAnimation(bool s, CModeBase* m) { ++g_susp; g_suspArg = s; g_suspMode = m; } };
struct CModeBase { virtual ~CModeBase() { } CThing* thing; };
struct CModeStandBase : CModeBase { void OnActivate() { ++g_base; } void OnDeactivate() { ++g_base; } };

struct CMode : CModeBase { bool down; virtual void OnDeactivate() { if (thing->type == 1) thing->SetSuspendAnimation(false, 0); down = false; } };
int main()
{
    CThing t; t.type = 1; CMode m; m.thing = &t; m.down = true;
    m.OnDeactivate(); if (g_susp != 1 || g_suspArg || g_suspMode != 0 || m.down) { std::printf("008d6740_TEST FAIL\n"); return 1; }
    t.type = 3; m.down = true; m.OnDeactivate(); if (g_susp != 1 || m.down) { std::printf("008d6740_TEST FAIL\n"); return 1; }
    std::printf("008d6740_TEST PASS\n");
    return 0;
}