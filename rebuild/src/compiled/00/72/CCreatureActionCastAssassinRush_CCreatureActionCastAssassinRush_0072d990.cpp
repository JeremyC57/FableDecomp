// CCreatureAction_CastAssassinRush::~CCreatureAction_CastAssassinRush() -- the implicit destructor: releases FinalAction (the
// CCountedPointer<CCreatureActionBase> at +0x12c: count block destroy + delete at zero, fields nulled), then ~CCombatActionBase by tail call
// Leaf: ??1CCreatureAction_CastAssassinRush@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072D990, 75 bytes; the implicit (compiler-generated) destructor, emitted out of line.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0x130] ; test eax,eax ; je N ; dec dword ptr [eax] ; mov eax,[esi+0x130] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+0x130] ; push eax ; call operator delete ; add esp,4 ; N: mov dword ptr [esi+0x12c],0 ; mov dword ptr [esi+0x130],0 ; mov ecx,esi ; pop esi ; jmp CCombatActionBase::~CCombatActionBase
// Identity (NF93, q11262): catalog CCreatureAction_CastAssassinRush::~CCreatureAction_CastAssassinRush; the landed 0x0096AE50 binds
//   ??1CCreatureAction_CastAssassinRush@@UAE@XZ to this address (reloc_bindings); its retail callers are the CastAssassinRush users
//   0x0072CAA0 SetupAssassinRush, 0x008C2550 JackOfBladesIntoRush::FinishAction, 0x0096AE50 WhisperRetreat::UseWill (cs.py); the released
//   member is FinalAction at +0x12C of engine/CCreatureAction_CastAssassinRush.h (retail class on CCombatActionBase, sizeof 0x138).
// Port (NF93, q11262): the unattributed stand-ins (CBase_0062e900 / CBase_0062e900_Derived_0072d990, a local CCountedPointer / SCountBlock)
//   replaced by the shared headers: the row is the header class's implicit destructor, emitted by the scaffolding below; the tail call now
//   binds ??1CCombatActionBase@@UAE@XZ (0x0062E900) and the release the shared CCountedPointer.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_CastAssassinRush.h"


namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the header class's implicit destructor, which VC7.1 does not expand
// in place (as at its retail callers 0x0072CAA0 / 0x008C2550 / 0x0096AE50, which call 0x0072D990), so this object emits the destructor out
// of line as a pick-any COMDAT (retail's 0x0072D990). Internal to this TU; never called.
void ReferenceCastAssassinRushDestructor(CCreatureAction_CastAssassinRush& action)
{
    action.CCreatureAction_CastAssassinRush::~CCreatureAction_CastAssassinRush();
}
}
