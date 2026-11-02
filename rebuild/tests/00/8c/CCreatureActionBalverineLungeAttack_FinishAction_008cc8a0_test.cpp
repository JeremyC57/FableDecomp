// Behaviour test for CCreatureAction_BalverineLungeAttack::FinishAction (0x008CC8A0): the creature's physics component is made pushable by
// the hero again (slot 84, true), component 200 (CTCHitOnCollision) is removed, then the combat-action base FinishAction runs.
#include <cstdio>
enum ETCInterfaceType { };
static int gOrder, gPushAt, gRemoveAt, gBaseAt; static bool gPushArg; static int gRemoved;
class CTCPhysicsBase { public:
    bool PushableByHero;
    virtual void SetAsPushableByHero(bool pushable) { gPushAt = ++gOrder; gPushArg = pushable; PushableByHero = pushable; }
};
class CBaseObject { public: virtual ~CBaseObject() { } };
class CThing : public CBaseObject { public:
    CTCPhysicsBase* PhysicsTC;
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    void RemoveTC(ETCInterfaceType type) { gRemoveAt = ++gOrder; gRemoved = (int)type; }
};
class CThingCreatureBase : public CThing { };
class CBaseIntelligentPointer { public:
    CBaseObject* Item;
    CBaseObject* GetPItem() const { return Item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CCombatActionBase { public:
    virtual ~CCombatActionBase() { }
    CIntelligentPointer<CThingCreatureBase> PCreature;
protected:
    virtual void FinishAction() { gBaseAt = ++gOrder; }
};
class CCreatureAction_BalverineLungeAttack : public CCombatActionBase { public:
    virtual void FinishAction();
};
void CCreatureAction_BalverineLungeAttack::FinishAction()
{
    PCreature.GetPItem()->GetPhysicsTC()->SetAsPushableByHero(true);
    PCreature.GetPItem()->RemoveTC((ETCInterfaceType)200);
    CCombatActionBase::FinishAction();
}
int main()
{
    CTCPhysicsBase physics; physics.PushableByHero = false;
    CThingCreatureBase balverine; balverine.PhysicsTC = &physics;
    CCreatureAction_BalverineLungeAttack lunge; lunge.PCreature.Item = &balverine;
    lunge.FinishAction();
    if (gPushAt != 1 || !gPushArg || !physics.PushableByHero || gRemoveAt != 2 || gRemoved != 200 || gBaseAt != 3) {
        std::printf("008cc8a0_TEST FAIL\n"); return 1; }
    std::printf("008cc8a0_TEST PASS\n");
    return 0;
}