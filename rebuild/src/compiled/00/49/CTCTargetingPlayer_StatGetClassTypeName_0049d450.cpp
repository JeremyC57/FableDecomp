// CTCTargetingPlayer::StatGetClassTypeName() -- static: the class-type name "CTCTargetingPlayer" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCTargetingPlayer@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049D450, 19 bytes: push esi; push -1; push 0x01238B98 "CTCTargetingPlayer"; mov esi, ecx;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning
//   CCharString by value (the hidden result pointer arrives in ecx; plain ret).
// Identity (NF417, 2026-11-25): FableWin.pdb name set ?StatGetClassTypeName@CTCTargetingPlayer@@SI?AVCCharString@@XZ (the Ego_r set has
//  no static of this class); the retail literal 0x01238B98 = "CTCTargetingPlayer" = the class name (read from the exe); the class's
//  GetClassTypeName 0x004E230A (RTTI .?AVCTCTargetingPlayer@@ vftable 0x0124280C slot 22) calls 0x0049D450; CTCBase::RegisterTCs
//  0x004EE23F expands it (push 0x01238B98 at 0x004EEA3A, one of its first 41 registrations) and the landed binder emits the same
//  pick-any copy; retail also expands it at 0x004A3E39 (CWorld::UpdateRegionLoad 0x004A3740, whose landed row includes
//  engine/CTCTargetingPlayer.h with the in-class static), 0x004A50F8 and 0x004ACBAB; its one rel32 caller is the GetClassTypeName
//  forwarder 0x004E230A (call at 0x004E230E).
// Literal: already spelled at HEAD ("CTCTargetingPlayer", -1); it is the text retail holds at 0x01238B98 (.rdata, NUL-preceded,
//  NUL-terminated, narrow).
// Respell (NF417, RULINGS r1 of NF412): was the stand-in self ??0CTCTargetingPlayer@@QAE@XZ, a constructor name for a function that is
//  no constructor (the bytes are a static returning a CCharString in the hidden slot), with a local one-member class and a local
//  CCharString.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (both name sets), the same symbol HEAD called.
// Header: engine/CTCTargetingPlayer.h under the layout-rename pattern (default branch: the generated flat layout); the static touches no member.
// Form (NF417; the NF414 / NF406 0x004D31C0 form): the static is defined in the class body, so it is a pick-any COMDAT, and a
//  scaffolding helper takes its address so this object emits it (the nf86 / nf88 form, landing nf387 ruling). Retail's own definition
//  is an in-class inline (the class's other TUs expand it; this is the out-of-line copy), and the landed binder defines the same
//  statics in the class body (pick-any), so an out-of-line (NoDuplicates) definition would clash with it at link (LNK4006, landing
//  nf382 ruling).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define CTCTargetingPlayer CTCTargetingPlayerLayout
#include "engine/CTCTargetingPlayer.h"
#undef CTCTargetingPlayer

class CTCTargetingPlayer : public CTCTargetingPlayerLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCTargetingPlayer", -1); }
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x0049D450 is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCTargetingPlayer::StatGetClassTypeName;
}
}
