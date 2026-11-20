// CCreatureActionBase::PeekThingSearchTools() const -- the creature's thing search tools, through its intelligent pointer and
//   CThing::PeekThingSearchTools
// Leaf: ?PeekThingSearchTools@CCreatureActionBase@@IBEABVCThingSearchTools@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00692990, 15 bytes: `add ecx,8 ; call GetPItem 0x00A01B50 ; mov ecx,eax ; jmp 0x004C7A60`.
// Name fix (NF155, q5083; was the stand-in free function Dest_val with dv_h2_004c7a60, catalog std::_Dest_val<...CRegisteredMusicEntry>,
//   a propagated label): Ego_r.pdb 0x00519300 + FableWin.pdb 0x01DB6530 ?PeekThingSearchTools@CCreatureActionBase@@IBEABVCThingSearchTools@@XZ
//   (both right after GetThingManager, as retail 0x00692990 after 0x00692980); declared in engine/CCreatureActionBase.h (opt-in
//   FABLE_RETAIL_CCREATUREACTIONBASE_PEEKTHINGSEARCHTOOLS) and already bound under this symbol by the landed caller 0x008BC1A0. The
//   callee 0x004C7A60 is the protected ?PeekThingSearchTools@CThing@@IBEABVCThingSearchTools@@XZ (Ego_r + FableWin, engine/CThing.h
//   FABLE_RETAIL_CTHING_MANAGERACCESS), reached through the existing CCreatureActionBase friend opt-in.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_PEEKWORLD_FRIEND_CREATUREACTIONBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PEEKTHINGSEARCHTOOLS
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"

const CThingSearchTools& CCreatureActionBase::PeekThingSearchTools() const
{
    return static_cast<const CThing*>(PCreature.GetPItem())->PeekThingSearchTools();
}
