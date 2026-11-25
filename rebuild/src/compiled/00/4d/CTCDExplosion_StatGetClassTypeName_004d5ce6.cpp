// CTCDExplosion::StatGetClassTypeName() -- static: the class-type name "CTCDExplosion" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCDExplosion@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5CE6, 19 bytes: push esi; push -1; push 0x0123D8B0 "CTCDExplosion"; mov esi, ecx;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning
//   CCharString by value (the hidden result pointer arrives in ecx; plain ret).
// Identity (NF402, 2026-11-25): FableWin.pdb name set ?StatGetClassTypeName@CTCDExplosion@@SI?AVCCharString@@XZ (tc_register.obj); the retail
//   literal 0x0123D8B0 = "CTCDExplosion" = the class name; binder CTCBase::RegisterTCs 0x004EE23F already binds this spelling (reloc_bindings.tsv); retail callers 0x004E314C (CTCDExplosion::GetClassTypeName 0x004E3148) and 0x004F73C2 (RegisterTCs).
// Literal: NF388 literal audit (pending/2026-11-25_nf388_handback/held_fixes): was the wrong "DoCreatureAction*" text.
// Respell NF402: was a free-function stand-in (?StatGetClassTypeName@@YI?AVCCharString@@XZ).
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (both name sets).
// Header: engine/CTCDExplosion.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form); the static touches no member.
// Form (NF407, 2026-11-25): the static is defined in the class body, so it is a pick-any COMDAT, and a scaffolding helper takes its
//   address so this object emits it (the nf86 / nf88 form, landing nf387 ruling; the nf406_b2 0x004D31C0 form). The landed binder
//   CTCBase::RegisterTCs 0x004EE23F defines the same static in the class body (TC_REGISTRATION_INTERFACE, pick-any), and retail's own
//   definition is an in-class inline (RegisterTCs expands it at its first 41 registrations and calls the out-of-line copy after that).
//   The previous out-of-line (NoDuplicates) definition clashed with the binder's pick-any copy at link (LNK4006, landing nf382 ruling).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#define CTCDExplosion CTCDExplosionLayout
#include "engine/CTCDExplosion.h"
#undef CTCDExplosion

class CTCDExplosion : public CTCDExplosionLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCDExplosion", -1); }
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x004D5CE6 is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCDExplosion::StatGetClassTypeName;
}
}
