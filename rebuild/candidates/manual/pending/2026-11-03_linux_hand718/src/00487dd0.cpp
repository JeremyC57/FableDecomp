// CPlayer::PeekPControlledCreature() const -- const view of the creature this player controls
// Leaf: ?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487DD0, 8 bytes: `add ecx,2Ch ; jmp 0x00A01B50` (byte twin of GetPControlledCreature 0x00487DC0).
// Identity: Ego_r.pdb / FableWin.pdb ?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ (Ego_r 0x00493660, out of
//   line; donor order SetControlledCreature 0x00493570 -> GetPControlledCreature -> PeekPControlledCreature 0x00493660 matches
//   retail 0x00487CF0 -> 0x00487DC0 -> 0x00487DD0); 19 landed callers bind this spelling.
// Body: ControlledCreature (+0x2c, CIntelligentPointer<CThingPlayerCreature>)'s inline Get() = static_cast of
//   CBaseIntelligentPointer::GetPItem() (0x00A01B50, ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ); the cast adds no
//   code (CBaseObject is the primary base at offset 0), so the call becomes a tail jump.
// Layout: engine/CPlayer.h (FABLE_RETAIL_PLAYER; chunk 718 types the +0x2c / +0x34 members under FABLE_RETAIL_CINTELLIGENTPOINTER),
//   engine/CIntelligentPointer.h (retail packed model), engine/CThingPlayerCreature.h (: CThingCreatureBase : ... : CBaseObject).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingPlayerCreature.h"
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"

const CThingPlayerCreature* CPlayer::PeekPControlledCreature() const
{
    return ControlledCreature.Get();
}
