#pragma optimize("s",on)
// CTCKraken::EndAttackBeam() -- end the beam attack: Mode = 3, then kill the beam effect and the tentacles (SetMode(3) folded, as retail)
// Leaf: ?EndAttackBeam@CTCKraken@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FC800, 23 bytes, thiscall: `mov dword [esi+64h],3 ; call 0x007FB870 ; mov ecx,esi ; jmp 0x007FB1C0`.
// Name (NF348, 2026-11-24): Ego_r.pdb + FableWin.pdb ?EndAttackBeam@CTCKraken@@QAEXXZ; Ego_r KrakenLowAttackOutOf::Initialise calls
//   EndAttackBeam twice, and retail 0x00923F60 calls 0x007FC800 twice (table witness); Ego_r donor order SetMode, StartLowAttackBeam,
//   StartHighAttackBeam, EndAttackBeam, SetPhase = retail 0x007FC730 .. 0x007FC820. Was the free stand-in ?CTCKraken_EndAttackBeam@@YIXPAVCTCKraken@@@Z.
//   Lands with row 0x0045C5F6 (??1CThingNoiseDef, NF355), which defined this symbol on a stand-in until then.
// Layout: the shared engine/CTCKraken.h (Mode +0x64, EKrakenMode opaque: the literal is cast, as FrameUpdate 0x007FC9D0 casts its phase).
// Callees (NF355): the private CTCKraken::KillBeamEffect 0x007FB870 and CTCKraken::KillTentacles 0x007FB1C0 (Ego_r.pdb
//   ?KillBeamEffect@CTCKraken@@AAEXXZ / ?KillTentacles@CTCKraken@@AAEXXZ, both declared in the shared header; their landed rows define
//   those names and SetMode 0x007FC730 binds them), instead of the free stand-ins @Kraken_PreEnd@4 / @Kraken_Finish@4.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CTCKraken.h"

void CTCKraken::EndAttackBeam()
{
    Mode = (EKrakenMode)3;
    KillBeamEffect();
    KillTentacles();
}
