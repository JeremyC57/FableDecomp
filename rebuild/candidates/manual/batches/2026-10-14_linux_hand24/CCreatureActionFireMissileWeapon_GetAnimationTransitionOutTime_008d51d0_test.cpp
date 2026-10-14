#include <cstdio>
struct CThingCreatureBase_Calls { bool m_quick; virtual bool IsQuickFiring() const { return m_quick; } };
struct CBaseIntelligentPointer { virtual void v00() { } CThingCreatureBase_Calls* m_pItem; CThingCreatureBase_Calls* GetPItem() const { return m_pItem; } };
struct CCreatureActionBase { virtual void v00() { } unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; long GetAnimationTransitionOutTime() const { return 42; } };
struct CCreatureAction_FireMissileWeapon : CCreatureActionBase { long GetAnimationTransitionOutTime() const; };
long CCreatureAction_FireMissileWeapon::GetAnimationTransitionOutTime() const { if (PCreature.GetPItem()->IsQuickFiring()) return 3; return CCreatureActionBase::GetAnimationTransitionOutTime(); }
int main()
{
    CThingCreatureBase_Calls creature; creature.m_quick = true; CCreatureAction_FireMissileWeapon a; a.PCreature.m_pItem = &creature;
    if (a.GetAnimationTransitionOutTime() != 3) { std::printf("008d51d0_TEST FAIL\n"); return 1; }
    creature.m_quick = false; if (a.GetAnimationTransitionOutTime() != 42) { std::printf("008d51d0_TEST FAIL\n"); return 1; }
    std::printf("008d51d0_TEST PASS\n");
    return 0;
}
