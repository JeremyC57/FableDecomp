// COpinionReactionManager::GetThingAttitudeToPlayerAsString(const CThing* thing) const -- the name of the thing's attitude to the player: from GetAttitudeOrReactionType with the player creature's opinion source when it has one, else from GetThingAttitudeToPlayer; indexes the static attitude-name string table
// Leaf: GetThingAttitudeToPlayerAsString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007018B0, length 136 bytes (padded to 144).
// Callees: CWorld::GetCurrentPlayer @ 0x0049D9D0, CPlayer::PeekPControlledCreature @ 0x00487DD0,
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0,
//          COpinionReactionManager::GetAttitudeOrReactionType @ 0x00701010, GetThingAttitudeToPlayer @ 0x00701840.
// Globals: the attitude-name table (CCharString[], 4 bytes each) @ 0x013BABD0.
// Layout: World +4, AttitudeMask retail +0x554 (PDB +0x55C).
// Shape: nested ifs (creature, component present, component non-null) all fall to the shared
// GetThingAttitudeToPlayer tail; each return forms the table address with `lea eax,[eax*4+table]`.
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
class CCharString { public:
    void* m_pData;
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
    const CThing* creature = World->GetCurrentPlayer()->PeekPControlledCreature();
    if (creature && creature->HasTC(TC_OPINION_SOURCE))
    {
        CTCOpinionSource* source = (CTCOpinionSource*)creature->GetTC(TC_OPINION_SOURCE);
        if (source)
            return g_OpinionAttitudeNames[GetAttitudeOrReactionType(thing, true, AttitudeMask, source)];
    }
    return g_OpinionAttitudeNames[GetThingAttitudeToPlayer(thing)];
}