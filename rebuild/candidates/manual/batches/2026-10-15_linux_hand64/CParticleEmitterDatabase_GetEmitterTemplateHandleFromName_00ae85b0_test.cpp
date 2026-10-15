#include <cstdio>
#include <cstring>
// Behaviour of CParticleEmitterDatabase::GetEmitterTemplateHandleFromName (0x00AE85B0): the upper-cased name is
// looked up in the symbol map; its handle is returned, or 0 when it is unknown. The temporary is destroyed.
static int g_live = 0;
struct CCharString {
    char m_text[32];
    CCharString(const char* s) { std::strcpy(m_text, s); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString ToUpper() const { CCharString r(m_text); for (int i = 0; r.m_text[i]; ++i) if (r.m_text[i] >= 'a' && r.m_text[i] <= 'z') r.m_text[i] -= 32; return r; }
};
struct CSymbolMap {
    bool Lookup(const CCharString& name, long* value)
    {
        if (std::strcmp(name.m_text, "SMOKE") == 0) { *value = 42; return true; }
        return false;
    }
};
struct CParticleEmitterDatabase {
    CSymbolMap SymbolMap;
    long GetEmitterTemplateHandleFromName(const CCharString& name)
    {
        long handle;
        bool found = SymbolMap.Lookup(name.ToUpper(), &handle);
        return found ? handle : 0;
    }
};
int main()
{
    CParticleEmitterDatabase d;
    CCharString smoke("smoke"), fire("fire");
    if (d.GetEmitterTemplateHandleFromName(smoke) != 42 || d.GetEmitterTemplateHandleFromName(fire) != 0 || g_live != 2) { std::printf("00ae85b0_TEST FAIL\n"); return 1; }
    std::printf("00ae85b0_TEST PASS\n");
    return 0;
}
