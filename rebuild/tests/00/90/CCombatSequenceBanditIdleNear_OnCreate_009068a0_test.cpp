// behaviour test for 0x009068A0 CCombatSequence_BanditIdleNear::OnCreate() (model; PushAction records the queued action)
#include <cstdio>
static int g_pushed = 0, g_base = 0; static long g_code = 0; static const void* g_owner; static const void* g_target;
class CThing { };
class CThingCreatureBase : public CThing { };
class CCombatActionBase { public: CCombatActionBase(CThingCreatureBase& o, CThing& t, long code) { g_owner = &o; g_target = &t; g_code = code; } virtual ~CCombatActionBase() { } };
class CCreatureAction_LeapBack : public CCombatActionBase { public: CCreatureAction_LeapBack(CThingCreatureBase& o, CThingCreatureBase& t) : CCombatActionBase(o, t, 50) { } };
class CCombatAction { public: CCombatActionBase* p; CCombatAction() : p(0) { } ~CCombatAction() { delete p; } };
struct CSeq {
    CThingCreatureBase* Creature; CThingCreatureBase* Target;
    void PushAction(CCombatAction& a) { if (a.p) ++g_pushed; }
    void BaseOnCreate() { ++g_base; }
    void OnCreate() { CCombatAction action; action.p = new CCreatureAction_LeapBack(*Creature, *Target); PushAction(action); BaseOnCreate(); }
};
int main()
{
    CThingCreatureBase c, t; CSeq s; s.Creature = &c; s.Target = &t;
    s.OnCreate();
    int ok = g_pushed == 1 && g_base == 1 && g_code == 50 && g_owner == &c && g_target == &t;
    if (!ok) { std::printf("009068a0_TEST FAIL\n"); return 1; }
    std::printf("009068a0_TEST PASS\n");
    return 0;
}