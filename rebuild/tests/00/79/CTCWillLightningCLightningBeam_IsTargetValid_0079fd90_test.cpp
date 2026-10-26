// Behaviour test for 0079fd90: body copied from src/0079fd90.cpp (CTCWillLightning::CLightningBeam::IsTargetValid), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase { public: };
class CThing;
static int g_getPItem = 0;
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const { ++g_getPItem; return m_pItem; }
};
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
class CThing { public:
    unsigned char _pad0[0x10];
    unsigned char m_thingType;
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
static CThing* g_virtualOwner = 0; static int g_getOwnerCalls = 0;
struct CTCOwned : CTCBase {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {}
    virtual void v24() {} virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {} virtual void v29() {} virtual void v30() {} virtual void v31() {}
    virtual void v32() {}
    virtual CThing* GetOwner() { ++g_getOwnerCalls; return g_virtualOwner; }
    unsigned char _pad04[0x40 - 0x04];
    CBaseIntelligentPointer Owner;
};
struct CLightningBeam {
    long                    m_lsState;
    CBaseIntelligentPointer m_tpParticleHandle;
    CBaseIntelligentPointer m_tcpHome;
    CBaseIntelligentPointer m_tcpTarget;
    bool IsTargetValid();
};

bool CLightningBeam::IsTargetValid()
{
    if (IS_THING_ALIVE(m_tcpHome)) {
        CTCOwned* owned;
        if (m_tcpHome.GetPItem()->GetInterface(&owned, 8)) {
            CThing* owner;
            if (m_tcpHome.GetPItem()->m_thingType == 1) {
                CTCOwned* creatureOwned;
                m_tcpHome.GetPItem()->GetInterface(&creatureOwned, 8);
                owner = creatureOwned->Owner.GetPItem();
                if (owner == 0)
                    owner = owned->GetOwner();
            } else {
                owner = owned->GetOwner();
            }
            if (m_tcpTarget.GetPItem() != owner)
                return false;
        }
    }
    return true;
}
static CThing g_home, g_ownerThing, g_otherThing; static CTCOwned g_owned; static CInterfaceMap::SEntry g_entries[3];
int main()
{
    memset(&g_home, 0, sizeof(g_home));
    g_entries[0].type = 2; g_entries[1].type = 8; g_entries[1].component = &g_owned; g_entries[2].type = 0x20;
    g_home.m_interfaces.first = g_entries; g_home.m_interfaces.last = g_entries + 3;
    CLightningBeam beam; memset(&beam, 0, sizeof(beam));
    // 1: no home -> valid
    g_getPItem = 0; CHECK(beam.IsTargetValid()); CHECK(g_getPItem == 1);
    // 2: dead home -> valid
    beam.m_tcpHome.m_pItem = &g_home; g_home.m_flags91 = 1; CHECK(beam.IsTargetValid()); g_home.m_flags91 = 0;
    // 3: home without the owned component -> valid, no owner lookup
    g_getOwnerCalls = 0; CHECK(beam.IsTargetValid()); CHECK(g_getOwnerCalls == 0);
    g_home.m_presentBits[0] |= 1UL << 8;
    // 4: non-creature home: the virtual owner decides
    g_home.m_thingType = 0; g_virtualOwner = &g_ownerThing; beam.m_tcpTarget.m_pItem = &g_ownerThing;
    g_getOwnerCalls = 0; CHECK(beam.IsTargetValid()); CHECK(g_getOwnerCalls == 1);
    beam.m_tcpTarget.m_pItem = &g_otherThing; CHECK(!beam.IsTargetValid());
    // 5: creature home with an Owner pointer: it wins over the virtual owner
    g_home.m_thingType = 1; g_owned.Owner.m_pItem = &g_otherThing; g_getOwnerCalls = 0;
    CHECK(beam.IsTargetValid()); CHECK(g_getOwnerCalls == 0);
    beam.m_tcpTarget.m_pItem = &g_ownerThing; CHECK(!beam.IsTargetValid());
    // 6: creature home with a null Owner pointer: falls back to the virtual owner
    g_owned.Owner.m_pItem = 0; g_getOwnerCalls = 0; CHECK(beam.IsTargetValid()); CHECK(g_getOwnerCalls == 1);
    beam.m_tcpTarget.m_pItem = 0; CHECK(!beam.IsTargetValid());
    if (fails == 0) printf("0079fd90_TEST PASS\n"); else printf("0079fd90_TEST FAIL\n");
    return fails;
}