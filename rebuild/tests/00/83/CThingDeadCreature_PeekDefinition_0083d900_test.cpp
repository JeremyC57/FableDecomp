// Behaviour test for 0083d900: body copied from src/0083d900.cpp (CThingDeadCreature::PeekDefinition, second instantiation), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void v000() {}
    virtual void DestroyDef() { ++g_destroyed; }
    long m_refCount;
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> struct CDefPointer {
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
};
struct CThingCreatureDef : CDefPointeeBase { };
static CThingCreatureDef* g_def = 0; static long g_askedIndex = -1; static int g_getDef = 0;
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingCreatureDef>& out);
};
bool CDefinitionManager::GetDef(long index, CDefPointer<CThingCreatureDef>& out)
{
    ++g_getDef; g_askedIndex = index;
    if (!g_def) return false;
    ++g_def->m_refCount; out.m_pObject = g_def; return true;
}
static CDefinitionManager g_manager;
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short m_defGlobalIndex;
    CDefinitionManager* GetDefinitionManager() const { return &g_manager; }
};
struct CThingDeadCreature : CThing {
    const CThingCreatureDef& PeekDefinition() const;
};
const CThingCreatureDef& CThingDeadCreature::PeekDefinition() const
{
    CDefPointer<CThingCreatureDef> def;
    GetDefinitionManager()->GetDef(m_defGlobalIndex, def);
    CThingCreatureDef* creatureDef = def.m_pObject;
    if (creatureDef)
        creatureDef->ReleaseDef();
    return *creatureDef;
}
int main()
{
    static CThingCreatureDef def; def.m_refCount = 1; g_def = &def;
    CThingDeadCreature c; memset(&c, 0, sizeof(c)); c.m_defGlobalIndex = 556;
    CHECK(&c.PeekDefinition() == &def); CHECK(g_getDef == 1 && g_askedIndex == 556); CHECK(def.m_refCount == 1 && g_destroyed == 0);
    def.m_refCount = 0; CHECK(&c.PeekDefinition() == &def); CHECK(g_destroyed == 1);
    g_def = 0; CHECK(&c.PeekDefinition() == 0); CHECK(g_destroyed == 1);
    if (fails == 0) printf("0083d900_TEST PASS\n"); else printf("0083d900_TEST FAIL\n");
    return fails;
}