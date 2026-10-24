#include <cstdio>
// Behaviour test for NCreatureMode::CModeDragonGetHit::FrameUpdate (retail 0x008E06E0): after the base update the get-hit
// timer counts down; at zero (and every frame after) the dragon snaps out of it at its target.
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
struct CCreatureAction_SnapDragonOutOf : CCreatureActionBase {
    CCreatureAction_SnapDragonOutOf(CThingCreatureBase* owner, CThing* target) { Kind = 2; Owner = owner; Target = target; }
};
struct CModeDragonGetHit : CModeBase {
    CBaseIntelligentPointer PTarget;
    long Timer;
    void FrameUpdate()
    {
        CModeBase::FrameUpdate();
        if (--Timer <= 0) {
            CThingCreatureBase* creature = ParentThing;
            creature->SetCurrentAction(CCreatureAction_SnapDragonOutOf(creature, PTarget.GetPItem()));
        }
    }
};

int main()
{
    CThingCreatureBase dragon;
    dragon.actions = 0;
    CThing hero;
    CModeDragonGetHit m;
    m.ParentThing = &dragon; m.PTarget.Item = &hero; m.Timer = 2;
    g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = dragon.actions == 0 && m.Timer == 1;
    m.FrameUpdate();
    pass = pass && dragon.actions == 1 && dragon.last.Kind == 2 && dragon.last.Target == &hero && g_baseUpdates == 2;
    if (!pass) { std::printf("008e06e0_TEST FAIL\n"); return 1; }
    std::printf("008e06e0_TEST PASS\n");
    return 0;
}