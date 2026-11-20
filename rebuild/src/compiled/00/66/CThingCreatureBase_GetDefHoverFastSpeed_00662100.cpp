// CThingCreatureBase::GetDefHoverFastSpeed() const -- returns the creature definition's MaxHoverSpeedFast
// Leaf: ?GetDefHoverFastSpeed@CThingCreatureBase@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662100, 13 bytes, thiscall: `mov eax,[ecx+0E0h] ; fld dword ptr [eax+0ACh] ; ret`.
// Names (NF155, PRIORITY q11684; was the July free fastcall stand-in for the propagated catalog
//   label CTCPhysicsBase::GetAirResistance): Ego_r.pdb 0x0053CF80 + FableWin.pdb 0x01D9AFD0 ?GetDefHoverFastSpeed@CThingCreatureBase@@QBEMXZ (Ego_r
//   lists GetDefHover{Slow,Medium,Fast}Speed as three consecutive 16-byte bodies, as retail 0x006620E0 / 0x006620F0 / 0x00662100;
//   FableWin.pdb has them in the same order); the body
//   reads PDef (+0xE0, CDefPointer<const CThingCreatureDef>) and the def's MaxHoverSpeedFast (+0xAC, Ego_r +0xB0 less the 4-byte
//   retail base shift, engine/CThingCreatureDef.h MEMBERS opt-in). The landed caller 0x008EA500 CModeHoverBase::InitialiseMovementSpeeds
//   already binds this spelling (NF128). Same shape as the landed sibling 0x00662010 CThingCreatureBase::GetProperty.
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETDEFHOVERSPEEDS
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/EMovementType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
float CThingCreatureBase::GetDefHoverFastSpeed() const
{
    return PDef->MaxHoverSpeedFast;
}
