// Grade: DIFFER(527v553) -- region def pointer held in eax (retail ecx); the early 'return false' exits share the main exit (retail keeps a separate pre-push exit); the two seat-claim tails merge fully
// CAIStateGroup_EatAtTable::GetSeat() -- set the eating duration from the region's definition (plus up to 45 random), then claim an eating point (or a seat) at the first usable tavern table in the building
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096CB40, 560 bytes.
// Callees: the state group's virtual slot 14 (+0x38, current building), CVectorMap::LowerBound 0x0040F020 / const
//   0x004365B0 (same-TU stand-ins; inlined GetInterface 0x21, 0x23 (const), 0xB8), the region accessor 0x007EE540,
//   CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0 (__fastcall), CBaseIntelligentPointer::GetPItem
//   0x00A01B50 / dtor 0x00A01C10, CTCTavernTable::GetEatingPoint 0x007B8B10 / GetSeat 0x007B8840,
//   CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760.
// Layout: CAIStateGroup_EatAtTable Table +0x14, SeatIndex +0x1c, SeatActionPointIndex +0x20, SeatPos +0x24, SeatRHSet
// +0x30, Duration +0x48 (Ego_r.pdb); retail CThing interface bitset +0x20 / map +0x44, flags +0x34 / +0x91.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { float M[6]; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
float __fastcall GFFloatRandom(unsigned long* seed, float range);   // 0x004978A0

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead, bit 5
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item);     // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CPointerListNode {
    CPointerListNode* _M_next;
    CPointerListNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};
struct CTCTableList {                           // interface 0x21
    unsigned char _pad00[0x0c];
    CPointerListNode* _M_node;                  // +0x0c STLport list
};
struct CEatDef {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void Release() { RefCount = RefCount - 1; if (RefCount == 0) DeleteThis(); }
    unsigned char _pad08[0x90 - 0x08];
    long EatingTime;                            // +0x90
};
struct CEatDefPointer {
    CEatDef* m_pDef;
    CEatDefPointer(CEatDef* def) : m_pDef(def) { if (m_pDef) m_pDef->AddRef(); }
    ~CEatDefPointer() { m_pDef->Release(); }
    CEatDef* operator->() const { return m_pDef; }
};
struct CRegionThing {
    unsigned char _pad00[0x94];
    CEatDef* PDef;                              // +0x94
};
struct CTCRegionLink {                          // interface 0x23
    CRegionThing* GetPRegion();                 // 0x007EE540
};
struct CTCTavernTable {                         // interface 0xB8
    bool GetEatingPoint(CThing* creature, C3DVector* pos, CRightHandedSet* set, long* actionPoint, long* seat);   // 0x007B8B10
    bool GetSeat(CThing* creature, C3DVector* pos, CRightHandedSet* set, long* actionPoint, long* seat, bool flag);   // 0x007B8840
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThing* Creature;                           // +0x18
};
struct CAIStateGroupBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34();
    virtual CThing* GetPBuilding();             // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThing* GetCreature() const { return PBrain->Creature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
};
struct CAIStateGroup_EatAtTable : CAIStateGroupBase {
    CIntelligentPointerThing Table;             // +0x14
    long SeatIndex;                             // +0x1c
    long SeatActionPointIndex;                  // +0x20
    C3DVector SeatPos;                          // +0x24
    CRightHandedSet SeatRHSet;                  // +0x30
    float Duration;                             // +0x48
    bool GetSeat();
};

bool CAIStateGroup_EatAtTable::GetSeat()
{
    CThing* building = GetPBuilding();
    if (!building || (building->m_flags91 & 1) || !building->HasTC(0x21))
        return false;
    CTCTableList* tables = (CTCTableList*)building->GetTC(0x21);
    const CThing* creature = GetCreature();
    if (creature->HasTC(0x23)) {
        CTCRegionLink* link = (CTCRegionLink*)creature->GetTC(0x23);
        if (link->GetPRegion()) {
            {
                CEatDefPointer def(link->GetPRegion()->PDef);
                Duration = (float)def->EatingTime;
            }
            Duration += GFFloatRandom(GetPWorldSeed(), 45.0f);
        }
    }
    for (CPointerListNode* node = tables->_M_node->_M_next; node != tables->_M_node; node = node->_M_next) {
        CBaseIntelligentPointer& table = node->_M_data;
        if (IS_THING_ALIVE(table) && !(table.GetPItem()->m_flags91 & 0x20)) {
            CThing* thing = table.GetPItem();
            if (!thing->HasTC(0xb8))
                continue;
            CTCTavernTable* tavernTable = (CTCTavernTable*)thing->GetTC(0xb8);
            if (tavernTable->GetEatingPoint(GetCreature(), &SeatPos, &SeatRHSet, &SeatActionPointIndex, &SeatIndex)) {
                Table = CIntelligentPointerThing(node->_M_data.GetPItem());
                return true;
            }
            if (tavernTable->GetSeat(GetCreature(), &SeatPos, &SeatRHSet, &SeatActionPointIndex, &SeatIndex, false)) {
                Table = CIntelligentPointerThing(node->_M_data.GetPItem());
                return true;
            }
        }
    }
    return false;
}
