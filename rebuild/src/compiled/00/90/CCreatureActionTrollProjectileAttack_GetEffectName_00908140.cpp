// Leaf: _CCreatureAction_TrollProjectileAttack_GetEffectName@4
#include "engine/CCreatureAction_TrollProjectileAttack.h"
#include "rebuild_abi.h"

struct FableEffectResult_00908140 { void* ctor(const char* text); };
extern "C" char FABLE_STDCALL CCreatureAction_TrollProjectileAttack_GetEffectName(FableEffectResult_00908140* result)
{
    result->ctor((const char*)19456924);
    return 1;
}
// Stand-in (owner rule 2026-11-05): CCreatureAction_TrollProjectileAttack_GetEffectName for CCreatureAction_TrollProjectileAttack::GetEffectName; RTTI vtable slot ?GetEffectName@CCreatureAction_TrollProjectileAttack@@MBE_NAAVCCharString@@@Z + PDB name set (egor/fablewin).
