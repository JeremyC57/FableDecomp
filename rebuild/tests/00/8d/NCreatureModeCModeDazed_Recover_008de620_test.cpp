#include <cstdio>
// Behaviour test for NCreatureMode::CModeDazed::Recover (retail 0x008DE620): the dazed creature starts the
// recover-from-dazed combat action.
class CThing { public: unsigned char m_state; unsigned char m_flags91; };
class CCreatureActionBase { public: int Kind; CThing* Owner; CThing* Target; };
class CThingCreatureBase : public CThing { public:
    int actions;
    CCreatureActionBase last;
    bool SetCurrentAction(const CCreatureActionBase& action) { ++actions; last = action; return true; }
};
struct CBaseIntelligentPointer {
    CThing* Item;
    CThing* GetPItem() const { return Item; }
};
static int g_baseUpdates;
struct CModeBase {
    CThingCreatureBase* ParentThing;
    void FrameUpdate() { ++g_baseUpdates; }
};
class CCombatAction_RecoverFromDazed : public CCreatureActionBase { public:
    CCombatAction_RecoverFromDazed(CThingCreatureBase* owner) { Kind = 1; Owner = owner; Target = 0; }
};
struct CModeDazed : CModeBase {
    void Recover()
    {
        CThingCreatureBase* creature = ParentThing;
        creature->SetCurrentAction(CCombatAction_RecoverFromDazed(creature));
    }
};

int main()
{
    CThingCreatureBase c;
    c.actions = 0;
    CModeDazed m;
    m.ParentThing = &c;
    m.Recover();
    if (c.actions != 1 || c.last.Kind != 1 || c.last.Owner != &c) { std::printf("008de620_TEST FAIL\n"); return 1; }
    std::printf("008de620_TEST PASS\n");
    return 0;
}