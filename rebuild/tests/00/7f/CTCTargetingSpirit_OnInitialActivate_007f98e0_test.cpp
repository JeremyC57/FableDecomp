// Behaviour test for 007f98e0: body copied from src/007f98e0.cpp (CTCTargetingSpirit::OnInitialActivate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CThing;
struct CRbNode {
    long     _M_color;
    CRbNode* _M_parent;
    CRbNode* _M_left;
    CRbNode* _M_right;
    long     first;
    float    second;
};
static CRbNode g_nodes[2]; static CRbNode g_header; static long g_findKey = -1;
struct CDistanceMap {
    struct iterator {
        CRbNode* _M_node;
        iterator() { }
    };
    CRbNode* _M_header;
    long     _M_node_count;
    iterator find(const long& key) const { g_findKey = key; iterator it; it._M_node = _M_header; for (int i = 0; i < 2; ++i) if (g_nodes[i].first == key) it._M_node = &g_nodes[i]; return it; }
};
static __forceinline float FindDistance(const CDistanceMap& map, long key)
{
    CDistanceMap::iterator it = map.find(key);
    return it._M_node != map._M_header ? it._M_node->second : 0.0f;
}
static int g_destroyed = 0;
struct CTargetingSpiritDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
    unsigned char _pad08[0x74 - 0x08];
    CDistanceMap Distances;
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
static CTargetingSpiritDef* g_def = 0; static int g_getSubDef = 0;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CTargetingSpiritDef>* out) const;
};
bool CDefClassBase::GetSubDef(CDefPointer<CTargetingSpiritDef>* out) const
{
    ++g_getSubDef;
    if (!g_def) return false;
    ++g_def->RefCount; out->Object = g_def; return true;
}
struct CThingDefHolder {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;
};
static int g_baseActivate = 0;
class CTCTargetingBase { public:
    void* __vfptr;
    CThingDefHolder* ParentThing;
    unsigned char _pad08[0x38 - 0x08];
    void OnInitialActivate() { ++g_baseActivate; }
};
struct CTCTargetingSpirit : CTCTargetingBase {
    float MaxTargetingDistance;
    void OnInitialActivate();
};
void CTCTargetingSpirit::OnInitialActivate()
{
    CTCTargetingBase::OnInitialActivate();
    CDefPointer<CTargetingSpiritDef> def;
    ParentThing->PDef->GetSubDef(&def);
    CTargetingSpiritDef* d = def.Object;
    MaxTargetingDistance = FindDistance(d->Distances, 0x10);
    if (--d->RefCount == 0)
        d->Destroy();
}
int main()
{
    static CThingDefHolder thing; static CDefClassBase classDef; static CTargetingSpiritDef def;
    thing.PDef = &classDef; def.RefCount = 1; def.Distances._M_header = &g_header; g_def = &def;
    g_nodes[0].first = 0x08; g_nodes[0].second = 5.0f; g_nodes[1].first = 0x10; g_nodes[1].second = 12.5f;
    CTCTargetingSpirit tc; memset(&tc, 0, sizeof(tc)); tc.ParentThing = &thing; tc.MaxTargetingDistance = -1.0f;
    tc.OnInitialActivate();
    CHECK(g_baseActivate == 1 && g_getSubDef == 1); CHECK(g_findKey == 0x10); CHECK(tc.MaxTargetingDistance == 12.5f); CHECK(def.RefCount == 1 && g_destroyed == 0);
    // no entry for key 0x10: distance 0
    g_nodes[1].first = 0x11; tc.OnInitialActivate(); CHECK(tc.MaxTargetingDistance == 0.0f);
    // a def held only by the lookup is destroyed
    def.RefCount = 0; tc.OnInitialActivate(); CHECK(g_destroyed == 1);
    if (fails == 0) printf("007f98e0_TEST PASS\n"); else printf("007f98e0_TEST FAIL\n");
    return fails;
}