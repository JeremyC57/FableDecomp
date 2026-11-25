// CTCActionUseStartExtrapolationMorph::StatGetClassTypeName() -- static: the class-type name "CTCActionUseStartExtrapolationMorph" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCActionUseStartExtrapolationMorph@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2C68, 19 bytes: push esi; push -1; push 0x0123A198 "CTCActionUseStartExtrapolationMorph"; mov esi, ecx;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning
//   CCharString by value (the hidden result pointer arrives in ecx; plain ret).
// Identity (NF414, 2026-11-25): FableWin.pdb name set ?StatGetClassTypeName@CTCActionUseStartExtrapolationMorph@@SI?AVCCharString@@XZ;
//  the retail literal 0x0123A198 = "CTCActionUseStartExtrapolationMorph" = the class name (read from the exe); the class's
//  GetClassTypeName 0x004DABAC (RTTI .?AVCTCActionUseStartExtrapolationMorph@@ vftable 0x0123F6FC slot 22) calls 0x004D2C68;
//  CTCBase::RegisterTCs 0x004EE23F calls it at 0x004EF9BA and the landed binder already binds this spelling (reloc_bindings.tsv).
// Literal (NF414, 2026-11-25): HEAD cast the retail address (void*)0x0123A198 (no relocation; a baked string that
//  purity.baked_string_addresses did not see, NF413 typedef_cast_scan); it is now the string literal, spelled from retail's bytes at
//  0x0123A198 (.rdata, NUL-terminated, narrow).
// Respell (NF414): was the stand-in self ?CTCActionUseStartExtrapolationMorph_StatGetClassTypeName@@YIPAXPAUGAN004d2c68@@@Z (no //
//  Leaf: line) calling the made-up ?m@GAN004d2c68@@QAEPAXPAXH@Z.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (both name sets).
// Header: no engine header; the class is declared with its registration interface only, as the landed binder 0x004EE23F does.
// Form (NF414; the NF406 0x004D31C0 / NF408 form): the static is defined in the class body, so it is a pick-any COMDAT, and a
//  scaffolding helper takes its address so this object emits it (the nf86 / nf88 form, landing nf387 ruling). Retail's own definition
//  is an in-class inline: RegisterTCs 0x004EE23F expands it at its first 41 registrations and calls the out-of-line copy after that,
//  and the landed binder defines the same static in the class body (TC_REGISTRATION_INTERFACE, pick-any), so an out-of-line
//  (NoDuplicates) definition would clash with it at link (LNK4006, landing nf382 ruling; the nf402 lesson).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"

class CTCActionUseStartExtrapolationMorph
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCActionUseStartExtrapolationMorph", -1); }
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x004D2C68 is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCActionUseStartExtrapolationMorph::StatGetClassTypeName;
}
}
