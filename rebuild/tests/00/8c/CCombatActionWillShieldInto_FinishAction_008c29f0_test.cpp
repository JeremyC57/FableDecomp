// Behaviour test for 0x008C29F0 CCombatAction_WillShieldInto::FinishAction: seconds -> frames (-1 stays forever), shield action handed over.
#include <cstdio>
#include <cstring>
class CCharString { public:
    const char* text;
    CCharString(const char* t, long) : text(t) { }
};
class CCreatureActionBase { public: long level; long frames; float health; };
struct CThingCreatureBase_Calls {
    CCreatureActionBase last; int sets;
    CThingCreatureBase_Calls() : sets(0) { }
    bool SetCurrentAction(const CCreatureActionBase& a) { last = a; ++sets; return true; }
};
struct CCastPhysicalShieldSpell {
    CCreatureActionBase body;
    CCastPhysicalShieldSpell(CThingCreatureBase_Calls*, long level, bool, const CCharString&, long frames, float health)
    { body.level = level; body.frames = frames; body.health = health; }
    operator const CCreatureActionBase&() const { return body; }
};
long g_framesPerSecond = 15;
struct CBaseIntelligentPointer {
    CThingCreatureBase_Calls* m_pItem;
    CThingCreatureBase_Calls* GetPItem() const { return m_pItem; }
};
class CCombatActionBase { public:
    CBaseIntelligentPointer PCreature;
    void FinishAction() { }
};
struct CWillShieldInto : CCombatActionBase {
    float ShieldUpTime, ShieldHealth;
    void FinishAction()
    {
        long shieldFrames;
        union { float f; unsigned int l; } bits;
        bits.f = ShieldUpTime;
        if (bits.l == 0xBF800000u)
            shieldFrames = -1;
        else
            shieldFrames = (long)(ShieldUpTime * g_framesPerSecond);
        CThingCreatureBase_Calls* creature = PCreature.GetPItem();
        CCombatActionBase::FinishAction();
        CCharString animName("", -1);
        creature->SetCurrentAction(CCastPhysicalShieldSpell(creature, 2, true, animName, shieldFrames, ShieldHealth));
    }
};
int main()
{
    CThingCreatureBase_Calls creature;
    CWillShieldInto a; a.PCreature.m_pItem = &creature; a.ShieldUpTime = 2.0f; a.ShieldHealth = 50.0f;
    a.FinishAction();
    if (creature.last.level != 2 || creature.last.frames != 30 || creature.last.health != 50.0f) { std::printf("008c29f0_TEST FAIL\n"); return 1; }
    a.ShieldUpTime = -1.0f;
    a.FinishAction();
    if (creature.last.frames != -1 || creature.sets != 2) { std::printf("008c29f0_TEST FAIL\n"); return 1; }
    std::printf("008c29f0_TEST PASS\n");
    return 0;
}