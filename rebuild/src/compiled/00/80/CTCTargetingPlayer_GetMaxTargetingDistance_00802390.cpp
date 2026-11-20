// CTCTargetingPlayer::GetMaxTargetingDistance() const -- the player targeting component's maximum targeting distance (MaxTargetingDistance +0x48)
// Leaf: ?GetMaxTargetingDistance@CTCTargetingPlayer@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00802390, 4 bytes: `fld dword ptr [ecx+48h] ; ret`.
// NF123 (2026-11-20, q4690): re-spelled from the stand-in ?M@C00802390@@QAEMXZ (local C00802390 model) to Ego_r.pdb 0x007FD220 + FableWin.pdb
//   0x024CE410 ?GetMaxTargetingDistance@CTCTargetingPlayer@@QBEMXZ on the shared engine/CTCTargetingPlayer.h (retail branch, MAXDISTANCE
//   opt-in); the landed caller 0x007C0690 binds it. Catalog label _global::GetMember_float0x48_00802390 was descriptive.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_MAXDISTANCE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CTCTargetingPlayer.h"

float CTCTargetingPlayer::GetMaxTargetingDistance() const
{
    return MaxTargetingDistance;
}
