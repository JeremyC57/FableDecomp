#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CThingCreatureBase {};
struct CCreatureActionBase { virtual void v00() {} unsigned char _body[0x110]; ~CCreatureActionBase() { Log("dtor"); } };
struct CCreatureAction_BeetleLand : CCreatureActionBase { CCreatureAction_BeetleLand(CThingCreatureBase*) { Log("land"); } };
struct CCreatureAction_BeetleTakeOff : CCreatureActionBase { CCreatureAction_BeetleTakeOff(CThingCreatureBase*) { Log("takeoff"); } };
struct CAIBrain { unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc];
    long AIAction_PerformActionAndWait(CCreatureActionBase*, bool wait) { Log(wait ? "perform-wait" : "perform"); return 0; }
    void Terminate() { Log("terminate"); }
};
struct CAIStateGroup_BeetleToggleHover : CAIStateGroupBase {
    bool IsLanding;
    virtual void Update()
    {
        if (IsLanding) { CThingCreatureBase* creature = PBrain->Creature; CCreatureAction_BeetleLand action(creature); AIAction_PerformActionAndWait(&action, false); }
        else { CThingCreatureBase* creature = PBrain->Creature; CCreatureAction_BeetleTakeOff action(creature); AIAction_PerformActionAndWait(&action, false); }
        Terminate();
    }
};
int main()
{
    CThingCreatureBase c; CAIBrain b; b.Creature = &c; CAIStateGroup_BeetleToggleHover g; g.PBrain = &b;
    g.IsLanding = true; g.Update(); g.IsLanding = false; g.Update();
    return Check("009211d0", "land perform dtor terminate takeoff perform dtor terminate ");
}