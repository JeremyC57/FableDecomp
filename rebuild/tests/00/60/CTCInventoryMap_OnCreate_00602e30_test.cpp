// Behaviour test for CTCInventoryMap::OnCreate 0x00602E30 (model copy with stub manager, defs and vectors)
#include <cstdio>

static int g_registered = 0, g_destroyed = 0, g_baseOnCreate = 0, g_resize = 0, g_cleared = 0;
static long g_setPDef = 0;

class CDefPointeeBase { public:
    long m_refCount;
    virtual void Destroy() { ++g_destroyed; }
};
template <class T> class CDefPointer { public: T* Object; };
struct CInventoryMapDef : CDefPointeeBase { long InventoryDefIndex; };
static CInventoryMapDef g_mapDef;
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CInventoryMapDef>* out) { ++g_mapDef.m_refCount; out->Object = &g_mapDef; return true; }
};
class CThing { public: CDefPointer<CDefClassBase> PDef; };
struct CBaseScreen { virtual void v00() {} };
struct CManager { void RegisterScreen(const CBaseScreen* s) { if (s) ++g_registered; } };
class CPlayerGui { public: CManager m; CManager* GetPHeroInfoScreensManager() { return &m; } };
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
struct CPrimitiveHandleVector {
    void Resize(long count) { g_resize = (int)count; }
    void erase() { ++g_cleared; }
    void clear() { erase(); }
};
class CWorldMap { public: long GetNoRegions() { return 12; } };
class CWorld { public: CWorldMap wm; virtual CWorldMap* GetWorldMap() { return &wm; } };
static CWorld g_world;
struct CTCInventoryBase_Map {
    CThing* ParentThing;
    virtual void OnCreate() { ++g_baseOnCreate; }
    void SetPDef(long index) { g_setPDef = index; }
    CWorld* GetWorld() const { return &g_world; }
};
class CTCInventoryMap : public CTCInventoryBase_Map, public CBaseScreen { public:
    CPrimitiveHandleVector MapScreenPrimitiveHandles, MapScreenQuestPrimitiveHandles,
                           MapScreenTeleportPrimitiveHandles, MapScreenHousePrimitiveHandles;
    long NumRegions;
    virtual void OnCreate();
};

void CTCInventoryMap::OnCreate()
{
    g_pPlayerGui->GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CInventoryMapDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    CInventoryMapDef* mapDef = def.Object;
    SetPDef(mapDef->InventoryDefIndex);
    CTCInventoryBase_Map::OnCreate();
    MapScreenPrimitiveHandles.Resize(2);
    MapScreenQuestPrimitiveHandles.clear();
    MapScreenTeleportPrimitiveHandles.clear();
    MapScreenHousePrimitiveHandles.clear();
    NumRegions = GetWorld()->GetWorldMap()->GetNoRegions() - 1;
    if (--mapDef->m_refCount == 0)
        mapDef->Destroy();
}

int main()
{
    CDefClassBase ownerDef; ownerDef.m_refCount = 1;
    CThing thing; thing.PDef.Object = &ownerDef;
    g_mapDef.m_refCount = 0; g_mapDef.InventoryDefIndex = 77;
    CTCInventoryMap map; map.ParentThing = &thing;
    map.OnCreate();
    if (g_registered != 1 || g_setPDef != 77 || g_baseOnCreate != 1 || g_resize != 2 || g_cleared != 3 ||
        map.NumRegions != 11 || ownerDef.m_refCount != 1 || g_mapDef.m_refCount != 0 || g_destroyed != 1) {
        std::printf("00602e30_TEST FAIL\n"); return 1;
    }
    std::printf("00602e30_TEST PASS\n");
    return 0;
}