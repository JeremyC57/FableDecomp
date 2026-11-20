// CCreatureActionBase::GetPlayer() -- the creature's player, through its intelligent pointer and CThing::GetPlayer
// Leaf: ?GetPlayer@CCreatureActionBase@@IAEAAVCPlayer@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00692930, 15 bytes: `add ecx,8 ; call GetPItem 0x00A01B50 ; mov ecx,eax ; jmp 0x004C7A30`
//   (the same shape as the landed twins GetWorld 0x00692940 / PeekWorld 0x00692950).
// Name fix (NF155, q4745; was the stand-in free function Dest_val with dv_h1 / dv_h2_004c7a30, catalog CCreatureActionBase::GetPlayer):
//   Ego_r.pdb 0x00519290 + FableWin.pdb 0x01DB6430 ?GetPlayer@CCreatureActionBase@@IAEAAVCPlayer@@XZ (Ego_r order GetPlayer / GetWorld,
//   as retail 0x00692930 / 0x00692940); declared in engine/CCreatureActionBase.h (opt-in FABLE_RETAIL_CCREATUREACTIONBASE_GETPLAYER) and
//   already bound under this symbol by the landed caller 0x008424A0. The callee 0x004C7A30 is ?GetPlayer@CThing@@QAEAAVCPlayer@@XZ
//   (Ego_r + FableWin, engine/CThing.h).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_PEEKWORLD_FRIEND_CREATUREACTIONBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETPLAYER
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"

CPlayer& CCreatureActionBase::GetPlayer()
{
    return static_cast<CThing*>(PCreature.GetPItem())->GetPlayer();
}
