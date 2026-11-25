// CTCObjectAugmentations::StatGetClassTypeName() -- static: the class-type name "CTCObjectAugmentations" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCObjectAugmentations@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3CDE, 19 bytes: push esi; push -1; push 0x0123B330 "CTCObjectAugmentations"; mov esi, ecx;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning
//   CCharString by value (the hidden result pointer arrives in ecx; plain ret).
// Identity (NF402, 2026-11-25): FableWin.pdb name set ?StatGetClassTypeName@CTCObjectAugmentations@@SI?AVCCharString@@XZ (tc_register.obj); the retail
//   literal 0x0123B330 = "CTCObjectAugmentations" = the class name; binder CTCBase::RegisterTCs 0x004EE23F already binds this spelling (reloc_bindings.tsv); retail caller 0x004F6051 (RegisterTCs).
// Literal: NF388 literal audit (pending/2026-11-25_nf388_handback/held_fixes): was the wrong "DoCreatureAction*" text.
// Respell NF402: was a free-function stand-in (?CActionDoCreatureAction_StatGetClassTypeName@@YI?AVCCharString@@PAVCTCObjectAugmentations@@@Z).
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (both name sets).
// Header: engine/CTCObjectAugmentations.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form); the static touches no member.
// Form (NF407, 2026-11-25): the static is defined in the class body, so it is a pick-any COMDAT, and a scaffolding helper takes its
//   address so this object emits it (the nf86 / nf88 form, landing nf387 ruling; the nf406_b2 0x004D31C0 form). The landed binder
//   CTCBase::RegisterTCs 0x004EE23F defines the same static in the class body (TC_REGISTRATION_INTERFACE, pick-any), and retail's own
//   definition is an in-class inline (RegisterTCs expands it at its first 41 registrations and calls the out-of-line copy after that).
//   The previous out-of-line (NoDuplicates) definition clashed with the binder's pick-any copy at link (LNK4006, landing nf382 ruling).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE
#define CTCObjectAugmentations CTCObjectAugmentationsLayout
#include "engine/CTCObjectAugmentations.h"
#undef CTCObjectAugmentations

class CTCObjectAugmentations : public CTCObjectAugmentationsLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCObjectAugmentations", -1); }
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x004D3CDE is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCObjectAugmentations::StatGetClassTypeName;
}
}
