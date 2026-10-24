// behaviour test for 005d9320: CTCInventoryItem::ClearCachedDefs -- zero the cached index, release and null both cached defs
#include <cstdio>
struct CDefPointeeBase {
    int RefCount; int destroyed;
    void Destroy() const { ++((CDefPointeeBase*)this)->destroyed; }
    void Release() const { --((CDefPointeeBase*)this)->RefCount; if (RefCount == 0) Destroy(); }
};
struct CDefPointer {
    CDefPointeeBase* Object;
    void Clear() { if (Object) Object->Release(); Object = 0; }
};
static long s_CachedDefIndex = 0;
static CDefPointer s_CachedThingDef = { 0 };
static CDefPointer s_CachedItemDef = { 0 };
static void ClearCachedDefs()
{
    s_CachedDefIndex = 0;
    s_CachedThingDef.Clear();
    s_CachedItemDef.Clear();
}
int main()
{
    int bad = 0;
    CDefPointeeBase thingDef = { 1, 0 }, itemDef = { 3, 0 };
    s_CachedDefIndex = 1234; s_CachedThingDef.Object = &thingDef; s_CachedItemDef.Object = &itemDef;
    ClearCachedDefs();
    if (s_CachedDefIndex != 0 || s_CachedThingDef.Object != 0 || s_CachedItemDef.Object != 0) ++bad;
    if (thingDef.RefCount != 0 || thingDef.destroyed != 1 || itemDef.RefCount != 2 || itemDef.destroyed != 0) ++bad;
    s_CachedDefIndex = 7;
    ClearCachedDefs();                     /* empty cache: only the index changes */
    if (s_CachedDefIndex != 0 || thingDef.destroyed != 1 || itemDef.RefCount != 2) ++bad;
    if (bad) { std::printf("005d9320_TEST FAIL\n"); return 1; }
    std::printf("005d9320_TEST PASS\n");
    return 0;
}