// behaviour test for 005d9a10: CTCInventoryItem::GetHairCardDef(long) -- manager lookup of the thing def, then its CHairCardDef sub-def;
// the result holds one reference, every temporary reference is dropped, and a miss on either lookup gives a null pointer
#include <cstdio>
class CDefPointeeBase { public:
    long RefCount; int destroyed; CDefPointeeBase* sub;
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) ++destroyed; }
};
struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
    void Assign(CDefPointeeBase* p) { if (p) p->AddRef(); if (Object) Object->Release(); Object = p; }
};
static CDefPointeeBase g_things[3];
static CDefPointeeBase g_subs[3];
static bool GetDef(long index, CDefPointer* out)
{
    if (index < 0 || index >= 3) return false;
    out->Assign(&g_things[index]);
    return true;
}
static bool GetSubDef(CDefPointeeBase* def, CDefPointer* out)
{
    if (!def->sub) return false;
    out->Assign(def->sub);
    return true;
}
static CDefPointer Get(long defIndex)
{
    CDefPointer thingDef;
    if (GetDef(defIndex, &thingDef)) {
        CDefPointer out;
        if (GetSubDef(thingDef.Object, &out))
            return out;
    }
    return CDefPointer();
}
int main()
{
    int bad = 0;
    for (int i = 0; i < 3; ++i) { g_things[i].RefCount = 1; g_things[i].destroyed = 0; g_things[i].sub = 0;
                                  g_subs[i].RefCount = 1; g_subs[i].destroyed = 0; g_subs[i].sub = 0; }
    g_things[1].sub = &g_subs[1];
    {
        CDefPointer r = Get(1);
        if (r.Object != &g_subs[1] || g_subs[1].RefCount != 2 || g_things[1].RefCount != 1) ++bad;
    }
    if (g_subs[1].RefCount != 1 || g_subs[1].destroyed || g_things[1].destroyed) ++bad;
    {
        CDefPointer r = Get(0);              /* thing def without the sub-def */
        if (r.Object != 0 || g_things[0].RefCount != 1) ++bad;
        CDefPointer m = Get(7);              /* no thing def */
        if (m.Object != 0) ++bad;
    }
    g_things[2].RefCount = 0; g_things[2].sub = &g_subs[2];   /* the lookup held the only thing-def reference */
    { CDefPointer r = Get(2); if (r.Object != &g_subs[2] || g_things[2].destroyed != 1) ++bad; }
    if (bad) { std::printf("005d9a10_TEST FAIL\n"); return 1; }
    std::printf("005d9a10_TEST PASS\n");
    return 0;
}