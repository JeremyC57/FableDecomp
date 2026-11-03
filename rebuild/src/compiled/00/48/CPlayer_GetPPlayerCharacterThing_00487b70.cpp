// CPlayer::GetPPlayerCharacterThing() const -- the player character (PlayerCharacter's item, or null) as a CThing
// Leaf: ?GetPPlayerCharacterThing@CPlayer@@QBEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487B70, 8 bytes: `add ecx,34h ; jmp 0x00A01B50`.
// Identity: Ego_r.pdb ?GetPPlayerCharacterThing@CPlayer@@QBEPAVCThing@@XZ (Ego_r 0x004933F0, out of
//   line; donor order Uninitialise 0x00493380 -> GetPPlayerCharacterThing -> SetPlayerCharacter 0x00493400 -> UninitCharacter 0x00493450 matches
//   retail 0x00487AF0 -> 0x00487B70 -> 0x00487B80 -> 0x00487BD0, and 8 bytes like Ego_r's 0x10 slot).
//   0x00487B80 builds a CIntelligentPointer temporary from its argument and SetPItem()s +0x34: SetPlayerCharacter. The catalog / bound
//   spelling ?GetPlayerCharacter@CPlayer@@QAEPAVCThing@@XZ is in neither PDB (NAME_FIXES.tsv).
// Body: PlayerCharacter (+0x34, CIntelligentPointer<CThingPlayerCreature>)'s inline Get() = static_cast of
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

CThing* CPlayer::GetPPlayerCharacterThing() const
{
    return PlayerCharacter.Get();
}