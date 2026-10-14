#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CCreatureActionBase { virtual void v00() {} unsigned char _body[0x114]; ~CCreatureActionBase() { Log("dtor"); } };
struct CThingCreatureBase { CCreatureActionBase* m_pAction; bool SetCurrentAction(CCreatureActionBase* a) { m_pAction = a; Log("set"); return true; } };
struct CCreatureAction_RiseUp : CCreatureActionBase { CCreatureAction_RiseUp(CThingCreatureBase*) { Log("rise"); } };
struct CAIBrain { unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; };
struct CAIStateGroup_CreatureIdleUnderground : CAIStateGroupBase {
    virtual void OnReplace() { CThingCreatureBase* creature = PBrain->Creature; CCreatureAction_RiseUp action(creature); creature->SetCurrentAction(&action); }
};
int main()
{
    CThingCreatureBase c; c.m_pAction = 0; CAIBrain b; b.Creature = &c; CAIStateGroup_CreatureIdleUnderground g; g.PBrain = &b;
    g.OnReplace();
    if (c.m_pAction == 0) return Check("00920960", "x");
    return Check("00920960", "rise set dtor ");
}
