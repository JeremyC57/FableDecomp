// CCreatureActionBase::PeekWorld() const -- the creature's current world, through its intelligent pointer and CThing::PeekWorld
// Leaf: ?PeekWorld@CCreatureActionBase@@IBEABVCWorld@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00692950, 15 bytes: `add ecx,8 ; call GetPItem 0x00A01B50 ; mov ecx,eax ; jmp 0x004C7AB0`
//   (the const twin of the landed GetWorld 0x00692940, which tail-jumps to CThing::GetWorld 0x004C7AA0).
// Name-fix 2026-11-12 (was the stand-in free function Dest_val with dv_h2_004c7ab0, catalog _global::Dest_val): Ego_r.pdb
//   005192b0 ?PeekWorld@CCreatureActionBase@@IBEABVCWorld@@XZ (directly after GetWorld 005192a0) + FableWin.exe.signatures 01db6490;
//   declared in engine/CCreatureActionBase.h (FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD) and bound under this symbol by the landed
//   const caller CCombatAction_GenericStrikeResponseKnockdown::FinishAction 0x00867B40. The callee 0x004C7AB0 is
//   ?PeekWorld@CThing@@IBEABVCWorld@@XZ (protected; header overlay opt-in FABLE_RETAIL_CTHING_PEEKWORLD_FRIEND_CREATUREACTIONBASE
//   gives this class access, as the twin's local friend does).
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
#define FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"

const CWorld& CCreatureActionBase::PeekWorld() const
{
    return static_cast<const CThing*>(PCreature.GetPItem())->PeekWorld();
}
