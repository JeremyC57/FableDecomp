// CTCVillage::StatGetClassTypeName() -- static: the class-type name "CTCVillage" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCVillage@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D31C0, 19 bytes: push esi; push -1; push 0x0123A5CC "CTCVillage"; mov esi, ecx;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning
//   CCharString by value (the hidden result pointer arrives in ecx; plain ret).
// Identity (NF406, 2026-11-25): FableWin.pdb name set ?StatGetClassTypeName@CTCVillage@@SI?AVCCharString@@XZ (tc_register.obj); the retail
//   literal 0x0123A5CC = "CTCVillage" = the class name (read from the exe); binder CTCBase::RegisterTCs 0x004EE23F already binds this
//   spelling (reloc_bindings.tsv); its only retail caller is 0x004F00E3 (RegisterTCs). CTCVillage::GetClassTypeName 0x0066122E
//   (vftable 0x0125A9AC slot 22) expands the body instead of calling it.
// Literal: the HEAD text cast the address (const char*)0x123a5cc, which carries no relocation and escapes the literal check; it is
//   now the string literal itself (the same text retail holds at 0x0123A5CC).
// Respell NF406: was a free-function stand-in (?StatGetClassTypeName@@YIPAVCCharString@@PAV1@@Z, the spelling HEAD 0x004D4232 also
//   had) calling the made-up ?ctor@CCharString@@QAEPAV1@PBDH@Z.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (both name sets).
// Header: engine/CTCVillage.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form, flattened default branch); the static
//   touches no member.
// Form (NF406): the static is defined in the class body, so it is a pick-any COMDAT, and a scaffolding helper takes its address so this
//   object emits it (the nf86 / nf88 form; landing nf387 ruling). Retail's own definition is an in-class inline: VC7.1 expands it at
//   the first 41 registrations of CTCBase::RegisterTCs 0x004EE23F and calls the out-of-line copy after that (FableWin.pdb puts the copy
//   in tc_register.obj), and the landed binder 0x004EE23F defines it in the class body too (TC_REGISTRATION_INTERFACE). An out-of-line
//   (NoDuplicates) definition here clashes with the binder's pick-any copy at link (LNK4006: a new duplicate, landing nf382 ruling),
//   which is why this row departs from the NF402 b1 out-of-line form of its siblings.
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define CTCVillage CTCVillageLayout
#include "engine/CTCVillage.h"
#undef CTCVillage

class CTCVillage : public CTCVillageLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCVillage", -1); }
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x004D31C0 is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCVillage::StatGetClassTypeName;
}
}
