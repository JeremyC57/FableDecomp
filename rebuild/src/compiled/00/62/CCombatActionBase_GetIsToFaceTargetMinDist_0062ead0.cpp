// CAIStateGroup_EnemyCreatureBase::GetCombatMinDistance
// Leaf: ?CCombatActionBase_GetIsToFaceTargetMinDist@@YIMPAUCAIStateGroup_EnemyCreatureBase@@@Z
// retail: fld dword ptr [0x122f59c]; ret  -> return a float global
struct CAIStateGroup_EnemyCreatureBase;

extern float g_CombatMinDistance_0122f59c;

float __fastcall CCombatActionBase_GetIsToFaceTargetMinDist(CAIStateGroup_EnemyCreatureBase* self)
{
    return g_CombatMinDistance_0122f59c;
}
// Stand-in (owner rule 2026-11-05): CCombatActionBase_GetIsToFaceTargetMinDist for CCombatActionBase::GetIsToFaceTargetMinDist; RTTI vtable slot ?GetIsToFaceTargetMinDist@CCombatActionBase@@UBEMXZ + PDB name set (egor/fablewin).
