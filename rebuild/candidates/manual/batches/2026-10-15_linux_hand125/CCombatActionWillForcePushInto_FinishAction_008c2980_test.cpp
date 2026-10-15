// Behaviour test for 0x008C2980 CCombatAction_WillForcePushInto::FinishAction: creature fetched before the base call, gets a cast-force action.
#include <cstdio>
#include <cstring>
static int g_liveStrings = 0;
struct CCharString {
    const char* text;
    CCharString(const char* t, long) : text(t) { ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
struct CCreatureActionBase { long level; bool release; char anim[32]; };
struct CThingCreatureBase_Calls {
    CCreatureActionBase last; int sets;
    CThingCreatureBase_Calls() : sets(0) { }
    bool SetCurrentAction(const CCreatureActionBase& a) { last = a; ++sets; return true; }
};
struct CCastWillForceSpell {
    CCreatureActionBase body; CThingCreatureBase_Calls* creature;
    CCastWillForceSpell(CThingCreatureBase_Calls* c, long level, bool release, const CCharString& anim)
        : creature(c) { body.level = level; body.release = release; std::strcpy(body.anim, anim.text); }
    operator const CCreatureActionBase&() const { return body; }
};
struct CBaseIntelligentPointer {
    CThingCreatureBase_Calls* m_pItem;
    CThingCreatureBase_Calls* GetPItem() const { return m_pItem; }
};
static int g_baseFinish = 0;
struct CCombatActionBase {
    CBaseIntelligentPointer PCreature;
    void FinishAction() { ++g_baseFinish; PCreature.m_pItem = 0; }   // the base drops the pointer: the local copy must survive
};
struct CWillForcePushInto : CCombatActionBase {
    void FinishAction()
    {
        CThingCreatureBase_Calls* creature = PCreature.GetPItem();
        CCombatActionBase::FinishAction();
        CCharString animName("ST_FORCE_PUSH_RELEASE", -1);
        creature->SetCurrentAction(CCastWillForceSpell(creature, 1, true, animName));
    }
};
int main()
{
    CThingCreatureBase_Calls creature;
    CWillForcePushInto a; a.PCreature.m_pItem = &creature;
    a.FinishAction();
    if (g_baseFinish != 1 || creature.sets != 1 || creature.last.level != 1 || !creature.last.release ||
        std::strcmp(creature.last.anim, "ST_FORCE_PUSH_RELEASE") != 0 || g_liveStrings != 0) {
        std::printf("008c2980_TEST FAIL\n"); return 1;
    }
    std::printf("008c2980_TEST PASS\n");
    return 0;
}
