// Behaviour test for CTCShopKeeper::FrameUpdate (0x00799AA0): nothing happens off the 15-frame beat (frame + unique id);
// on it a new customer is acquired when needed and the opinions follow the village's hero-criminal state.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __int64 long long
#endif
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    unsigned __int64 UniqueID;
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
static long g_frame;
class CWorld { public:
    static long GetWorldFrame() { return g_frame; }
};
class CTCVillage { public:
    bool criminal;
    bool IsHeroCriminal() { return criminal; }
};
class CTCVillageMember { public:
    CTCVillage* village;
    CTCVillage* GetPVillage() const { return village; }
};
class CTCShop { public:
    long StockState;
};
class CTCShopKeeper { public:
    virtual void v00() { }
    CThing* ParentThing;
    bool RepressedOpinions;
    CTCShop* shop; bool needs; int acquired; int repressCalls; bool lastRepress;
    CTCShop* GetShopTC() const { return shop; }
    bool RequiresNewCustomer() { return needs; }
    void RepressOpinions(bool repress) { ++repressCalls; lastRepress = repress; RepressedOpinions = repress; }
    void AcquireNewCustomer() { ++acquired; }
    void FrameUpdate();
};

void CTCShopKeeper::FrameUpdate()
{
    unsigned __int64 id = ParentThing->UniqueID;
    if ((CWorld::GetWorldFrame() + id) % 15 == 0) {
        if (GetShopTC() && RequiresNewCustomer())
            AcquireNewCustomer();
        if (ParentThing->HasTC(0x23)) {
            CTCVillage* village = ((CTCVillageMember*)ParentThing->GetTC(0x23))->GetPVillage();
            if (village) {
                if (village->IsHeroCriminal()) {
                    if (RepressedOpinions)
                        RepressOpinions(false);
                } else if (!RepressedOpinions && GetShopTC() && GetShopTC()->StockState == 0)
                    RepressOpinions(true);
            }
        }
    }
}

int main()
{
    int bad = 0;
    CTCVillage v; v.criminal = false;
    CTCVillageMember m; m.village = &v;
    CInterfaceMap::SEntry e[1] = { { 0x23, (CTCBase*)&m } };
    CThing thing; std::memset(thing.m_presentBits, 0, sizeof(thing.m_presentBits));
    thing.UniqueID = ((unsigned __int64)1 << 32) | 4;                 // (frame + id) % 15 == 0 when frame == 11 (2^32 % 15 == 1)
    thing.m_presentBits[0x23 >> 5] = 1UL << (0x23 & 31); thing.m_interfaces.first = e; thing.m_interfaces.last = e + 1;
    CTCShop shop; shop.StockState = 0;
    CTCShopKeeper k; k.ParentThing = &thing; k.RepressedOpinions = false; k.shop = &shop; k.needs = true;
    k.acquired = 0; k.repressCalls = 0;
    g_frame = 12; k.FrameUpdate();
    if (k.acquired || k.repressCalls) bad |= 1;
    g_frame = 10; k.FrameUpdate();
    if (k.acquired != 1 || k.repressCalls != 1 || !k.lastRepress) bad |= 2;     // honest hero, stocked shop: repress
    k.FrameUpdate();                                   // already repressed: no second call
    if (k.acquired != 2 || k.repressCalls != 1) bad |= 4;
    v.criminal = true; k.needs = false; k.FrameUpdate();
    if (k.acquired != 2 || k.repressCalls != 2 || k.lastRepress) bad |= 8;     // criminal: release
    v.criminal = false; shop.StockState = 1; k.FrameUpdate();
    if (k.repressCalls != 2) bad |= 16;                // not stock state 0: stays released
    shop.StockState = 0; k.shop = 0; k.needs = true; k.FrameUpdate();
    if (k.repressCalls != 2 || k.acquired != 2) bad |= 32;
    if (bad) { std::printf("00799aa0_TEST FAIL\n"); return 1; }
    std::printf("00799aa0_TEST PASS\n");
    return 0;
}