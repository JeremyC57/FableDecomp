#include <cstdio>
// Behaviour test for NCreatureMode::CModeDragonFireBeam::FrameUpdate (retail 0x008E0780): after the hover update the beam
// effect starts once the dragon (state 1) has performed its current action; the beam timer counts down and the beam is
// updated every frame; at zero the dragon starts the fire-beam-out-of action.
struct CThing { unsigned char m_state; unsigned char m_flags91; };
struct CCreatureActionBase { int Kind; CThing* Owner; CThing* Target; };
struct CThingCreatureBase : CThing {
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
struct CCombatActionBase : CCreatureActionBase {
    bool Performed;
    bool HasPerformedAction() const { return Performed; }
};
struct CDragon : CThingCreatureBase {
    CCombatActionBase* PCurrentAction;
    CCombatActionBase* GetCurrentAction() const { return PCurrentAction; }
};
struct CCreatureAction_DragonFireBeamOutOf : CCreatureActionBase {
    CCreatureAction_DragonFireBeamOutOf(CThingCreatureBase* owner) { Kind = 4; Owner = owner; Target = 0; }
};
struct CModeDragonHover {
    CDragon* ParentThing;
    void FrameUpdate() { ++g_baseUpdates; }
};
struct CModeDragonFireBeam : CModeDragonHover {
    long Timer;
    int starts;
    int updates;
    void StartBeamEffect() { ++starts; }
    void UpdateBeam() { ++updates; }
    void FrameUpdate()
    {
        CModeDragonHover::FrameUpdate();
        if (ParentThing->m_state == 1 && ParentThing->GetCurrentAction() != 0 && ParentThing->GetCurrentAction()->HasPerformedAction())
            StartBeamEffect();
        --Timer;
        UpdateBeam();
        if (Timer <= 0) {
            CThingCreatureBase* creature = ParentThing;
            creature->SetCurrentAction(CCreatureAction_DragonFireBeamOutOf(creature));
        }
    }
};

int main()
{
    CCombatActionBase roar;
    roar.Performed = false;
    CDragon dragon;
    dragon.actions = 0; dragon.m_state = 1; dragon.PCurrentAction = &roar;
    CModeDragonFireBeam m;
    m.ParentThing = &dragon; m.Timer = 2; m.starts = 0; m.updates = 0;
    g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = m.starts == 0 && m.updates == 1 && m.Timer == 1 && dragon.actions == 0;
    roar.Performed = true;
    m.FrameUpdate();
    pass = pass && m.starts == 1 && m.updates == 2 && dragon.actions == 1 && dragon.last.Kind == 4;
    dragon.PCurrentAction = 0;
    m.FrameUpdate();
    pass = pass && m.starts == 1 && g_baseUpdates == 3;
    dragon.PCurrentAction = &roar; dragon.m_state = 2;
    m.FrameUpdate();
    pass = pass && m.starts == 1;
    if (!pass) { std::printf("008e0780_TEST FAIL\n"); return 1; }
    std::printf("008e0780_TEST PASS\n");
    return 0;
}