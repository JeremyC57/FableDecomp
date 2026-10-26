// Behaviour test for ResetGlobalScriptDef (0x00CBEEB0): release one reference of the global script def
// (destroying it when the count reaches zero) and clear the global.
#include <cstdio>

static int g_destroyed = 0;
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual ~CDefVirtual() { }
    virtual void Destroy() { ++g_destroyed; }
};
class CScriptDef : public CDefVirtual, public CRefCounted { public:
    void ReleaseReference()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
CScriptDef* g_pScriptDef = 0;

void ResetGlobalScriptDef()
{
    if (g_pScriptDef)
        g_pScriptDef->ReleaseReference();
    g_pScriptDef = 0;
}

int main()
{
    CScriptDef def;
    def.m_count = 2;
    g_pScriptDef = &def;
    ResetGlobalScriptDef();
    bool firstOk = g_pScriptDef == 0 && def.m_count == 1 && g_destroyed == 0;
    g_pScriptDef = &def;
    ResetGlobalScriptDef();
    bool secondOk = g_pScriptDef == 0 && def.m_count == 0 && g_destroyed == 1;
    ResetGlobalScriptDef();                     // null global: nothing to release
    if (!firstOk || !secondOk || g_destroyed != 1 || g_pScriptDef != 0) {
        std::printf("00cbeeb0_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbeeb0_TEST PASS\n");
    return 0;
}