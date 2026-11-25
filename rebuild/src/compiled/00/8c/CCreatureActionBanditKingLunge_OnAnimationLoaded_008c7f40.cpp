// CCreatureAction_BanditKingLunge::OnAnimationLoaded  @ 0x008c7f40

struct CCreatureAction_Base
{
    char pad[0x10];   // 0x00 .. 0x0f
    int field10;      // 0x10
    int field14;      // 0x14
};

class CCreatureAction_BanditKingLunge : public CCreatureAction_Base
{ public:
    void OnAnimationLoaded();
};

// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3; q1: the PDB member keeps its protected / private access and a friend line gives the caller access):
//   0x00859EF0 ?OnAnimationLoaded@CCombatActionBase@@MAEXXZ (was ?BaseOnAnimationLoaded@CCreatureAction_Base@@QAEXXZ)
class CCreatureAction_BanditKingLunge;
class CCombatActionBase { protected: virtual void OnAnimationLoaded(); friend class CCreatureAction_BanditKingLunge; };
void CCreatureAction_BanditKingLunge::OnAnimationLoaded()
{
    reinterpret_cast<CCombatActionBase*>(this)->CCombatActionBase::OnAnimationLoaded();
    if (field14 < 0)
        field14 = field10 - 5;
}