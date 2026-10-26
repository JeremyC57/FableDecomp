// Behaviour test for 00666850: body copied from src/00666850.cpp (CThingPlayerCreature::vfunc_71), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SSubDefMapNode {
    SSubDefMapNode* m_pLeft;
    SSubDefMapNode* m_pParent;
    SSubDefMapNode* m_pRight;
    long            m_colour;
    long            m_key;
    void*           m_pValue;
};
static SSubDefMapNode g_nodes[3]; static SSubDefMapNode g_head; static int g_findCalls = 0;
struct CSubDefMap {
    SSubDefMapNode* m_pHead;
    SSubDefMapNode* find(long key) { ++g_findCalls; for (int i = 0; i < 3; ++i) if (g_nodes[i].m_key == key) return &g_nodes[i]; return m_pHead; }
    SSubDefMapNode* end() const { return m_pHead; }
};
static int g_destroyed = 0;
struct CSubDef {
    virtual void v000() {}
    virtual void Destroy() { ++g_destroyed; }
    long          m_refCount;
    unsigned char _pad08[0x58];
    CSubDefMap    m_map;
};
template <class T> class CDefPointer { public:
    T* m_pDef;
    CDefPointer() : m_pDef(0) {}
    ~CDefPointer() { if (m_pDef) { if (--m_pDef->m_refCount == 0) m_pDef->Destroy(); } }
    T* operator->() const { return m_pDef; }
};
static CSubDef* g_subDef = 0; static int g_getSubDef = 0;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CSubDef>* out);
};
bool CDefClassBase::GetSubDef(CDefPointer<CSubDef>* out)
{
    ++g_getSubDef;
    if (!g_subDef)
        return false;
    ++g_subDef->m_refCount;
    out->m_pDef = g_subDef;
    return true;
}
class CThingPlayerCreature { public:
    unsigned char               _pad00[0x70];
    CDefClassBase*              PBaseDef;
    void* vfunc_71(long key);
};
void* CThingPlayerCreature::vfunc_71(long key)
{
    CDefPointer<CSubDef> subDef;
    PBaseDef->GetSubDef(&subDef);
    CSubDef* def = subDef.m_pDef;
    CSubDefMap* map = &def->m_map;
    SSubDefMapNode* it = map->find(key);
    void* result = it != map->end() ? it->m_pValue : 0;
    if (--def->m_refCount == 0) def->Destroy();
    subDef.m_pDef = 0;
    return result;
}
int main()
{
    static CSubDef sub; static CDefClassBase base; static int values[3];
    sub.m_refCount = 1; sub.m_map.m_pHead = &g_head; g_subDef = &sub;
    for (int i = 0; i < 3; ++i) { g_nodes[i].m_key = 10 + i; g_nodes[i].m_pValue = &values[i]; }
    CThingPlayerCreature pc; memset(&pc, 0, sizeof(pc)); pc.PBaseDef = &base;
    CHECK(pc.vfunc_71(11) == &values[1]); CHECK(g_getSubDef == 1 && g_findCalls == 1); CHECK(sub.m_refCount == 1 && g_destroyed == 0);
    CHECK(pc.vfunc_71(99) == 0); CHECK(sub.m_refCount == 1);
    sub.m_refCount = 0; CHECK(pc.vfunc_71(12) == &values[2]); CHECK(g_destroyed == 1 && sub.m_refCount == 0);
    if (fails == 0) printf("00666850_TEST PASS\n"); else printf("00666850_TEST FAIL\n");
    return fails;
}