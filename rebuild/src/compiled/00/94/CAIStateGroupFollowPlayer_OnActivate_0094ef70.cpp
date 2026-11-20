// CAIStateGroup_FollowPlayer::OnActivate() -- pick the follow distance: a world-seed random float up to 2.6 plus the base distance
// Leaf: ?OnActivate@CAIStateGroup_FollowPlayer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094EF70, plain `ret` (virtual slot 2): `call 0x008FCFD0 ; push 2.6f ; mov ecx,eax ;
//   call 0x004978A0 ; fadd [0x01293344] ; fstp [esi+1Ch]`.
// NF131 (2026-11-20, q5141): the free stand-in CAIStateGroup_FollowPlayer_OnActivate over the flat generated header (GetObj / Method stand-ins)
//   gives way to the retail class (engine/CAIStateGroup_FollowPlayer.h FABLE_RETAIL_CAISTATEGROUP_FOLLOWPLAYER on the retail
//   engine/CAIStateGroupBase.h). Own name: ?OnActivate@CAIStateGroup_FollowPlayer@@UAEXXZ (Ego_r.pdb 0x00973830 + FableWin.pdb 0x02BD5C90), was
//   ?CAIStateGroup_FollowPlayer_OnActivate@@YIXPAUCAIStateGroup_FollowPlayer@@@Z. Callees: ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ 0x008FCFD0
//   (Ego_r 0x00847E20 + FableWin 0x0288DA10; was ?GetObj@@YIPAUFollowPlayerProximitySource@@PAX@Z) and ?GFFloatRandom@@YIMMPAK@Z 0x004978A0
//   (Ego_r 0x004A1E20 + FableWin 0x018BAC90, out of line: engine/GFFloatRandom.h; was ?Method@FollowPlayerProximitySource@@QAEMM@Z).
//   The float at 0x01293344 keeps the landed descriptive ?g_bias@@3MA.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_FOLLOWPLAYER
#include "engine/CAIStateGroup_FollowPlayer.h"
#include "engine/GFFloatRandom.h"
extern float g_bias;                                     // 0x01293344

void CAIStateGroup_FollowPlayer::OnActivate()
{
    Proximity = GFFloatRandom(2.6f, GetPWorldSeed()) + g_bias;
}
