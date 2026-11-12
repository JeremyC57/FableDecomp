// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CCombatActionBase>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCCombatActionBase@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085A5A0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x0085A5A0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_0085a5a0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCCombatActionBase@@@@SIXPAX@Z; stored by the CCountedPointer<CCombatActionBase>(T*) ctor 0x0085A840 and Reset 0x00905FD0 (reloc_bindings ??0 / ?Reset@?$CCountedPointer@VCCombatActionBase@@@@..., callers 0x00857430 / 0x00906150); Ego_r.pdb CCombatAction::CreatureAction is CCountedPointer<CCombatActionBase>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#include "engine/CDefPointer.h"
#include "engine/CCombatActionBase.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CCombatActionBase>::DeleteData(void*);
