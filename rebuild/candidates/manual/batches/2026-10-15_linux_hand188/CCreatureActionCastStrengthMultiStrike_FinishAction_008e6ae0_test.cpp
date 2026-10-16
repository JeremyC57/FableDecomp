// Behaviour test for 0x008E6AE0 CCreatureAction_CastStrengthMultiStrike::FinishAction: chain n-1 strikes or remove TC 0x8c.
#include <cstdio>
struct CCreatureActionBase { long strikes; void* owner; void* target; };
struct CThing { unsigned char m_flags91; long removed; void RemoveTC(long t) { removed = t; } };
struct CThingCreatureBase : CThing {
    CCreatureActionBase current; int sets;
    bool SetCurrentAction(const CCreatureActionBase& a) { current = a; ++sets; return true; }
};
struct CBaseIntelligentPointer { CThingCreatureBase* PItem; CThingCreatureBase* GetPItem() const { return PItem; } };
struct CCreatureAction_CastStrengthMultiStrike : CCreatureActionBase {
    CBaseIntelligentPointer Owner, Target; long NumberOfStrikes; int baseFinish;
    CCreatureAction_CastStrengthMultiStrike(CThingCreatureBase* o, CThing* t, long n) : NumberOfStrikes(n), baseFinish(0)
    { strikes = n; owner = o; target = t; Owner.PItem = o; Target.PItem = (CThingCreatureBase*)t; }
    void FinishAction()
    {
        ++baseFinish;
        if (NumberOfStrikes > 1 && Owner.GetPItem() && !(Owner.GetPItem()->m_flags91 & 1)
            && Target.GetPItem() && !(Target.GetPItem()->m_flags91 & 1)) {
            CThing* target = Target.GetPItem();
            CThingCreatureBase* owner = Owner.GetPItem();
            CThingCreatureBase* creature = Owner.GetPItem();
            creature->SetCurrentAction(CCreatureAction_CastStrengthMultiStrike(owner, target, NumberOfStrikes - 1));
        } else {
            Owner.GetPItem()->RemoveTC(0x8c);
        }
    }
};
int main()
{
    CThingCreatureBase o, t; o.m_flags91 = 0; t.m_flags91 = 0; o.sets = 0; o.removed = 0;
    CCreatureAction_CastStrengthMultiStrike a(&o, &t, 3);
    a.FinishAction();
    bool ok = o.sets == 1 && o.current.strikes == 2 && o.current.owner == &o && o.current.target == &t && o.removed == 0;
    a.NumberOfStrikes = 1; a.FinishAction(); ok = ok && o.sets == 1 && o.removed == 0x8c;
    o.removed = 0; a.NumberOfStrikes = 5; t.m_flags91 = 1; a.FinishAction(); ok = ok && o.sets == 1 && o.removed == 0x8c && a.baseFinish == 3;
    if (!ok) { std::printf("008e6ae0_TEST FAIL\n"); return 1; }
    std::printf("008e6ae0_TEST PASS\n");
    return 0;
}
