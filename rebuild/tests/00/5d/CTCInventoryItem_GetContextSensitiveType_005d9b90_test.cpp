// behaviour test for 005d9b90: CTCInventoryItem::GetContextSensitiveType(long) -- cache the defs, fetch the thing def's
// context-sensitive sub-def and return its Type (the sub-def reference is dropped); 0 on any miss
#include <cstdio>
class CDefPointeeBase { public:
    long RefCount; int destroyed; int Type; CDefPointeeBase* sub;
    void Release() { if (--RefCount == 0) ++destroyed; }
};
struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
};
static CDefPointeeBase g_things[3];
static CDefPointeeBase g_subs[3];
static CDefPointeeBase* s_CachedThingDef = 0;
static bool CacheDef(long defIndex)
{
    if (defIndex < 0 || defIndex >= 3) return false;
    s_CachedThingDef = &g_things[defIndex];
    return true;
}
static bool GetSubDef(const CDefPointeeBase* def, CDefPointer* out)
{
    out->Object = def->sub;
    if (out->Object) ++out->Object->RefCount;
    return out->Object != 0;
}
static int GetContextSensitiveType(long defIndex)
{
    if (CacheDef(defIndex)) {
        CDefPointer sub;
        if (GetSubDef(s_CachedThingDef, &sub))
            return sub.Object->Type;
    }
    return 0;
}
int main()
{
    int bad = 0;
    for (int i = 0; i < 3; ++i) { g_things[i].RefCount = 1; g_things[i].destroyed = 0; g_things[i].Type = -1; g_things[i].sub = 0;
                                  g_subs[i].RefCount = 1; g_subs[i].destroyed = 0; g_subs[i].Type = 20 + i; g_subs[i].sub = 0; }
    g_things[1].sub = &g_subs[1];
    if (GetContextSensitiveType(1) != 21 || g_subs[1].RefCount != 1 || g_subs[1].destroyed) ++bad;
    if (GetContextSensitiveType(0) != 0) ++bad;          /* no sub-def */
    if (GetContextSensitiveType(5) != 0) ++bad;          /* no def */
    g_things[2].sub = &g_subs[2]; g_subs[2].RefCount = 0; /* the lookup held the only reference */
    if (GetContextSensitiveType(2) != 22 || g_subs[2].destroyed != 1) ++bad;
    if (bad) { std::printf("005d9b90_TEST FAIL\n"); return 1; }
    std::printf("005d9b90_TEST PASS\n");
    return 0;
}