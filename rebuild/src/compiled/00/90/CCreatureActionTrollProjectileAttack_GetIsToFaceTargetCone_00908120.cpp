// Leaf: ?GetIsToFaceTargetCone@CCreatureAction_TrollProjectileAttack@@UBEMXZ
// CCreatureAction_TrollProjectileAttack::GetIsToFaceTargetCone @ 0x00908120
//   fld DWORD PTR ds:0x122ded8
//   ret
//
// Virtual const accessor that returns a global float. The 'this' pointer
// (ecx) is unused: the value is a module-global loaded straight into st(0).
// Modeled as __fastcall (this in ecx, byte-identical to __fastcall for a
// this-only accessor). VC7.1 emits `fld DWORD PTR ds:<addr>; ret` for a
// function that simply returns a global float value.
//
// The address 0x122ded8 is relocation-masked in parity, so the global need
// not resolve to that exact address; declaring it extern lets the linker
// place it while the compiler still emits the fld/ret shape.

#define FABLE_RETAIL_CCREATUREACTION_TROLLPROJECTILEATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_TROLLWHACKGROUNDBASE_CLASS
#include "engine/CCreatureAction_TrollProjectileAttack.h"
extern float g_FloatOne;

float CCreatureAction_TrollProjectileAttack::GetIsToFaceTargetCone() const
{
    return g_FloatOne;
}
// Flatten route (flatten_pilot v5, NF330): CCreatureAction_TrollProjectileAttack__GetIsToFaceTargetCone -> CCreatureAction_TrollProjectileAttack::GetIsToFaceTargetCone, the RTTI-placed PDB member; class model: engine/CCreatureAction_TrollProjectileAttack.h opt-in FABLE_RETAIL_CCREATUREACTION_TROLLPROJECTILEATTACK_VIRTUAL_MODEL, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR, FABLE_RETAIL_CCREATUREACTION_TROLLWHACKGROUNDBASE_CLASS.
