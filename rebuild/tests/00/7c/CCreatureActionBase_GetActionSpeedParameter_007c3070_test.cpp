// Behaviour test for 007c3070: body copied from src/007c3070.cpp (CCreatureActionBase::GetActionSpeedParameter), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strCtor = 0, g_strDtor = 0; static const char* g_strText = 0; static long g_strLen = 0;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length) : m_pData((void*)text) { ++g_strCtor; g_strText = text; g_strLen = length; }
    ~CCharString() { ++g_strDtor; m_pData = 0; }
};
static int g_destroyed = 0; static int g_getParam = 0; static long g_paramValue = 0; static const void* g_paramName = 0;
struct CSubDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
    void GetParameter(const CCharString& name, long* pValue) { ++g_getParam; g_paramName = name.m_pData; *pValue = g_paramValue; }
};
struct CDefPointer {
    CSubDef* m_p;
    CDefPointer() : m_p(0) { }
};
static CSubDef* g_subDef = 0; static int g_getSubDef = 0;
class CThing { public:
    bool GetSubDef(CDefPointer& def)
    {
        ++g_getSubDef;
        if (!g_subDef) return false;
        ++g_subDef->m_count; def.m_p = g_subDef; return true;
    }
};
struct CIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* operator->() const { return m_pItem; }
};
class CCreatureActionBase { public:
    virtual void v00() {}
    unsigned char _pad04[4];
    CIntelligentPointer PCreature;
    unsigned char _pad10[0xa8 - 0x10];
    long GetActionSpeedParameter();
};

long CCreatureActionBase::GetActionSpeedParameter()
{
    CDefPointer def;
    long result = 0;                        // the register result (edi), whose zero also initialises the out word
    long value = 0;                         // the addressed out word (frame slot)
    CSubDef* p;
    {
        CCharString name("SOUL_EFFECT", -1);
        bool found = PCreature->GetSubDef(def);
        p = def.m_p;                        // the pointer local, loaded right after the lookup
        if (found) {
            p->GetParameter(name, &value);
            result = value;                 // read into edi before the string destructor and the release
        }
    }
    if (p) {
        if (--p->m_count == 0)
            p->Destroy();
    }
    return result;
}
int main()
{
    static CThing creature; static CSubDef sub; sub.m_count = 1; g_subDef = &sub; g_paramValue = 42;
    CCreatureActionBase a; a.PCreature.m_pItem = &creature;
    CHECK(a.GetActionSpeedParameter() == 42);
    CHECK(g_strCtor == 1 && g_strDtor == 1 && strcmp(g_strText, "SOUL_EFFECT") == 0 && g_strLen == -1);
    CHECK(g_getSubDef == 1 && g_getParam == 1 && g_paramName == (const void*)g_strText); CHECK(sub.m_count == 1 && g_destroyed == 0);
    // no sub-def: zero, nothing released
    g_subDef = 0; CHECK(a.GetActionSpeedParameter() == 0); CHECK(g_getParam == 1 && g_strDtor == 2 && g_destroyed == 0);
    // a def held only by the lookup is destroyed after the read
    g_subDef = &sub; sub.m_count = 0; g_paramValue = -5; CHECK(a.GetActionSpeedParameter() == -5); CHECK(g_destroyed == 1);
    if (fails == 0) printf("007c3070_TEST PASS\n"); else printf("007c3070_TEST FAIL\n");
    return fails;
}