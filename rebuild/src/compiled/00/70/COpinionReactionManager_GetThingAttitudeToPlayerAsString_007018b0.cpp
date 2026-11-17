// COpinionReactionManager::GetThingAttitudeToPlayerAsString(const CThing& thing) const -- the name of the thing's attitude to the player: from GetAttitudeOrReactionType with the player creature's opinion source when it has one, else from GetThingAttitudeToPlayer; indexes the static attitude-name string table
// Leaf: ?GetThingAttitudeToPlayerAsString@COpinionReactionManager@@QBEABVCCharString@@ABVCThing@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007018B0, length 136 bytes (padded to 144).
// Callees: CWorld::PeekMyPlayer @ 0x0049D9D0, CPlayer::PeekPControlledCreature @ 0x00487DD0,
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0,
//          COpinionReactionManager::GetAttitudeOrReactionType @ 0x00701010, GetThingAttitudeToPlayer @ 0x00701840.
// Globals: the attitude-name table (CCharString[], 4 bytes each) @ 0x013BABD0.
// Layout: World +4, AttitudeMask retail +0x554 (PDB +0x55C).
// Shape: nested ifs (creature, component present, component non-null) all fall to the shared
// GetThingAttitudeToPlayer tail; each return forms the table address with `lea eax,[eax*4+table]`.
// NF52 (round 7): moved onto the shared headers. The table is the private static COpinionReactionManager::AttitudeStrings
// (?AttitudeStrings@COpinionReactionManager@@0QBVCCharString@@B, 0x013BABD0; both PDBs' dynamic initializer / atexit destructor,
// engine/COpinionReactionManager.h FABLE_RETAIL_GREETING_PASS23), read directly by its own class. The self signature is the PDB's:
// Ego_r.pdb 0x00551D70 / FableWin.pdb 0x01F16020 ?GetThingAttitudeToPlayerAsString@COpinionReactionManager@@QBEABVCCharString@@ABVCThing@@@Z
// (const, const CThing&; the row spelled (const CThing*) non-const), declared with the private GetAttitudeOrReactionType by the overlay
// opt-in FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYERASSTRING; GetThingAttitudeToPlayer is the header's ATTITUDETOPLAYER opt-in.
// World +0x04 / AttitudeMask +0x554 are the header's members. CWorld::PeekMyPlayer / CPlayer::PeekPControlledCreature /
// CThingPlayerCreature come from engine/CWorld.h / CPlayer.h / CThingPlayerCreature.h. Were local COpinionReactionManager / CWorld /
// CPlayer / CThingPlayerCreature models and the made-up extern g_OpinionAttitudeNames.
class CTCBase;
class CTCOpinionSource;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYER
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYERASSTRING
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EOpinion.h"
#include "engine/EOpinionAttitudeType.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#include "engine/CThingPlayerCreature.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
enum EOpinionReactionType;                  // MS forward enum (Ego_r.pdb result type)
#include "engine/COpinionReactionManager.h"

const CCharString& COpinionReactionManager::GetThingAttitudeToPlayerAsString(const CThing& thing) const
{
    const CThing* creature = World->PeekMyPlayer()->PeekPControlledCreature();
    if (creature && creature->HasTC(TCI_OPINION_SOURCE))
    {
        CTCOpinionSource* source = (CTCOpinionSource*)((CTCOpinionSource*)&creature->GetTC(TCI_OPINION_SOURCE));
        if (source)
            return AttitudeStrings[GetAttitudeOrReactionType(thing, true, AttitudeMask, source)];
    }
    return AttitudeStrings[GetThingAttitudeToPlayer(thing)];
}
