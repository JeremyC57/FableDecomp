// CTCTargeted::StatGetClassTypeName() -- static: the class-type name "CTCTargeted" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCTargeted@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2A91, 19 bytes: push esi; push -1; push 0x01239FC0 "CTCTargeted"; mov esi, ecx;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning
//   CCharString by value (the hidden result pointer arrives in ecx; plain ret).
// Identity (NF414, 2026-11-25): FableWin.pdb name set ?StatGetClassTypeName@CTCTargeted@@SI?AVCCharString@@XZ; the retail literal
//  0x01239FC0 = "CTCTargeted" = the class name (read from the exe); the class's GetClassTypeName 0x004DA926 (RTTI .?AVCTCTargeted@@
//  vftable 0x0123F024 slot 22) calls 0x004D2A91; CTCBase::RegisterTCs 0x004EE23F expands it (one of its first 41 registrations), and
//  the landed binder emits the same pick-any copy.
// Literal (NF414, 2026-11-25): HEAD cast the retail address (void*)0x01239FC0 (no relocation; a baked string that
//  purity.baked_string_addresses did not see, NF413 typedef_cast_scan); it is now the string literal, spelled from retail's bytes at
//  0x01239FC0 (.rdata, NUL-terminated, narrow).
// Respell (NF414): was the stand-in self ?CTCTargeted_StatGetClassTypeName@@YIPAXPAUGAN004d2a91@@@Z (no // Leaf: line) calling the
//  made-up ?m@GAN004d2a91@@QAEPAXPAXH@Z.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (both name sets).
// Header: engine/CTCTargeted.h under the layout-rename pattern (the 0x004D31C0 CTCVillage form, default branch); the static touches no member.
// Form (NF414; the NF406 0x004D31C0 / NF408 form): the static is defined in the class body, so it is a pick-any COMDAT, and a
//  scaffolding helper takes its address so this object emits it (the nf86 / nf88 form, landing nf387 ruling). Retail's own definition
//  is an in-class inline: RegisterTCs 0x004EE23F expands it at its first 41 registrations and calls the out-of-line copy after that,
//  and the landed binder defines the same static in the class body (TC_REGISTRATION_INTERFACE, pick-any), so an out-of-line
//  (NoDuplicates) definition would clash with it at link (LNK4006, landing nf382 ruling; the nf402 lesson).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define CTCTargeted CTCTargetedLayout
#include "engine/CTCTargeted.h"
#undef CTCTargeted

class CTCTargeted : public CTCTargetedLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCTargeted", -1); }
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x004D2A91 is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCTargeted::StatGetClassTypeName;
}
}
