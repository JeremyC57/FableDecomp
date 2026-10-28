// COpinionReactionManager::GetThingAttitudeToPlayer(const CThing* thing) const -- the thing's attitude towards the player: GetAttitudeOrReactionType in attitude mode, with the player creature's opinion-source component (if any) and the manager's attitude mask
// Leaf: GetThingAttitudeToPlayer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00701840, length 103 bytes (padded to 112).
// Callees: CWorld::GetCurrentPlayer @ 0x0049D9D0, CPlayer::PeekPControlledCreature @ 0x00487DD0,
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0,
//          COpinionReactionManager::GetAttitudeOrReactionType(const CThing*, bool, bool*, CTCOpinionSource*) @ 0x00701010.
// Layout: PDef +0, World +4; AttitudeMask (bool[79]) retail +0x554 (PDB +0x55C, the retail manager is 8 bytes smaller
// before it). The landed GetToleranceToBeingHit (0x00701940) calls this.
// Shape: `T* p = 0; if (...) p = GetTC(k);` gives the preloaded `xor ecx,ecx`; the lookup key lives in the
// single `push ecx` frame word.
class CTCBase;
class CTCOpinionSource;
struct CInterfaceMap {                 // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;   // 0x004365B0 (a second copy of the 0x0040F020 body)
};
class CThing { public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];         // +0x20 bitset of the interface types present
    CInterfaceMap m_interfaces;             // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;        // 0x00487DD0
};
class CWorld { public:
    CPlayer* GetCurrentPlayer();            // 0x0049D9D0
};

enum { TC_OPINION_SOURCE = 0x50 };

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
    const CThing* creature = World->GetCurrentPlayer()->PeekPControlledCreature();
    CTCOpinionSource* source = 0;
    if (creature && creature->HasTC(TC_OPINION_SOURCE))
        source = (CTCOpinionSource*)creature->GetTC(TC_OPINION_SOURCE);
    return GetAttitudeOrReactionType(thing, true, AttitudeMask, source);
}