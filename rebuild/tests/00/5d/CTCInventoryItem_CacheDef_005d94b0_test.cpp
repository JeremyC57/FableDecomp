// Behaviour test for 0x005D94B0 CTCInventoryItem::CacheDef(long defIndex)
#include <cstdio>

static int g_deleted = 0;
struct CPointee {
    unsigned int IVRefCount;
    bool HasItemDef;
    void AddRef() { ++IVRefCount; }
    void Release() { if (--IVRefCount == 0) ++g_deleted; }
};
struct CPtr {
    CPointee* Object;
    CPtr() : Object(0) { }
    ~CPtr() { if (Object) { Object->Release(); Object = 0; } }
    CPtr& operator=(const CPtr& o)
    {
        if (Object != o.Object) {
            if (Object) { Object->Release(); Object = 0; }
            Object = o.Object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
};
static CPointee g_things[4];
static CPointee g_items[4];
static int g_lookups = 0;
static bool GetDef(int index, CPtr* out)
{
    ++g_lookups;
    if (index < 0 || index >= 4) return false;
    out->Object = &g_things[index]; g_things[index].AddRef();
    return true;
}
static bool GetSubDef(CPointee* thing, CPtr* out)
{
    int i = (int)(thing - g_things);
    if (!thing->HasItemDef) return false;
    out->Object = &g_items[i]; g_items[i].AddRef();
    return true;
}
static int s_CachedDefIndex = 0;
static CPtr s_CachedThingDef, s_CachedItemDef;

static bool CacheDef(int defIndex)
{
    if (defIndex > 0 && defIndex == s_CachedDefIndex)
        return true;
    CPtr thingDef;
    if (GetDef(defIndex, &thingDef)) {
        CPtr itemDef;
        if (GetSubDef(thingDef.Object, &itemDef)) {
            s_CachedDefIndex = defIndex;
            s_CachedThingDef = thingDef;
            s_CachedItemDef = itemDef;
            return true;
        }
    }
    return false;
}

int main()
{
    for (int i = 0; i < 4; ++i) { g_things[i].IVRefCount = 1; g_items[i].IVRefCount = 1; g_things[i].HasItemDef = (i != 2); g_items[i].HasItemDef = false; }
    if (!CacheDef(1) || s_CachedDefIndex != 1 || s_CachedThingDef.Object != &g_things[1] || s_CachedItemDef.Object != &g_items[1]) { std::printf("005d94b0_TEST FAIL\n"); return 1; }
    if (g_things[1].IVRefCount != 2 || g_items[1].IVRefCount != 2) { std::printf("005d94b0_TEST FAIL\n"); return 1; }
    int before = g_lookups;
    if (!CacheDef(1) || g_lookups != before) { std::printf("005d94b0_TEST FAIL\n"); return 1; }     // cache hit
    if (CacheDef(2) || s_CachedDefIndex != 1 || g_things[2].IVRefCount != 1) { std::printf("005d94b0_TEST FAIL\n"); return 1; } // no item def
    if (CacheDef(7) || s_CachedDefIndex != 1) { std::printf("005d94b0_TEST FAIL\n"); return 1; }  // no def
    if (!CacheDef(3) || s_CachedDefIndex != 3 || g_things[1].IVRefCount != 1 || g_items[1].IVRefCount != 1 || g_things[3].IVRefCount != 2) { std::printf("005d94b0_TEST FAIL\n"); return 1; }
    before = g_lookups;
    if (!CacheDef(0) || g_lookups != before + 1) { std::printf("005d94b0_TEST FAIL\n"); return 1; } // index 0 always looks up
    if (g_deleted != 0) { std::printf("005d94b0_TEST FAIL\n"); return 1; }
    std::printf("005d94b0_TEST PASS\n");
    return 0;
}