// CPlayer::CreateCharacter(const CCharString& characterDefName) -- create this player's character creature: find the start holy
// site (or take the debug override position when one is set and lies on the map), offset a co-op player one unit in X and Y from
// the main player's creature (or from the holy site), tell the world map where the player is, create the CThingPlayerCreature from
// the named character def (or the player def's CharacterDef when the name is empty), face it like the holy site, and keep it as
// PlayerCharacter. The main player's creation sets the world's PlayerSpawnStatus to 1.
// Leaf: ?CreateCharacter@CPlayer@@AAE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00489D40, length 815 bytes, `ret 4`.
// Names: Ego_r.pdb / FableWin.pdb ?CreateCharacter@CPlayer@@AAE_NABVCCharString@@@Z (private), and the callees' PDB spellings:
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   CPlayer::LocateCharacterStartHolySite 0x00488B20 (ABE, const CThingHolySite**), CWorld::GetWorldMap (slot 12, +0x30),
//   CWorldMap::GetMapNumberAt(const C3DVector&) (slot 16, +0x40), CThingPhysical::PeekPos 0x006A4D00, CThing::GetPos 0x004C73D0,
//   CTCPhysicsBase::GetRHSet (+0x120) / SetRHSet (+0x80), CPlayerManager::GetMainPlayerNumber 0x00449700 / GetMainPlayer 0x00449970,
//   CInitBaseClass::IsValid 0x0099A330, CWorldMap::SetPlayerPos 0x005063E0, CCharString copy ctor 0x0099EC30 / operator=(char)
//   0x0099F040 / dtor 0x0099EAE0 / (const char*, long) 0x0099EBF0, CBasicString<char>::operator==(const char*) 0x004115A0,
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CThingPlayerCreature::Create 0x006AC910 (__fastcall),
//   NProgressDisplay::DisplayProgress 0x009E9F40 (__fastcall).
// Globals: GSetPlayerStartPos 0x013B8647 (Ego_r.pdb + FableWin.pdb data symbol), GOverridePlayerStartPos 0x013B8650 (FableWin.pdb
//   `dynamic initializer for 'GOverridePlayerStartPos'`; the C3DVector read right after the flag), 1.0f 0x0122DED8,
//   "CPlayer::CreateCharacter" 0x012380D8, CIntelligentPointer<CThingPlayerCreature> vtable 0x01231C4C.
// Shape: the start holy site comes back through a block-scoped out local copied to a register local (esi); the co-op offset,
//   map position and creation sit in one inner block with `init` declared just before it -- that is what lets the holy-site
//   slot and the def-name CCharString share esp+0Ch as retail (function-scope name: holy site moves to the temporaries' slot).
//   GetPControlledCreature 0x00487DC0 is defined out of line in this TU (its real body, `add ecx,2Ch ; jmp GetPItem`), which
//   VC7.1 /Ob2 expands as `lea ecx,[eax+2Ch]`; the physics call is slot 32 ResetRHSet (+0x80), not SetRHSet (+0x12C).
// Layout: engine/CPlayer.h (FABLE_RETAIL_PLAYER, overlay FABLE_RETAIL_PLAYER_CREATECHARACTER for the typed PlayerDef +0x21c and the
//   two private members). CPlayerDef has no shared header (only _quarantine/): a local model over CParentDefClassBase (retail 0x39,
//   so the Ego_r long CharacterDef lands at +0x3c). Retail passes CharacterDef through CCharString::operator=(char) (the implicit
//   long -> char conversion picks that overload: `xor ecx,ecx ; mov cl,[eax+3Ch] ; push ecx`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_ASSIGN_CHAR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_CREATE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_SWITCHCHARACTERS
#define FABLE_RETAIL_PLAYER_CREATECHARACTER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_SETPLAYERPOS
#include <string.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingHolySite.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingPlayerCreatureInit.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/NProgressDisplay.h"

class CPlayerDef : public CParentDefClassBase {              // RTTI .?AVCPlayerDef@@ : CParentDefClassBase (Ego_r.pdb members)
public:
    long          CharacterDef;                              // +0x3c
    unsigned char Colour[4];                                 // +0x40 CRGBColour
};

struct CThingRespawnInfo {                                   // Ego_r.pdb CThingRespawnInfo (no shared header): CPlayer::RespawnInfo +0xe8
    C3DVector DiedPosition;                                  // +0x00
    bool      DeadInfoSet;                                   // +0x0c
};

extern bool      GSetPlayerStartPos;                         // 0x013B8647
extern C3DVector GOverridePlayerStartPos;                    // 0x013B8650

CThingPlayerCreature* CPlayer::GetPControlledCreature() const
{
    return ControlledCreature.Get();
}

inline bool CPlayer::IsMainPlayer() const
{
    long number = Number;
    return number == PlayerManager->GetMainPlayerNumber();
}

bool CPlayer::CreateCharacter(const CCharString& characterDefName)
{
    ControlledCreature.Get();
    PlayerCharacter.Get();
    C3DVector pos;
    CRightHandedSet rh;
    const CThingHolySite* site;
    {
        const CThingHolySite* holySite = 0;
        if (!LocateCharacterStartHolySite(&holySite) && !GSetPlayerStartPos)
            return false;
        site = holySite;
    }
    if (GSetPlayerStartPos && World->GetWorldMap().GetMapNumberAt(GOverridePlayerStartPos)) {
        pos = GOverridePlayerStartPos;
        rh = CRightHandedSet(C3DVector(0.0f, 1.0f, 0.0f), C3DVector(0.0f, 0.0f, -1.0f));
    } else {
        pos = site->PeekPos();
        rh = site->GetPhysicsTC()->GetRHSet();
    }
    CThingPlayerCreatureInit init;
    {
        if (!IsMainPlayer()) {
            if (PlayerManager->GetMainPlayer().IsValid() && PlayerManager->GetMainPlayer().GetPControlledCreature()) {
                const C3DVector& p = PlayerManager->GetMainPlayer().GetPControlledCreature()->GetPos();
                pos = C3DVector(p.X + 1.0f, p.Y + 1.0f, p.Z);
            } else {
                const C3DVector& p = site->PeekPos();
                pos = C3DVector(p.X + 1.0f, p.Y + 1.0f, p.Z);
            }
        }
        World->GetWorldMap().SetPlayerPos(Number, pos, CWorldMap::WAIT_FOR_LOAD);
        init.OriginalThing = 0;
        init.PreviousThing = 0;
        init.PlaceAtEndOfList = false;
        CCharString name(characterDefName);
        if (name == "")
            name = PlayerDef->CharacterDef;
        long def = DefinitionManager->GetDefGlobalIndexFromName(name);
        CThingPlayerCreature* creature = CThingPlayerCreature::Create(def, pos, Number, init);
        NProgressDisplay::DisplayProgress("CPlayer::CreateCharacter", -1.0f, false, false);
        creature->GetPhysicsTC()->ResetRHSet(rh);
        PlayerCharacter = CIntelligentPointer<CThingPlayerCreature>(creature);
        reinterpret_cast<CThingRespawnInfo&>(RespawnInfo).DeadInfoSet = false;
        if (IsMainPlayer())
            World->PlayerSpawnStatus = (EPlayerSpawnStatus)1;
    }
    return true;
}