#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_base = 0, g_susp = 0; static bool g_suspArg = false; static void* g_suspMode = 0;
struct CModeBase;
struct CThing { unsigned char pad[0x10]; unsigned char type; void SetSuspendAnimation(bool s, CModeBase* m) { ++g_susp; g_suspArg = s; g_suspMode = m; } };
struct CModeBase { virtual ~CModeBase() { } CThing* thing; };
struct CModeStandBase : CModeBase { void OnActivate() { ++g_base; } void OnDeactivate() { ++g_base; } };

static int g_kill = 0; static bool g_killArg = false;
struct CThingX { unsigned char pad[0x91]; unsigned char dead : 1; unsigned char rest : 7; void Kill(bool f) { ++g_kill; g_killArg = f; } };
struct CBaseIP { virtual ~CBaseIP() { } CThingX* item; CThingX* GetPItem() const { return item; } };
struct CMode : CModeStandBase { unsigned char pad[0x14]; CBaseIP target;
    virtual void OnDeactivate() { CModeStandBase::OnDeactivate(); if (thing->type == 1) thing->SetSuspendAnimation(false, this); if (target.GetPItem() && !target.GetPItem()->dead) target.GetPItem()->Kill(true); } };
int main()
{
    CThing t; t.type = 1; CThingX v; v.dead = 0; CMode m; m.thing = &t; m.target.item = &v;
    m.OnDeactivate(); if (g_base != 1 || g_susp != 1 || g_suspArg || g_suspMode != &m || g_kill != 1 || !g_killArg) { std::printf("008d79a0_TEST FAIL\n"); return 1; }
    v.dead = 1; t.type = 2; m.OnDeactivate(); if (g_susp != 1 || g_kill != 1) { std::printf("008d79a0_TEST FAIL\n"); return 1; }
    m.target.item = 0; m.OnDeactivate(); if (g_kill != 1 || g_base != 3) { std::printf("008d79a0_TEST FAIL\n"); return 1; }
    std::printf("008d79a0_TEST PASS\n");
    return 0;
}