// Grade: DIFFER(345v345) -- only the esi/edi assignment of the gossipee and the map/home-building temporaries is swapped
// CThingFilter_IsValidGossipee::operator()(const CThingAICreature&) const -- a gossip partner other than the gossiper itself, of the required faction (or factionless when none is required) and village (or village-less), and, during the village's curfew, sharing the gossiper's home building and standing inside it (or both homeless)
// Leaf: ??RCThingFilter_IsValidGossipee
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090C900, length 345 bytes (+7 int3), `ret 4`.
//   Name from Ego_r.pdb (`??RCThingFilter_IsValidGossipee@@QBE_NABVCThingAICreature@@@Z`).
// Callees: the const CVectorMap LowerBound @ 0x004365B0, CTCVillageMember::GetPVillage @ 0x007EE540,
//   CTCVillage::IsCurfewTime @ 0x00658D5D, CIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition const
//   @ 0x004C73D0, CThingBuilding::IsPointInside(const C3DVector&) const @ 0x0082D000.
// Layout: filter (Ego_r.pdb, matches retail): Thing +0, PFaction +4, PTCVillage +8. CThing (retail) bitset +0x20
//   (0x23 CTCVillageMember = bit 3 of +0x24, 0x49 CTCEnemy = bit 9 of +0x28), map +0x44. CTCEnemy::PFaction +0x18
//   (PDB +0x28; the two std::list members are 8 bytes each in retail). CThingAICreature::HomeBuilding +0x168
//   (PDB +0x190; the CIntelligentPointer<CThingBuilding> that IsPointInside is called on).
struct CTCBase { void* vfptr; struct CThing* ParentThing; };
struct C3DVector { float X, Y, Z; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

struct CThingBuilding : CThing {
    bool IsPointInside(const C3DVector& point) const;   // 0x0082D000
};
struct CIntelligentPointerBuilding {
    void* Ptr;
    CThingBuilding* GetPItem() const;                   // 0x00A01B50
};
struct CThingAICreature : CThing {
    unsigned char _pad4c[0x168 - 0x4c];
    CIntelligentPointerBuilding HomeBuilding;           // +0x168
};

struct CFaction;
struct CTCEnemy : CTCBase {
    unsigned char _pad08[0x18 - 8];
    CFaction*     PFaction;                             // +0x18
};
struct CTCVillage : CTCBase {
    bool IsCurfewTime() const;                          // 0x00658D5D
};
struct CTCVillageMember : CTCBase {
    CTCVillage* GetPVillage() const;                    // 0x007EE540
};

struct CThingFilter_IsValidGossipee {
    CThingAICreature* Thing;                            // +0
    CFaction*         PFaction;                         // +4
    CTCVillage*       PTCVillage;                       // +8
    bool operator()(const CThingAICreature& thing) const;
};

bool CThingFilter_IsValidGossipee::operator()(const CThingAICreature& thing) const
{
    CFaction* faction = PFaction;
    if (faction) {
        if (!thing.HasTC(0x49) || faction != ((const CTCEnemy*)thing.GetTC(0x49))->PFaction)
            return false;
    } else if (thing.HasTC(0x49))
        return false;
    if (PTCVillage) {
        if (!thing.HasTC(0x23) || PTCVillage != ((const CTCVillageMember*)thing.GetTC(0x23))->GetPVillage())
            return false;
    } else if (thing.HasTC(0x23) && ((const CTCVillageMember*)thing.GetTC(0x23))->GetPVillage())
        return false;
    if (PTCVillage && !PTCVillage->IsCurfewTime()) {
        CThingBuilding* home = thing.HomeBuilding.GetPItem();
        if (home)
            return &thing != Thing && Thing->HomeBuilding.GetPItem() == home && home->IsPointInside(thing.GetPosition());
        return &thing != Thing && !Thing->HomeBuilding.GetPItem();
    }
    return &thing != Thing;
}
