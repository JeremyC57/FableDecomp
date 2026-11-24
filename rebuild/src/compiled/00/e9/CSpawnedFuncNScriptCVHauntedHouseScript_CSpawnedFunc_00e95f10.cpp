// CSpawnedFunc<NScript::CV_HauntedHouseScript>::CSpawnedFunc(const CCharString& name, unsigned long id, NScript::CV_HauntedHouseScript* pclass, void (NScript::CV_HauntedHouseScript::*func)()) -- build a spawned script function bound to a script object and its member callback
// Leaf: ??0?$CSpawnedFunc@VCV_HauntedHouseScript@NScript@@@@QAE@ABVCCharString@@KPAVCV_HauntedHouseScript@NScript@@P823@AEXXZ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E95F10, 112 bytes, `__fastcall`, `ret 10h`; vtable 0x012ECBA8. FableWin.pdb has the shape
//   ??0?$CSpawnedFunc@V...@@@@QAE@ABVCCharString@@KPAV...@P8...@AEXXZ@Z for every script T; the only caller 0x00E94C0B passes
//   CV_HauntedHouseScript::CheckRegionLeaving 0x00E953E0 (caller 0x00E94C0B; the vtable 0x012ECBA8 it stores is the one CV_HauntedHouseScript's RegisterMain 0x00E93DA0 uses), so T is CV_HauntedHouseScript; the manifest's CV_ExposeMayorScript and CExpression_FollowScript labels are propagated.
// Callees: the inlined CSpawnedFuncBase ctor (0x00CDD450 out of line): process base ctor 0x00A44740 (1, 0x40, 0.1f),
//   CCharString(const CCharString&) 0x0099EC30, CCharString() 0x0099E4B0.
// Layout: Ego_r.pdb CSpawnedFunc<T> 0x3C = CSpawnedFuncBase (0x34) + Func (+0x34, single-inheritance member pointer) + PClass (+0x38).
//   Local polymorphic model (engine/CSpawnedFuncBase.h flattens the vptr).
// Shape: the base ctor is an inline definition in this TU; initialisers Func(func), PClass(pclass) -- VC7.1 schedules the PClass store
//   before the derived vtable store and Func after it.
// CSpawnedFuncBase: the destructor declared only (FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY), so the ??_G copies this TU emits with
//   the vtables call 0x00CDD4C0 out of line, as retail's 0x00CDD4A0 / 0x00E93E40 do (and as the HEAD row-local model declared it).
#define FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY
// NF336 (2026-11-24): moved off the row-local class model onto the shared engine headers (acctB Gate 0 prerequisite). The family
//   preamble below is shared by the CV_HauntedHouseScript object-mate rows (a per-row departure is marked where it occurs).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_DECLS
#define FABLE_RETAIL_CM_UNDEADHERE_MAIN
#define FABLE_RETAIL_CM_QUESTCONTROL_MAIN_PASS26
#define FABLE_RETAIL_CHHUNDEAD_MEMBERS
#define FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CV_HauntedHouseScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
// Retail expands the CSpawnedFuncBase constructor in this out-of-line copy (the CSuspendableProcessMicrothread(true, 0x40, 0.1f)
//   call 0x00A44740, then the members; RegisterMain 0x00E93DA0 calls the out-of-line 0x00CDD450 instead): its body is visible here.
inline CSpawnedFuncBase::CSpawnedFuncBase(const CCharString& name, unsigned long id)
    : CSuspendableProcessMicrothread(true, 0x40, 0.1f), ID(id), LastScriptFrame(-1), CurrentScriptFrame(-1), Name(name),
      FuncPaused(false), Interrupted(false)
{
}
template CSpawnedFunc<NScript::CV_HauntedHouseScript>::CSpawnedFunc(const CCharString& name, unsigned long id, NScript::CV_HauntedHouseScript* pclass, void (NScript::CV_HauntedHouseScript::*func)());
