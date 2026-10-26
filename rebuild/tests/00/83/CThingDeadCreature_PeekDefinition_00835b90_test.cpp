// Behaviour test for 00835b90: body copied from src/00835b90.cpp (CThingDeadCreature::PeekDefinition), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CDefVirtual {
    virtual void v000() {}
    virtual void DestroyDef() { ++g_destroyed; }
};
struct CRefCounted { long m_refCount; };
class CDefPointeeBase : public CDefVirtual, public CRefCounted { public:
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> class CDefPointer { public:
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->ReleaseDef();
    }
    T& operator*() const { return *m_pObject; }
};
class CThingCreatureDef : public CDefPointeeBase { public: };
static CThingCreatureDef* g_def = 0; static long g_askedIndex = -1; static int g_getDef = 0;
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingCreatureDef>& out);
};
bool CDefinitionManager::GetDef(long index, CDefPointer<CThingCreatureDef>& out)
{
    ++g_getDef; g_askedIndex = index;
    if (!g_def) return false;
    ++g_def->m_refCount; out.m_pObject = g_def; return true;
}
static CDefinitionManager g_manager;
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short m_defGlobalIndex;
    CDefinitionManager* GetDefinitionManager() const { return &g_manager; }
};
class CThingDeadCreature : public CThing { public:
    const CThingCreatureDef& PeekDefinition() const;
};
const CThingCreatureDef& CThingDeadCreature::PeekDefinition() const
{
    CDefPointer<CThingCreatureDef> def;
    GetDefinitionManager()->GetDef(m_defGlobalIndex, def);
    return *def;
}
int main()
{
    static CThingCreatureDef def; def.m_refCount = 1; g_def = &def;
    CThingDeadCreature c; memset(&c, 0, sizeof(c)); c.m_defGlobalIndex = 555;
    CHECK(&c.PeekDefinition() == &def); CHECK(g_getDef == 1 && g_askedIndex == 555); CHECK(def.m_refCount == 1 && g_destroyed == 0);
    def.m_refCount = 0; CHECK(&c.PeekDefinition() == &def); CHECK(g_destroyed == 1);
    if (fails == 0) printf("00835b90_TEST PASS\n"); else printf("00835b90_TEST FAIL\n");
    return fails;
}