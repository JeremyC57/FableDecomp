#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
class CThingCreatureBase { public:};
namespace N_0062e900 { struct CCreatureActionBase { virtual void v00() {} unsigned char _body[0x110]; ~CCreatureActionBase() { Log("dtor"); } }; } using N_0062e900::CCreatureActionBase;
namespace N_008ec6a0 { struct CCreatureAction_GoUnderground : CCreatureActionBase { CCreatureAction_GoUnderground(CThingCreatureBase* c) { Log(c ? "ctor" : "ctor-null"); } }; } using N_008ec6a0::CCreatureAction_GoUnderground;
class CAIBrain { public: unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc];
    long AIAction_PerformActionAndWait(CCreatureActionBase*, bool wait) { Log(wait ? "perform-wait" : "perform"); return 0; }
    void Terminate() { Log("terminate"); }
};
struct CAIStateGroup_CreatureGoUnderground : CAIStateGroupBase {
    virtual void Update() { CThingCreatureBase* creature = PBrain->Creature; { CCreatureAction_GoUnderground action(creature); AIAction_PerformActionAndWait(&action, false); } Terminate(); }
};
int main()
{
    CThingCreatureBase c; CAIBrain b; b.Creature = &c; CAIStateGroup_CreatureGoUnderground g; g.PBrain = &b;
    { g.Update(); }
    return Check("00920730", "ctor perform dtor terminate ");
}