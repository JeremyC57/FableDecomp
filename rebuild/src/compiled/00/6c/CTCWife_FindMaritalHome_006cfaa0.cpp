// CTCWife::FindMaritalHome() const -- the wife's village (her own village membership, else the first thing found with a village component), then the first live building in it whose buyable house is owned by the player, not rented, not in use and has several dress levels; null when none
// Leaf: FindMaritalHome
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CFAA0, length 348 bytes (plain `ret`, no frame pointer; Ego_r.pdb
//   `?FindMaritalHome@CTCWife@@ABEPAVCThingBuilding@@XZ`).
// Callees: CVectorMap LowerBound const 0x004365B0 (inlined const GetTC: interfaces 0x23 and 0xD5) and non-const 0x0040F020
//   (interface 0x22); CTCVillageMember::GetPVillage 0x007EE540 (manifest "IS_THING_ALIVE<...>" propagated, as the landed
//   GetSleepTimes 0x00833390); CTCBase::GetThingSearchTools 0x00686D90 (manifest "_Cons_val<...>" propagated);
//   CThingSearchTools::GetPFirstThing<filter, CThing>(filter*, CThing**) 0x006D3FB0 (the manifest names the
//   HasDefGlobalIndex filter; here a one-long CThingFilter_HasTC holding 0x22); CBaseIntelligentPointer::GetPItem 0x00A01B50;
//   CTCBuyableHouse::IsHouseBeingUsed(bool) 0x006BFB10, CTCBuyableHouse::HasMultipleDressLevels() 0x006BF7F0.
// Layout: CTCBase ParentThing +0x04; CThing bitset +0x20 (0x22/0x23 = bits 2/3 of byte +0x24, 0xD5 = bit 21 of word +0x38),
//   interface map +0x44, dead bit +0x91; retail CTCVillage VillageBuildings +0x28 (4-byte STLport list, landed
//   siblings); CTCBuyableHouse OwnedByPlayer +0x11, Rented +0x12 (PDB).
// Shape: the member component is fetched once and asked for its village twice; the village starts null and the search
//   path sets it only on success (all failures then share the one return-0 epilogue, ebp pushed in the prologue); the
//   filter is a temporary passed by const reference (PDB `QBE_NABVCThingFilter_HasTC@@`), so its slot is free again for
//   the later GetTC keys (a named filter costs a third frame slot); the list head is cached in a local (ebx).
class CTCBase;
class CTCVillage;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    template <class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (const T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CThingBuilding : public CThing { };

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct SBuildingNode {                          // STLport list node of list<CIntelligentPointer<CThingBuilding> >
    SBuildingNode*          _M_next;
    SBuildingNode*          _M_prev;
    CIntelligentPointer<CThing> _M_data;            // +0x08
};
class CTCVillage {
public:
    unsigned char  _pad_0x00[0x28];
    SBuildingNode* VillageBuildings;            // +0x28 head node
};
class CTCVillageMember { public:                       // interface 0x23
    CTCVillage* GetPVillage() const;            // 0x007EE540
};
class CTCBuyableHouse { public:                        // interface 0xD5
    unsigned char _pad_0x00[0x11];
    bool          OwnedByPlayer;                // +0x11
    bool          Rented;                       // +0x12
    bool IsHouseBeingUsed(bool flag) const;     // 0x006BFB10
    bool HasMultipleDressLevels() const;        // 0x006BF7F0
};

class CThingFilter_HasTC { public:                     // one-long interface filter (descriptive)
    long Type;
    CThingFilter_HasTC(long type) : Type(type) { }
};
class CThingSearchTools { public:
    template <class F, class T> bool GetPFirstThing(const F& filter, T** result) const;   // 0x006D3FB0
};

class CTCBase
{
public:
    virtual void v00();
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x00686D90
};

class CTCWife : public CTCBase { public:
    CThing* ParentThing;                        // +0x04
    CThingBuilding* FindMaritalHome() const;
};

CThingBuilding* CTCWife::FindMaritalHome() const
{
    CTCVillage* village = 0;
    const CThing* parent = ParentThing;
    const CTCVillageMember* member;
    if (parent->HasTC(0x23) && (member = parent->GetTC<CTCVillageMember>(0x23))->GetPVillage() != 0)
        village = member->GetPVillage();
    else {
        CThing* thing = 0;
        if (PeekThingSearchTools().GetPFirstThing(CThingFilter_HasTC(0x22), &thing) && thing != 0 && thing->HasTC(0x22))
            village = thing->GetTC<CTCVillage>(0x22);
    }
    if (village != 0) {
        SBuildingNode* head = village->VillageBuildings;
        for (SBuildingNode* node = head->_M_next; node != head; node = node->_M_next) {
            const CThing* building = node->_M_data.GetPItem();
            if (building != 0 && !(building->m_flags & 1) && building->HasTC(0xd5)) {
                const CTCBuyableHouse* house = building->GetTC<CTCBuyableHouse>(0xd5);
                if (house->OwnedByPlayer && !house->Rented && !house->IsHouseBeingUsed(false) && house->HasMultipleDressLevels())
                    return (CThingBuilding*)building;
            }
        }
    }
    return 0;
}