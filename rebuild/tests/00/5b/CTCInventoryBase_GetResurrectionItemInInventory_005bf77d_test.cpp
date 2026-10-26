#include <cstdio>
// Behaviour test for CTCInventoryBase::GetResurrectionItemInInventory (0x005BF77D): same body as src/, with the item
// listing, def lookups, object creation and interfaces stubbed and recorded.
static int g_destroyed = 0;
class CDefClassBase { public: long RefCount; void Destroy() { ++g_destroyed; } };
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() { Object = 0; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CResurrectionItemDef : public CDefClassBase { public: };
static CResurrectionItemDef g_resDef;
struct CThingDef : CDefClassBase {
    bool resurrects;
    bool GetSubDef(CDefPointer<CResurrectionItemDef>* out)
    { if (!resurrects) return false; ++g_resDef.RefCount; out->Object = &g_resDef; return true; }
};
static CThingDef g_defs[4];
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingDef>* out) { ++g_defs[index].RefCount; out->Object = &g_defs[index]; return true; }
};
static CDefinitionManager g_manager;
static int g_notified = 0;
struct CTCInterface1A { void Notify() { ++g_notified; } };
static CTCInterface1A g_iface;
class CThing {
public:
    bool hasE1;
    bool HasTC(long type) { return type == 0xe1 && hasE1; }
    template<class T> bool GetInterface(T** ppOut, long type) { if (type == 0x1a) { *ppOut = (T*)&g_iface; return true; } return false; }
};
static CThing g_objects[4];
static int g_created = 0;
namespace NInventory { class CItem { public: long DefIndex; long Count; }; }
struct CItemVector {
    NInventory::CItem* _M_start; NInventory::CItem* _M_finish; NInventory::CItem* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned long size() const { return _M_finish - _M_start; }
    NInventory::CItem& operator[](unsigned long n) { return _M_start[n]; }
};
static NInventory::CItem g_items[3] = { { 1, 1 }, { 2, 1 }, { 3, 1 } };
struct CTCInventoryBase {
    CDefinitionManager* GetDefinitions() const { return &g_manager; }
    void GetAllItems(CItemVector& items) { items._M_start = g_items; items._M_finish = g_items + 3; }
    CThing* CreateInventoryObject(long defIndex) { ++g_created; return &g_objects[defIndex]; }
    CThing* GetResurrectionItemInInventory();
};

CThing* CTCInventoryBase::GetResurrectionItemInInventory()
{
    CItemVector items;
    GetAllItems(items);
    unsigned long count = items.size();
    for (unsigned long i = 0; i < count; ++i) {
        NInventory::CItem& item = items[i];
        CDefPointer<CThingDef> thingDef;
        GetDefinitions()->GetDef(item.DefIndex, &thingDef);
        CDefPointer<CResurrectionItemDef> resurrectionDef;
        if (thingDef->GetSubDef(&resurrectionDef)) {
            CThing* object = CreateInventoryObject(item.DefIndex);
            if (object->HasTC(0xe1))
                return object;
            CTCInterface1A* component = 0;
            object->GetInterface(&component, 0x1a);
            component->Notify();
        }
    }
    return 0;
}

int main()
{
    int fails = 0;
    for (int i = 0; i < 4; ++i) { g_defs[i].RefCount = 1; g_defs[i].resurrects = false; g_objects[i].hasE1 = false; }
    g_resDef.RefCount = 1;
    g_defs[2].resurrects = true; g_defs[3].resurrects = true; g_objects[3].hasE1 = true;
    CTCInventoryBase inv;
    CThing* found = inv.GetResurrectionItemInInventory();
    if (found != &g_objects[3] || g_created != 2 || g_notified != 1) ++fails;
    if (g_defs[1].RefCount != 1 || g_defs[2].RefCount != 1 || g_defs[3].RefCount != 1 || g_resDef.RefCount != 1 || g_destroyed) ++fails;
    g_objects[3].hasE1 = false;
    if (inv.GetResurrectionItemInInventory() != 0 || g_created != 4 || g_notified != 3) ++fails;
    if (fails) { std::printf("005bf77d_TEST FAIL %d\n", fails); return 1; }
    std::printf("005bf77d_TEST PASS\n");
    return 0;
}