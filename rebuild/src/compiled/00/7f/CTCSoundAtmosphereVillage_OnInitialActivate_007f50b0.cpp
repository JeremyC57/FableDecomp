// CTCSoundAtmosphereVillage::OnInitialActivate() -- the sound-atmosphere activation, then count the village's living spaces by summing the def value of every village building
// Leaf: vfunc_6
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F50B0, length 156 bytes (padded row 160); CTCSoundAtmosphereVillage vtable 0x0124111C
// slot 6 ("vfunc_6" on CTC classes is CTCBase::OnInitialActivate; the source names it vfunc_6 to keep the catalog leaf).
// Callees: CTCSoundAtmosphere::OnInitialActivate @ 0x007F53A0 (base vtable 0x01271DFC slot 6; GetSubDef into PDef +0x10; manifest label
// "CTCInventoryAbilities::AddLearnedExpression" is a byte twin), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined
// lookup of the village component, interface 0x22 = bit 2 of CThing+0x24), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThingBuilding
// 0x0082D2D0 (returns the building's CDefPointer by value through the hidden pointer; manifest "CThingBuilding::PeekPMaterial"), the def's
// virtual slot 1 (destroy).
// Layout: Ego_r.pdb CTCSoundAtmosphereVillage (0x1C): TimeUntilNextUpdate +0x0C, PDef +0x10, NumLivingSpaces +0x14, NumVillageMembers +0x18.
// CTCVillage::VillageBuildings (list<CIntelligentPointer<CThingBuilding> >, one sentinel pointer) +0x28; building def value +0x44.
// Shape: the village comes from an inline accessor returning an unset local on the absent path (retail reads the dead frame word); the def
// pointer is a by-value result whose dtor (`if (Object) { Object->ReleaseTemporary(); Object = 0; }`, DecRef/GetRefCount on the ref-count
// base at +4) merges with the `if (def.Object)` test; the GetPItem call sits between the hidden-pointer push and the call.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
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
#pragma auto_inline(on)
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CBuildingDef : CRefVirtual, CRefCounted {
    unsigned char _pad08[0x44 - 0x08];
    long LivingSpaces;                          // +0x44
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template<class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
class CThingBuilding { public:
    CDefPointer<CBuildingDef> GetPBuildingDef() const;   // 0x0082D2D0
};
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
struct CBuildingList {                          // std::list<CIntelligentPointer<CThingBuilding> >: one sentinel pointer
    struct node { node* next; node* prev; CIntelligentPointer<CThingBuilding> value; };
    node* head;
};
class CTCVillage { public:
    unsigned char _pad00[0x28];
    CBuildingList VillageBuildings;             // +0x28
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
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CTCSoundAtmosphere {
    void* __vfptr;                              // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    unsigned char _base08[4];
    float TimeUntilNextUpdate;                  // +0x0c
    void* PDef;                                 // +0x10 CDefPointer<const CSoundAtmospheresDef>
    void OnInitialActivate();                   // 0x007F53A0
};
struct CTCSoundAtmosphereVillage : CTCSoundAtmosphere {
    long NumLivingSpaces;                       // +0x14
    long NumVillageMembers;                     // +0x18
    __forceinline CTCVillage* GetVillage()
    {
        CTCVillage* village;
        if (ParentThing->HasTC(0x22))
            village = (CTCVillage*)ParentThing->GetTC(0x22);
        return village;
    }
    void vfunc_6();
};

void CTCSoundAtmosphereVillage::vfunc_6()
{
    CTCSoundAtmosphere::OnInitialActivate();
    CTCVillage* village = GetVillage();
    NumLivingSpaces = 0;
    for (CBuildingList::node* n = village->VillageBuildings.head->next; n != village->VillageBuildings.head; n = n->next) {
        CDefPointer<CBuildingDef> def = n->value.GetPItem()->GetPBuildingDef();
        if (def.Object)
            NumLivingSpaces += def.Object->LivingSpaces;
    }
}