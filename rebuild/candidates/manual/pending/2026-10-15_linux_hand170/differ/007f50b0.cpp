// CTCSoundAtmosphereVillage::OnInitialActivate() -- the sound-atmosphere activation, then count the village's living spaces by summing the def value of every village building
// Grade: DIFFER(160v156) -- frame: retail one dword (`push ecx`) shared by the lookup key, the unset village local and the by-value def pointer; ours gives the def pointer a second slot (`sub esp,8`), everything else identical
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
template<class T> struct CDefPointer {
    T* Object;
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
struct CThingBuilding {
    CDefPointer<CBuildingDef> GetPBuildingDef() const;   // 0x0082D2D0
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThingBuilding>: 8 bytes
    void* m_vfptr;
    void* m_pItem;
    CThingBuilding* GetPItem() const;           // 0x00A01B50
};
struct CBuildingList {                          // std::list<CIntelligentPointer<CThingBuilding> >: one sentinel pointer
    struct node { node* next; node* prev; CBaseIntelligentPointer value; };
    node* head;
};
struct CTCVillage {
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
