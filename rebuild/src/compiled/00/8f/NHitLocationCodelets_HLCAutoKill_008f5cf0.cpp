// NHitLocationCodelets::HLCAutoKill(CThing& thing, const CHitParameters& hit_parameters) -- hit-location codelet 2 (AUTO_KILL): kill the thing that was hit
// Leaf: ?HLCAutoKill@NHitLocationCodelets@@YIXAAVCThing@@ABVCHitParameters@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F5CF0, 8 bytes: push 1 ; call CThing::Kill 0x004C9B80 ; ret (__fastcall, thing in ecx).
// Identity (agentNF69, q10834): Ego_r.pdb 0x0091B8D0 + FableWin.pdb ?HLCAutoKill@NHitLocationCodelets@@YIXAAVCThing@@ABVCHitParameters@@@Z; Ego_d object
//   hit_location_codelets.obj; NHitLocations::CHitLocation::StatRegisterClass 0x008162F0 registers this address as EHitLocationCodelet 2
//   (Ego_d AUTO_KILL) in RegisteredHitLocationCodelets (docs/journal/2026-11/EGOD_FINDINGS.md section 1). Was the generated
//   _unattributed::CallWith_008f5cf0 placeholder (CClass_008f5cf0 stand-in). The hit parameters are unused.
// Callee: CThing::Kill(bool) 0x004C9B80 (engine/CThing.h, Ego_r public member).
#include "engine/CThing.h"
class CHitParameters;

namespace NHitLocationCodelets {
void __fastcall HLCAutoKill(CThing& thing, const CHitParameters& hit_parameters)
{
    thing.Kill(true);
}
}
