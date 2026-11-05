// COpinionReactionManager::GetThingAttitudeToPlayer(const CThing* thing) const -- the thing's attitude towards the player: GetAttitudeOrReactionType in attitude mode, with the player creature's opinion-source component (if any) and the manager's attitude mask
// Leaf: GetThingAttitudeToPlayer
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00701840, length 103 bytes (padded to 112).
// Callees: CWorld::PeekMyPlayer @ 0x0049D9D0, CPlayer::PeekPControlledCreature @ 0x00487DD0,
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0,
//          COpinionReactionManager::GetAttitudeOrReactionType(const CThing*, bool, bool*, CTCOpinionSource*) @ 0x00701010.
// Layout: PDef +0, World +4; AttitudeMask (bool[79]) retail +0x554 (PDB +0x55C, the retail manager is 8 bytes smaller
// before it). The landed GetToleranceToBeingHit (0x00701940) calls this.
// Shape: `T* p = 0; if (...) p = GetTC(k);` gives the preloaded `xor ecx,ecx`; the lookup key lives in the
// single `push ecx` frame word.
class CTCBase;
class CTCOpinionSource;
#define FABLE_RETAIL_CTHING
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

class COpinionReactionManager {
public:
    const void*   PDef;                     // +0x00 CDefPointer<const COpinionReactionManagerDef>
    CWorld*       World;                    // +0x04
    unsigned char _pad008[0x54c];
    bool          AttitudeMask[79];         // +0x554 (PDB +0x55C)
    long GetAttitudeOrReactionType(const CThing* thing, bool attitudeOnly, bool* mask, CTCOpinionSource* source);   // 0x00701010
    long GetThingAttitudeToPlayer(const CThing* thing);
};

long COpinionReactionManager::GetThingAttitudeToPlayer(const CThing* thing)
{
    const CThing* creature = ((CPlayer*)((CWorld*)World)->PeekMyPlayer())->PeekPControlledCreature();
    CTCOpinionSource* source = 0;
    if (creature && creature->HasTC(TCI_OPINION_SOURCE))
        source = (CTCOpinionSource*)((CTCOpinionSource*)&creature->GetTC(TCI_OPINION_SOURCE));
    return GetAttitudeOrReactionType(thing, true, AttitudeMask, source);
}