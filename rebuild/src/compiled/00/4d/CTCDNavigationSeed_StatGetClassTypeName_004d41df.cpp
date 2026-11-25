// CTCDNavigationSeed::StatGetClassTypeName() -- static: the class-type name "CTCDNavigationSeed" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCDNavigationSeed@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D41DF, 19 bytes: push esi; push -1; push 0x0123B928 "CTCDNavigationSeed"; mov esi, ecx;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning
//   CCharString by value (the hidden result pointer arrives in ecx; plain ret).
// Identity (NF417, 2026-11-25): FableWin.pdb name set ?StatGetClassTypeName@CTCDNavigationSeed@@SI?AVCCharString@@XZ (the Ego_r set has
//  no static of this class); the retail literal 0x0123B928 = "CTCDNavigationSeed" = the class name (read from the exe); the class's
//  GetClassTypeName 0x004D4212 (RTTI .?AVCTCDNavigationSeed@@ vftable 0x0123B944 slot 22) calls 0x004D41DF; CTCBase::RegisterTCs
//  0x004EE23F calls it at 0x004F26DC and the landed binder already binds this spelling (reloc_bindings.tsv).
// Literal: already spelled at HEAD ("CTCDNavigationSeed", -1); it is the text retail holds at 0x0123B928 (.rdata, NUL-preceded,
//  NUL-terminated, narrow).
// Respell (NF417, RULINGS r1 of NF412): was the stand-in self ??0CThingComponentDefNameTag@@QAE@XZ, a constructor name for a function
//  that is no constructor (the bytes are a static returning a CCharString in the hidden slot), with a local one-member class and a
//  local CCharString. the same constructor spelling also sat on 0x004D3DFC, 0x004D3EF9 and 0x004D41DF (three addresses, one name); no
//  CThingComponentDefNameTag class exists in either PDB name set or RTTI.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (both name sets), the same symbol HEAD called.
// Header: engine/CTCDNavigationSeed.h under the layout-rename pattern (default branch: CTCDNavigationSeed : CTCDriverBase); the static touches no member.
// Form (NF417; the NF414 / NF406 0x004D31C0 form): the static is defined in the class body, so it is a pick-any COMDAT, and a
//  scaffolding helper takes its address so this object emits it (the nf86 / nf88 form, landing nf387 ruling). Retail's own definition
//  is an in-class inline (the class's other TUs expand it; this is the out-of-line copy), and the landed binder defines the same
//  statics in the class body (pick-any), so an out-of-line (NoDuplicates) definition would clash with it at link (LNK4006, landing
//  nf382 ruling).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define CTCDNavigationSeed CTCDNavigationSeedLayout
#include "engine/CTCDNavigationSeed.h"
#undef CTCDNavigationSeed

class CTCDNavigationSeed : public CTCDNavigationSeedLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCDNavigationSeed", -1); }
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x004D41DF is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCDNavigationSeed::StatGetClassTypeName;
}
}
