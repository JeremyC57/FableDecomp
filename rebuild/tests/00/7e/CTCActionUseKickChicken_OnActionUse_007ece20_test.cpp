#include <cstdio>
#include <cstring>

class CThing { public: int id; };
class CThingCreatureBase;
static int g_live = 0, g_ctors = 0, g_dtors = 0;
static CThingCreatureBase* g_ctorCreature = 0; static CThing* g_ctorThing = 0;
class CCreatureActionBase { public: int kind; };
class CCombatAction_KickThingOnGround : public CCreatureActionBase { public:
    CThingCreatureBase* m_creature; CThing* m_thing;
    CCombatAction_KickThingOnGround(CThingCreatureBase* c, CThing* t) : m_creature(c), m_thing(t) { kind = 42; ++g_ctors; ++g_live; g_ctorCreature = c; g_ctorThing = t; }
    ~CCombatAction_KickThingOnGround() { ++g_dtors; --g_live; }
};
class CThingCreatureBase : public CThing { public:
    int m_seenKind; int m_liveAtCall;
    bool SetCurrentAction(CCreatureActionBase* action) { m_seenKind = action->kind; m_liveAtCall = g_live; return true; }
};
class CTCBase { public: CThing* ParentThing; CThing* GetThing() const { return ParentThing; } };
struct CTCActionUseKickChicken : CTCBase {
    void OnActionUse(CThing& user)
    {
        CThingCreatureBase& creature = static_cast<CThingCreatureBase&>(user);
        CCombatAction_KickThingOnGround action(&creature, GetThing());
        creature.SetCurrentAction(&action);
    }
};
int main()
{
    CThing chicken; chicken.id = 9;
    CThingCreatureBase hero; hero.id = 1; hero.m_seenKind = 0; hero.m_liveAtCall = 0;
    CTCActionUseKickChicken use; use.ParentThing = &chicken;
    use.OnActionUse(hero);
    if (g_ctors != 1 || g_dtors != 1 || g_live != 0 || hero.m_seenKind != 42 || hero.m_liveAtCall != 1) { std::printf("007ece20_TEST FAIL\n"); return 1; }
    if (g_ctorCreature != &hero || g_ctorThing != &chicken) { std::printf("007ece20_TEST FAIL\n"); return 1; }
    std::printf("007ece20_TEST PASS\n");
    return 0;
}