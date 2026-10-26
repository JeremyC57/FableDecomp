#include <cstdio>
// Behaviour test for NCreatureMode::CModeBriarRoseTeleportIntangible::FrameUpdate (retail 0x008E8940): after the base
// update the teleport timer counts down; at zero, with a live target, the concentrate-into action is started at it.
class CThing { public: unsigned char m_flags91; };
class CCreatureActionBase { public: CThing* Target; bool Flag; };
static CThing* g_startedAt;
static int g_starts;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action) { ++g_starts; g_startedAt = action.Target; return !action.Flag; }
};
class CCreatureAction_BriarRoseConcentrateInto : public CCreatureActionBase { public:
    CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase* owner, CThing* target, bool flag) { Target = target; Flag = flag; (void)owner; }
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
struct CModeBriarRoseTeleportIntangible : CModeBase {
    long Timer;
    CBaseIntelligentPointer PTarget;
    void FrameUpdate()
    {
        CModeBase::FrameUpdate();
        if (--Timer <= 0) {
            CThingCreatureBase* owner = ParentThing;
            if (PTarget.GetPItem() != 0 && !(PTarget.GetPItem()->m_flags91 & 1) &&
                owner->SetCurrentAction(CCreatureAction_BriarRoseConcentrateInto(owner, PTarget.GetPItem(), false)))
                return;
        }
    }
};

int main()
{
    CThingCreatureBase rose;
    rose.m_flags91 = 0;
    CThing target;
    target.m_flags91 = 0;
    CModeBriarRoseTeleportIntangible m;
    m.ParentThing = &rose; m.Timer = 2; m.PTarget.Item = &target;
    g_starts = 0; g_startedAt = 0; g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = m.Timer == 1 && g_starts == 0;
    m.FrameUpdate();
    pass = pass && m.Timer == 0 && g_starts == 1 && g_startedAt == &target;
    target.m_flags91 = 1;
    m.FrameUpdate();
    pass = pass && g_starts == 1;
    m.PTarget.Item = 0;
    m.FrameUpdate();
    pass = pass && g_starts == 1 && g_baseUpdates == 4 && m.Timer == -2;
    if (!pass) { std::printf("008e8940_TEST FAIL\n"); return 1; }
    std::printf("008e8940_TEST PASS\n");
    return 0;
}