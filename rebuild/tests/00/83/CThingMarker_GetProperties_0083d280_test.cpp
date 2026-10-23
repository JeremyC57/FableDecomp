// Behaviour test for 0083d280: body copied from src/0083d280.cpp (CThingMarker::GetProperties), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CThingMarkerDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long          m_count;
    unsigned char _pad08[0x7c];
    unsigned long Properties;
    void AddRef() { m_count = m_count + 1; }
    void Release() { m_count = m_count - 1; if (m_count == 0) Destroy(); }
};
struct CDefPointer {
    CThingMarkerDef* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    CThingMarkerDef* Get() const { return m_p; }
    CThingMarkerDef* operator->() const { return m_p; }
};
static CThingMarkerDef* g_def = 0; static long g_askedIndex = -1; static int g_getDef = 0;
struct CDefinitionManager {
    bool GetDef_0083d630(long globalIndex, CDefPointer& out);
};
bool CDefinitionManager::GetDef_0083d630(long globalIndex, CDefPointer& out)
{
    ++g_getDef; g_askedIndex = globalIndex;
    if (!g_def)
        return false;
    g_def->AddRef();
    out.m_p = g_def;
    return true;
}
static CDefinitionManager g_manager;
struct CThing {
    virtual void v00() {}
    unsigned char  _pad04[0x88];
    unsigned short DefGlobalIndex;
    CDefinitionManager* GetDefinitionManager() { return &g_manager; }
};
struct CThingMarker : CThing {
    __forceinline CDefPointer GetMarkerDef()
    {
        CDefPointer def;
        GetDefinitionManager()->GetDef_0083d630(DefGlobalIndex, def);
        return def;
    }
    virtual unsigned long GetProperties();
};
unsigned long CThingMarker::GetProperties()
{
    return GetMarkerDef()->Properties;
}
int main()
{
    static CThingMarkerDef def; def.m_count = 1; def.Properties = 0xABCD; g_def = &def;
    CThingMarker m; m.DefGlobalIndex = 77;
    CHECK(m.GetProperties() == 0xABCD); CHECK(g_getDef == 1 && g_askedIndex == 77); CHECK(def.m_count == 1 && g_destroyed == 0);
    def.m_count = 0; def.Properties = 5; CHECK(m.GetProperties() == 5); CHECK(g_destroyed == 1 && def.m_count == 0);
    if (fails == 0) printf("0083d280_TEST PASS\n"); else printf("0083d280_TEST FAIL\n");
    return fails;
}