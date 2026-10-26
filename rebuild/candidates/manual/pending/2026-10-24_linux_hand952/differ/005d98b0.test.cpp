// behaviour test for 0x005D98B0 CTCInventoryItem::IsStockItem(long defIndex): false without probing when the def cannot be cached, otherwise the cached def's GetSubDef<CStockItemDef> answer, and a sub-def handed back is released once
#include <cstdio>
struct CDef { long RefCount; int Released; void Release() { ++Released; } };
struct CDefPtr {
    CDef* Object;
    CDefPtr() : Object(0) { }
    ~CDefPtr() { if (Object != 0) { if (--Object->RefCount == 0) Object->Release(); } }
};
static CDef gSub;
struct CThingDef {
    bool HasSub; int Probes;
    bool GetSubDef(CDefPtr* out) { ++Probes; if (!HasSub) return false; ++gSub.RefCount; out->Object = &gSub; return true; }
};
static CThingDef gCached;
static CThingDef* s_CachedThingDef = &gCached;
static bool CacheDef(long defIndex) { return defIndex > 0; }
static bool IsKind(long defIndex)                   // the function under test
{
    if (!CacheDef(defIndex))
        return false;
    CDefPtr subDef;
    return s_CachedThingDef->GetSubDef(&subDef);
}
int main()
{
    int fail = 0;
    gCached.HasSub = true; gCached.Probes = 0; gSub.RefCount = 0; gSub.Released = 0;
    if (IsKind(0) || gCached.Probes != 0) fail |= 1;
    if (!IsKind(5) || gCached.Probes != 1 || gSub.RefCount != 0 || gSub.Released != 1) fail |= 2;
    gCached.HasSub = false;
    if (IsKind(5) || gCached.Probes != 2 || gSub.Released != 1) fail |= 4;
    if (fail) { std::printf("005d98b0_TEST FAIL %d\n", fail); return 1; }
    std::printf("005d98b0_TEST PASS\n");
    return 0;
}
