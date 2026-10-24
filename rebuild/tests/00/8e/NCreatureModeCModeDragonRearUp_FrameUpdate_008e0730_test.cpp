#include <cstdio>
// Behaviour test for NCreatureMode::CModeDragonRearUp::FrameUpdate (retail 0x008E0730): the rear-up timer only counts while
// the dragon is not performing an action; once it is spent the dragon starts the rear-up-out-of action.
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
static bool g_busy;
struct CDragon : CThingCreatureBase {
    bool IsPerformingAnyAction() { return g_busy; }
};
struct CCreatureAction_DragonRearUpOutOf : CCreatureActionBase {
    CCreatureAction_DragonRearUpOutOf(CThingCreatureBase* owner) { Kind = 3; Owner = owner; Target = 0; }
};
struct CModeDragonRearUp {
    CDragon* ParentThing;
    long Timer;
    void FrameUpdate()
    {
        ++g_baseUpdates;
        CDragon* creature = ParentThing;
        if (!creature->IsPerformingAnyAction())
            --Timer;
        if (Timer <= 0)
            creature->SetCurrentAction(CCreatureAction_DragonRearUpOutOf(creature));
    }
};

int main()
{
    CDragon dragon;
    dragon.actions = 0;
    CModeDragonRearUp m;
    m.ParentThing = &dragon; m.Timer = 1;
    g_busy = true;
    m.FrameUpdate();
    int pass = m.Timer == 1 && dragon.actions == 0;
    g_busy = false;
    m.FrameUpdate();
    pass = pass && m.Timer == 0 && dragon.actions == 1 && dragon.last.Kind == 3;
    if (!pass) { std::printf("008e0730_TEST FAIL\n"); return 1; }
    std::printf("008e0730_TEST PASS\n");
    return 0;
}