#include <cstdio>
// Behaviour test for NCreatureMode::CModeChargeForcePush::FrameUpdate (retail 0x008DFF50): after the base update a creature
// with thing-type flag 8 is given the force-push-out-of action (code 0x96), flagged to hit only enemies when the mode is.
class CCreatureActionBase { public: long Code; bool OnlyHitEnemies; };
class CThingCreatureBase { public:
    unsigned long ThingTypeFlags;
    int actions;
    CCreatureActionBase last;
    void SetCurrentAction(const CCreatureActionBase& action) { ++actions; last = action; }
};
class CCreatureAction_ForcePushOutOf : public CCreatureActionBase { public:
    CCreatureAction_ForcePushOutOf(CThingCreatureBase& owner) { Code = 0x96; OnlyHitEnemies = false; (void)owner; }
    void SetOnlyHitEnemies() { OnlyHitEnemies = true; }
};
static int g_baseUpdates;
struct CModeBase {
    CThingCreatureBase* ParentThing;
    void FrameUpdate() { ++g_baseUpdates; }
};
struct CModeChargeForcePush : CModeBase {
    bool OnlyHitEnemies;
    void FrameUpdate()
    {
        CModeBase::FrameUpdate();
        CThingCreatureBase* creature = ParentThing;
        if (creature->ThingTypeFlags & 8) {
            CCreatureAction_ForcePushOutOf action(*creature);
            if (OnlyHitEnemies)
                action.SetOnlyHitEnemies();
            creature->SetCurrentAction(action);
        }
    }
};

int main()
{
    CThingCreatureBase c;
    c.ThingTypeFlags = 8 | 2; c.actions = 0;
    CModeChargeForcePush m;
    m.ParentThing = &c; m.OnlyHitEnemies = true;
    g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = c.actions == 1 && c.last.Code == 0x96 && c.last.OnlyHitEnemies;
    m.OnlyHitEnemies = false;
    m.FrameUpdate();
    pass = pass && c.actions == 2 && !c.last.OnlyHitEnemies;
    c.ThingTypeFlags = 2;
    m.FrameUpdate();
    pass = pass && c.actions == 2 && g_baseUpdates == 3;
    if (!pass) { std::printf("008dff50_TEST FAIL\n"); return 1; }
    std::printf("008dff50_TEST PASS\n");
    return 0;
}