// CTCDExperienceOrb::StatGetClassTypeName() -- static: the class-type name "CTCDExperienceOrb" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCDExperienceOrb@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4369, 19 bytes: push esi; push -1; push 0x0123BB40 "CTCDExperienceOrb"; mov esi, ecx;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning
//   CCharString by value (the hidden result pointer arrives in ecx; plain ret).
// Identity (NF414, 2026-11-25): FableWin.pdb name set ?StatGetClassTypeName@CTCDExperienceOrb@@SI?AVCCharString@@XZ; the retail literal
//  0x0123BB40 = "CTCDExperienceOrb" = the class name (read from the exe); the class's GetClassTypeName 0x0077AB70 (RTTI
//  .?AVCTCDExperienceOrb@@ vftable 0x0126ABFC slot 22) expands it instead of calling it; CTCBase::RegisterTCs 0x004EE23F calls it at
//  0x004F2C5E and the landed binder already binds this spelling (reloc_bindings.tsv).
// Literal: NF413 spelled it (HEAD before nf413 cast the retail address (const char*)0x0123BB40); it is the string literal, the text
//  retail holds at 0x0123BB40 (.rdata, NUL-terminated, narrow).
// Respell (NF414): was the stand-in self ?FableStatGetClassTypeName_5@@YIPAVCCharString@@PAV1@@Z (no // Leaf: line) calling the made-up
//  ?Init@CCharString@@QAEXPBDH@Z.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (both name sets).
// Header: engine/CTCDExperienceOrb.h includes engine/CDefPointer.h, which #errors unless FABLE_RETAIL_DEF_CLASS_BASE is defined (a Wine
//  cl hang, GOTCHAS NF402), so the row declares the class with its registration interface only, as the landed binder 0x004EE23F does
//  (header debt: the default branch does not compile on its own).
// Form (NF414; the NF406 0x004D31C0 / NF408 form): the static is defined in the class body, so it is a pick-any COMDAT, and a
//  scaffolding helper takes its address so this object emits it (the nf86 / nf88 form, landing nf387 ruling). Retail's own definition
//  is an in-class inline: RegisterTCs 0x004EE23F expands it at its first 41 registrations and calls the out-of-line copy after that,
//  and the landed binder defines the same static in the class body (TC_REGISTRATION_INTERFACE, pick-any), so an out-of-line
//  (NoDuplicates) definition would clash with it at link (LNK4006, landing nf382 ruling; the nf402 lesson).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"

class CTCDExperienceOrb
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCDExperienceOrb", -1); }
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x004D4369 is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCDExperienceOrb::StatGetClassTypeName;
}
}
