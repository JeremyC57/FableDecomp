// CThingFilter_IsValidGossipee::CThingFilter_IsValidGossipee(CThingAICreature* thing) -- records the gossiping creature, its faction (from the kind-0x49 component) and its village (from the village-member component, kind 0x23)
// Leaf: ??0CThingFilter_IsValidGossipee
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090C870, length 135 bytes (`ret 4`, + int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (two inlined GetTC lookups: kind 0x49 = bit 9 of
// CThing+0x28, kind 0x23 = bit 3 of CThing+0x24), the CTCVillageMember village getter @ 0x007EE540 (thiscall on the
// component; manifest "IS_THING_ALIVE<CIntelligentPointer<CThingPhysical const>>" is a propagated label -- the body reads the
// member's CIntelligentPointer at +0x10).
// Layout: PDB CThingFilter_IsValidGossipee { CThingAICreature* Thing (+0); CFaction* PFaction (+4); CTCVillage* PTCVillage (+8); };
// the kind-0x49 component's faction pointer at +0x18 (local model); CThing component bits +0x20, interface map +0x44.
// Shape: init list Thing(thing), PFaction(0), PTCVillage(0) (one `xor eax,eax`), then two out-parameter __forceinline GetInterface
// lookups (HasTC + GetTC picks the wrong registers); both keys are homed in the dead thing argument slot.
class CTCBase;
class CFaction;
class CTCVillage;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long kind)
    {
        if (m_presentBits[kind >> 5] & (1UL << (kind & 31))) {
            long type = kind;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || kind < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CThingAICreature : public CThing {};
struct CTCFactionMember {                       // component kind 0x49
    unsigned char _pad00[0x18];
    CFaction*     PFaction;                     // +0x18
};
struct CTCVillageMember {                       // component kind 0x23
    CTCVillage* GetPTCVillage() const;          // 0x007EE540
};
struct CThingFilter_IsValidGossipee {
    CThingAICreature* Thing;                    // +0x00
    CFaction*         PFaction;                 // +0x04
    CTCVillage*       PTCVillage;               // +0x08
    CThingFilter_IsValidGossipee(CThingAICreature* thing);
};

CThingFilter_IsValidGossipee::CThingFilter_IsValidGossipee(CThingAICreature* thing)
    : Thing(thing), PFaction(0), PTCVillage(0)
{
    CTCFactionMember* faction;
    if (thing->GetInterface(&faction, 0x49))
        PFaction = faction->PFaction;
    CTCVillageMember* member;
    if (thing->GetInterface(&member, 0x23))
        PTCVillage = member->GetPTCVillage();
}
