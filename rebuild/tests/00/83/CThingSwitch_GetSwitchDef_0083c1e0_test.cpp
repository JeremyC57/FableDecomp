// Behaviour test for 0083c1e0: body copied from src/0083c1e0.cpp (CThingSwitch::GetSwitchDef), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
class CDefPointeeBase { public:
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
};
class CThingPhysicalSwitchDef : public CDefPointeeBase { public: };
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};
static CThingPhysicalSwitchDef* g_def = 0; static long g_askedIndex = -1; static int g_getDef = 0;
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CThingPhysicalSwitchDef>* out);
};
bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingPhysicalSwitchDef>* out)
{
    ++g_getDef; g_askedIndex = globalIndex;
    if (!g_def) return false;
    ++g_def->m_refCount; out->Object = g_def; return true;
}
static CDefinitionManager g_manager;
class CThingSwitch { public:
    unsigned char _pad00[0x8c];
    unsigned short DefGlobalIndex;
    CDefinitionManager* GetDefinitionManager() { return &g_manager; }
    CDefPointer<CThingPhysicalSwitchDef> GetSwitchDef();
};
CDefPointer<CThingPhysicalSwitchDef> CThingSwitch::GetSwitchDef()
{
    CDefPointer<CThingPhysicalSwitchDef> def;
    GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
    return def;
}
int main()
{
    static CThingPhysicalSwitchDef def; def.m_refCount = 1; g_def = &def;
    CThingSwitch sw; memset(&sw, 0, sizeof(sw)); sw.DefGlobalIndex = 0x4321;
    { CDefPointer<CThingPhysicalSwitchDef> r = sw.GetSwitchDef(); CHECK(r.Object == &def); CHECK(def.m_refCount == 2); CHECK(g_getDef == 1 && g_askedIndex == 0x4321); }
    CHECK(def.m_refCount == 1 && g_destroyed == 0);
    g_def = 0;
    { CDefPointer<CThingPhysicalSwitchDef> r = sw.GetSwitchDef(); CHECK(r.Object == 0); }
    CHECK(def.m_refCount == 1 && g_destroyed == 0);
    if (fails == 0) printf("0083c1e0_TEST PASS\n"); else printf("0083c1e0_TEST FAIL\n");
    return fails;
}