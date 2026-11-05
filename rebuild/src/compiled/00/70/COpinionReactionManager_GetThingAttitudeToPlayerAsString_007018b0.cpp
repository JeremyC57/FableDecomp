// COpinionReactionManager::GetThingAttitudeToPlayerAsString(const CThing* thing) const -- the name of the thing's attitude to the player: from GetAttitudeOrReactionType with the player creature's opinion source when it has one, else from GetThingAttitudeToPlayer; indexes the static attitude-name string table
// Leaf: GetThingAttitudeToPlayerAsString
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007018B0, length 136 bytes (padded to 144).
// Callees: CWorld::PeekMyPlayer @ 0x0049D9D0, CPlayer::PeekPControlledCreature @ 0x00487DD0,
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0,
//          COpinionReactionManager::GetAttitudeOrReactionType @ 0x00701010, GetThingAttitudeToPlayer @ 0x00701840.
// Globals: the attitude-name table (CCharString[], 4 bytes each) @ 0x013BABD0.
// Layout: World +4, AttitudeMask retail +0x554 (PDB +0x55C).
// Shape: nested ifs (creature, component present, component non-null) all fall to the shared
// GetThingAttitudeToPlayer tail; each return forms the table address with `lea eax,[eax*4+table]`.
class CTCBase;
class CTCOpinionSource;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;        // 0x00487DD0
};
class CWorld {
public:
    CPlayer const* PeekMyPlayer() const;   // 0x0049D9D0 (Ego_r.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ)
private:             // 0x0049D9D0
};

extern CCharString g_OpinionAttitudeNames[];    // 0x013BABD0

class COpinionReactionManager {
public:
    const void*   PDef;                     // +0x00 CDefPointer<const COpinionReactionManagerDef>
    CWorld*       World;                    // +0x04
    unsigned char _pad008[0x54c];
    bool          AttitudeMask[79];         // +0x554 (PDB +0x55C)
    long GetAttitudeOrReactionType(const CThing* thing, bool attitudeOnly, bool* mask, CTCOpinionSource* source);   // 0x00701010
    long GetThingAttitudeToPlayer(const CThing* thing);     // 0x00701840
    const CCharString& GetThingAttitudeToPlayerAsString(const CThing* thing);
};

const CCharString& COpinionReactionManager::GetThingAttitudeToPlayerAsString(const CThing* thing)
{
    const CThing* creature = ((CPlayer*)((CWorld*)World)->PeekMyPlayer())->PeekPControlledCreature();
    if (creature && creature->HasTC(TCI_OPINION_SOURCE))
    {
        CTCOpinionSource* source = (CTCOpinionSource*)((CTCOpinionSource*)&creature->GetTC(TCI_OPINION_SOURCE));
        if (source)
            return g_OpinionAttitudeNames[GetAttitudeOrReactionType(thing, true, AttitudeMask, source)];
    }
    return g_OpinionAttitudeNames[GetThingAttitudeToPlayer(thing)];
}