// Behaviour test for 007f50b0: body copied from src/007f50b0.cpp (CTCSoundAtmosphereVillage::vfunc_6 = OnInitialActivate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
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
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
static int g_destroyed = 0;
struct CRefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
};
struct CBuildingDef : CRefVirtual, CRefCounted {
    unsigned char _pad08[0x44 - 0x08];
    long LivingSpaces;
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
static int g_getDef = 0;
struct CThingBuilding {
    CBuildingDef* m_pDef;
    CDefPointer<CBuildingDef> GetPBuildingDef() const
    {
        ++g_getDef;
        CDefPointer<CBuildingDef> r; r.Object = m_pDef; if (m_pDef) ++m_pDef->m_count;
        return r;
    }
};
struct CBaseIntelligentPointer {
    void* m_vfptr;
    void* m_pItem;
    CThingBuilding* GetPItem() const { return (CThingBuilding*)m_pItem; }
};
struct CBuildingList {
    struct node { node* next; node* prev; CBaseIntelligentPointer value; };
    node* head;
};
struct CTCVillage {
    unsigned char _pad00[0x28];
    CBuildingList VillageBuildings;
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
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
static int g_baseActivate = 0;
struct CTCSoundAtmosphere {
    void* __vfptr;
    CThing* ParentThing;
    unsigned char _base08[4];
    float TimeUntilNextUpdate;
    void* PDef;
    void OnInitialActivate() { ++g_baseActivate; }
};
struct CTCSoundAtmosphereVillage : CTCSoundAtmosphere {
    long NumLivingSpaces;
    long NumVillageMembers;
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
int main()
{
    static CThing thing; static CTCVillage village; static CInterfaceMap::SEntry entries[2]; static CBuildingDef defA, defB;
    static CThingBuilding bA, bB, bNone; static CBuildingList::node head, n[3];
    memset(&thing, 0, sizeof(thing)); memset(&village, 0, sizeof(village));
    entries[0].type = 0x22; entries[0].component = (CTCBase*)&village; entries[1].type = 0x30;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2; thing.m_presentBits[1] |= 1UL << (0x22 & 31);
    defA.m_count = 1; defA.LivingSpaces = 3; defB.m_count = 1; defB.LivingSpaces = 4;
    bA.m_pDef = &defA; bB.m_pDef = &defB; bNone.m_pDef = 0;
    village.VillageBuildings.head = &head; head.next = &n[0]; n[0].next = &n[1]; n[1].next = &n[2]; n[2].next = &head;
    n[0].value.m_pItem = &bA; n[1].value.m_pItem = &bNone; n[2].value.m_pItem = &bB;
    CTCSoundAtmosphereVillage tc; memset(&tc, 0, sizeof(tc)); tc.ParentThing = &thing; tc.NumLivingSpaces = 99;
    tc.vfunc_6();
    CHECK(g_baseActivate == 1); CHECK(g_getDef == 3); CHECK(tc.NumLivingSpaces == 7);
    CHECK(defA.m_count == 1 && defB.m_count == 1); CHECK(g_destroyed == 0);
    // a def held only by the temporary is destroyed when it is released
    defA.m_count = 0; g_getDef = 0; head.next = &n[0]; n[0].next = &head;
    tc.vfunc_6();
    CHECK(tc.NumLivingSpaces == 3); CHECK(g_destroyed == 1); CHECK(g_baseActivate == 2);
    // empty village
    head.next = &head; tc.vfunc_6(); CHECK(tc.NumLivingSpaces == 0);
    if (fails == 0) printf("007f50b0_TEST PASS\n"); else printf("007f50b0_TEST FAIL\n");
    return fails;
}