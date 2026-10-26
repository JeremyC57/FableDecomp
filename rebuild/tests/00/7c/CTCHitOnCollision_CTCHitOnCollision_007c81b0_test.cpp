#include <cstdio>
class CThing { public: int id; };
class CThingGameObject;
static int g_baseCalls = 0;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_baseCalls; }
};
struct CRetailVector_GameObjectPtr { CThingGameObject** First; CThingGameObject** Last; CThingGameObject** End;
    CRetailVector_GameObjectPtr() : First(0), Last(0), End(0) { } };
class CTCHitOnCollision : public CTCBase_Calls { public:
    CRetailVector_GameObjectPtr CollidedList; bool DontHitWhenCombatAction, Blockable, PassThroughIfBlocked, PushSideways; float Damage;
    virtual void v00() { }
    CTCHitOnCollision(CThing* thing) : CTCBase_Calls(thing), CollidedList(), DontHitWhenCombatAction(false), Blockable(false),
        PassThroughIfBlocked(true), PushSideways(false), Damage(2.0f) { }
};
int main()
{
    CThing thing = { 3 };
    CTCHitOnCollision* h = new CTCHitOnCollision(&thing);
    if (g_baseCalls != 1 || h->ParentThing != &thing || h->CollidedList.First != 0 || h->CollidedList.Last != 0 || h->CollidedList.End != 0
        || h->DontHitWhenCombatAction || h->Blockable || !h->PassThroughIfBlocked || h->PushSideways || h->Damage != 2.0f)
    { std::printf("007c81b0_TEST FAIL\n"); return 1; }
    delete h;
    std::printf("007c81b0_TEST PASS\n");
    return 0;
}