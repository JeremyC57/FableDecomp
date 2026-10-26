// Behaviour test for 0082acd0: body copied from src/0082acd0.cpp (CMiniMapDisplay::InitialiseHouseMarkers), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase { public: };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
    CTCBase* Find(long kind) const
    {
        long key = kind;
        const SEntry* it = LowerBound(&key);
        return ((it == last || kind < it->type) ? last : it)->component;
    }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x8e - 0x4c];
    short         MapIndex;
    long GetMapIndex() const { return MapIndex; }
    unsigned char m_flags90;
    unsigned char m_flags91;
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template <class T> __forceinline const T* GetTC(long interfaceType) const
    {
        return (const T*)m_interfaces.Find(interfaceType);
    }
};
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> struct list { _List_node<T>* _M_node; };
static int g_regionCalls = 0, g_currentCalls = 0;
class CWorldMap { public:
    long GetRegionNumberMapIsIn(long mapIndex) { ++g_regionCalls; return mapIndex / 10; }
    long GetCurrentRegionNumber() { ++g_currentCalls; return 3; }
};
struct SMarkerGraphics { long Primary; long Secondary; };
struct CArray_MarkerGraphics {
    SMarkerGraphics* m_pBegin;
    SMarkerGraphics* m_pEnd;
    SMarkerGraphics& operator[](unsigned long i) { return m_pBegin[i]; }
};
class CPlayerGuiDef { public: unsigned char _pad00[0x958]; CArray_MarkerGraphics MiniMapMarkerGraphics; };
static CPlayerGuiDef g_guiDef; static SMarkerGraphics g_graphics[8];
CPlayerGuiDef* __fastcall PeekGuiDef() { return &g_guiDef; }
static int g_forSaleCalls = 0;
class CTCBuyableHouse : public CTCBase { public:
    unsigned char _pad00[0x11];
    bool          OwnedByHero;
    bool          ForSale;
    bool IsForSale() const { ++g_forSaleCalls; return ForSale; }
};
struct CThingLists { unsigned char _pad00[0x3c]; list<CThing*> Houses; };
class CThingManager { public: void* __vftable; CThingLists* Lists; };
static CThingManager g_thingManager; static CWorldMap g_worldMap;
class CWorld { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual CThingManager* GetThingManager() { return &g_thingManager; }
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {}
    virtual CWorldMap* GetWorldMap() { return &g_worldMap; }
};
class CMainGameComponent { public: unsigned char _pad00[0x24]; CWorld* PWorld; };
static CMainGameComponent g_mainGame; CMainGameComponent* g_pMainGameComponent = &g_mainGame; static CWorld g_world;
class CRegion;
static const CThing* g_added[8]; static long g_addedGraphic[8]; static int g_addCount = 0;
class CMiniMapDisplay { public:
    virtual void v00() {}
    unsigned char _pad04[0xb4];
    CRegion*      m_pRegion;
    void AddMarker(const CThing* thing, long graphicIndex) { g_added[g_addCount] = thing; g_addedGraphic[g_addCount++] = graphicIndex; }
    void InitialiseHouseMarkers();
};

void CMiniMapDisplay::InitialiseHouseMarkers()
{
    if (m_pRegion) {
        CWorld* world = g_pMainGameComponent->PWorld;
        CWorldMap* worldMap = world->GetWorldMap();
        list<CThing*>& houses = world->GetThingManager()->Lists->Houses;
        for (_List_node<CThing*>* node = houses._M_node->_M_next; node != houses._M_node; node = node->_M_next) {
            CThing* thing = node->_M_data;
            if (worldMap->GetCurrentRegionNumber() == worldMap->GetRegionNumberMapIsIn(thing->GetMapIndex()) && thing->HasTC(0xd5)) {
                const CTCBuyableHouse* house = thing->GetTC<CTCBuyableHouse>(0xd5);
                if (house->OwnedByHero)
                    AddMarker(thing, PeekGuiDef()->MiniMapMarkerGraphics[4].Primary);
                else if (house->IsForSale())
                    AddMarker(thing, PeekGuiDef()->MiniMapMarkerGraphics[4].Secondary);
            }
        }
    }
}
static CThing g_things[5]; static CInterfaceMap::SEntry g_entries[5][2]; static CTCBuyableHouse g_house[5];
static void MakeThing(int i, short mapIndex, bool hasHouse, bool owned, bool forSale)
{
    CThing& t = g_things[i]; memset(&t, 0, sizeof(t)); t.MapIndex = mapIndex;
    g_entries[i][0].type = 0x10; g_entries[i][0].component = 0;
    g_entries[i][1].type = 0xd5; g_entries[i][1].component = &g_house[i];
    t.m_interfaces.first = g_entries[i]; t.m_interfaces.last = g_entries[i] + (hasHouse ? 2 : 1);
    if (hasHouse) t.m_presentBits[0xd5 >> 5] |= 1UL << (0xd5 & 31);
    g_house[i].OwnedByHero = owned; g_house[i].ForSale = forSale;
}
int main()
{
    g_mainGame.PWorld = &g_world; static CThingLists lists; g_thingManager.Lists = &lists;
    g_guiDef.MiniMapMarkerGraphics.m_pBegin = g_graphics; g_guiDef.MiniMapMarkerGraphics.m_pEnd = g_graphics + 8;
    g_graphics[4].Primary = 81; g_graphics[4].Secondary = 82;
    MakeThing(0, 31, true, true, true); MakeThing(1, 32, true, false, true); MakeThing(2, 45, true, true, true);
    MakeThing(3, 33, false, true, true); MakeThing(4, 34, true, false, false);
    _List_node<CThing*> head, n[5];
    head._M_next = &n[0]; for (int i = 0; i < 4; ++i) n[i]._M_next = &n[i + 1]; n[4]._M_next = &head;
    for (int i = 0; i < 5; ++i) n[i]._M_data = &g_things[i];
    lists.Houses._M_node = &head;
    unsigned char raw[sizeof(CMiniMapDisplay)]; memset(raw, 0, sizeof(raw));
    CMiniMapDisplay* d = (CMiniMapDisplay*)raw;
    d->m_pRegion = 0; d->InitialiseHouseMarkers();
    CHECK(g_addCount == 0); CHECK(g_currentCalls == 0);
    d->m_pRegion = (CRegion*)1; d->InitialiseHouseMarkers();
    CHECK(g_addCount == 2); CHECK(g_regionCalls == 5); CHECK(g_currentCalls == 5); CHECK(g_forSaleCalls == 2);
    CHECK(g_added[0] == &g_things[0]); CHECK(g_addedGraphic[0] == 81);
    CHECK(g_added[1] == &g_things[1]); CHECK(g_addedGraphic[1] == 82);
    if (fails == 0) printf("0082acd0_TEST PASS\n"); else printf("0082acd0_TEST FAIL\n");
    return fails;
}