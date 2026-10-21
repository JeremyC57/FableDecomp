#include <cstdio>
// Behaviour test for CTCInventory::RemoveItemFromInventory (0x00583CD4): same body as src/, with the base removal,
// the held-count query, the def lookups and the light interface stubbed and recorded.
static int g_destroyed = 0;
struct CDefClassBase {
    long RefCount;
    void Destroy() { ++g_destroyed; }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() { Object = 0; }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CLightDef : CDefClassBase { };
static CLightDef g_lightDef;
struct CThingDef : CDefClassBase {
    bool hasLight;
    bool GetSubDef(CDefPointer<CLightDef>* out)
    {
        if (!hasLight) return false;
        ++g_lightDef.RefCount; out->Object = &g_lightDef; return true;
    }
};
static CThingDef g_thingDef;
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingDef>* out)
    {
        ++g_thingDef.RefCount; out->Object = &g_thingDef; return true;
    }
};
static CDefinitionManager g_defs;
static int g_lightCalls = 0; static bool g_lightState = true;
struct CTCLight { void SetActive(bool a) { ++g_lightCalls; g_lightState = a; } };
static CTCLight g_light;
struct CThing {
    bool hasLight;
    template<class T> bool GetInterface(T** ppOut, long type)
    {
        if (type == 0x27 && hasLight) { *ppOut = (T*)&g_light; return true; }
        return false;
    }
};
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return _M_start[n]; }
};
static long g_baseDef = -1, g_baseCount = -1;
static long g_held = 0;
static int g_countCalls = 0;
struct CTCInventoryBase {
    CThing* ParentThing;
    CDefinitionManager* GetDefinitions() const { return &g_defs; }
    long GetNumberOfItemsOfTypeInInventory(long d) { ++g_countCalls; return g_held; }
    void RemoveItemFromInventory(long d, long c) { g_baseDef = d; g_baseCount = c; }
};
struct CTCInventory : CTCInventoryBase {
    CRetailVector<long> SlotItems;
    CRetailVector<long> QuickAccessItems;
    bool NeedsRefresh;
    void RemoveItemFromInventory(long defIndex, long count);
};

void CTCInventory::RemoveItemFromInventory(long defIndex, long count)
{
    CTCInventoryBase::RemoveItemFromInventory(defIndex, count);
    bool removed;
    do {
        removed = false;
        for (unsigned long i = 0; i < SlotItems.size(); ++i) {
            if (SlotItems[i] == defIndex && GetNumberOfItemsOfTypeInInventory(defIndex) == 0) {
                SlotItems[i] = 0;
                removed = true;
                NeedsRefresh = true;
            }
        }
    } while (removed);
    for (unsigned long i = 0; i < QuickAccessItems.size(); ++i) {
        if (QuickAccessItems[i] == defIndex && GetNumberOfItemsOfTypeInInventory(defIndex) == 0) {
            QuickAccessItems[i] = 0;
            NeedsRefresh = true;
        }
    }
    CDefPointer<CThingDef> thingDef;
    GetDefinitions()->GetDef(defIndex, &thingDef);
    CDefPointer<CLightDef> lightDef;
    if (thingDef->GetSubDef(&lightDef)) {
        CTCLight* light;
        if (ParentThing->GetInterface(&light, 0x27))
            light->SetActive(false);
    }
}

int main()
{
    int fails = 0;
    CThing owner; owner.hasLight = true;
    long slots[4] = { 5, 7, 5, 0 };
    long quick[3] = { 7, 5, 5 };
    CTCInventory inv;
    inv.ParentThing = &owner; inv.NeedsRefresh = false;
    inv.SlotItems._M_start = slots; inv.SlotItems._M_finish = slots + 4;
    inv.QuickAccessItems._M_start = quick; inv.QuickAccessItems._M_finish = quick + 3;
    g_thingDef.RefCount = 1; g_thingDef.hasLight = true; g_lightDef.RefCount = 1;
    inv.RemoveItemFromInventory(5, 2);
    if (g_baseDef != 5 || g_baseCount != 2) ++fails;
    if (slots[0] || slots[1] != 7 || slots[2] || quick[0] != 7 || quick[1] || quick[2] || !inv.NeedsRefresh) ++fails;
    if (g_countCalls != 4) ++fails;                              // 2 slot hits + rerun finds none, then 2 quick hits
    if (g_lightCalls != 1 || g_lightState) ++fails;
    if (g_thingDef.RefCount != 1 || g_lightDef.RefCount != 1 || g_destroyed) ++fails;
    // still held: nothing is cleared, no light def -> no light call
    g_held = 1; g_countCalls = 0; inv.NeedsRefresh = false; g_thingDef.hasLight = false;
    slots[0] = 7;
    inv.RemoveItemFromInventory(7, 1);
    if (slots[0] != 7 || slots[1] != 7 || quick[0] != 7 || inv.NeedsRefresh || g_countCalls != 3 || g_lightCalls != 1) ++fails;
    if (fails) { std::printf("00583cd4_TEST FAIL %d\n", fails); return 1; }
    std::printf("00583cd4_TEST PASS\n");
    return 0;
}
