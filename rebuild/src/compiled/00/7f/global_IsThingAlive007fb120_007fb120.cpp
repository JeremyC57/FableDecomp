// CTCKraken::HeadReadyToStrike() -- the head may strike while the kraken's target exists and is not dead: IS_THING_ALIVE(PTarget)
// Leaf: ?HeadReadyToStrike@CTCKraken@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FB120, oracle length 39 bytes, thiscall.
// Retail bytes:  push esi ; lea esi,[ecx+0x20] ; mov ecx,esi ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ;
//                test byte ptr [eax+91h],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// Name (NF404, T3 nf401 0x007FB120): Ego_r.pdb + FableWin.pdb ?HeadReadyToStrike@CTCKraken@@QAE_NXZ (non-const, public).
//   (1) FableWin.pdb 0x02778AA0 (tc_kraken.obj, 35 B /Od): ecx+0x20 (PTarget) -> IS_THING_ALIVE<CIntelligentPointer<CThing>>
//       0x01CCA150, bool result; Ego_r.pdb 0x007F6450 is this retail body instruction for instruction (`lea esi,[ecx+20h]`, GetPItem
//       twice, the dead bit at Ego_r's +0xA1).
//   (2) the sole retail caller CAIStateGroup_KrakenAttack::Update 0x00924EDE sits where FableWin's Update (0x02C5FE12) calls
//       HeadReadyToStrike: false -> AIAction_DoNothing; true -> LowPoseInto (on a flag / counter) else RoarInto, HighAttackInto.
//   The old self ?IsThingAlive_007fb120@@YI_NAAUCHolder@@@Z and the table's ?IsTargetAlive@CTCKraken@@QBE_NXZ (descriptive label of
//   landing 2026-11-08e: "in neither PDB name set") are both made up.
// Layout: the shared engine/CTCKraken.h (PTarget +0x20 = Ego_r's offset 32); IS_THING_ALIVE from engine/ThingAlive.h (GetPItem
//   0x00A01B50 twice: the null test, then CThing::IsDead, bit 0 of +0x91).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCKraken.h"
#include "engine/ThingAlive.h"

bool CTCKraken::HeadReadyToStrike()
{
    return IS_THING_ALIVE(PTarget);
}
