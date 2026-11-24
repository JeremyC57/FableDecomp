#pragma optimize("s",on)
// CTCKraken::EndAttackBeam() -- end the beam attack: Mode = 3, then kill the beam effect and the tentacles (SetMode(3) folded, as retail)
// Leaf: ?EndAttackBeam@CTCKraken@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FC800, 23 bytes, thiscall: `mov dword [esi+64h],3 ; call 0x007FB870 ; mov ecx,esi ; jmp 0x007FB1C0`.
// Name (NF348, 2026-11-24): Ego_r.pdb + FableWin.pdb ?EndAttackBeam@CTCKraken@@QAEXXZ; Ego_r KrakenLowAttackOutOf::Initialise calls
//   EndAttackBeam twice, and retail 0x00923F60 calls 0x007FC800 twice (table witness); Ego_r donor order SetMode, StartLowAttackBeam,
//   StartHighAttackBeam, EndAttackBeam, SetPhase = retail 0x007FC730 .. 0x007FC820. Was the free stand-in ?CTCKraken_EndAttackBeam@@YIXPAVCTCKraken@@@Z.
// Layout: the shared engine/CTCKraken.h (Mode +0x64, EKrakenMode opaque: the literal is cast, as FrameUpdate 0x007FC9D0 casts its phase).
// Callees: KillBeamEffect 0x007FB870 and KillTentacles 0x007FB1C0 keep their landed free __fastcall stand-ins (@Kraken_PreEnd@4 /
//   @Kraken_Finish@4; their owner rows are T3 ports nf302), so the call bytes and bindings are unchanged.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CTCKraken.h"

extern "C" void __fastcall Kraken_PreEnd(CTCKraken* self);    // 0x007FB870 (stand-in for the private CTCKraken::KillBeamEffect)
extern "C" void __fastcall Kraken_Finish(CTCKraken* self);    // 0x007FB1C0 (stand-in for the private CTCKraken::KillTentacles)

void CTCKraken::EndAttackBeam()
{
    Mode = (EKrakenMode)3;
    Kraken_PreEnd(this);
    Kraken_Finish(this);
}
