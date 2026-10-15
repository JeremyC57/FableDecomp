#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0x0045641A: MorphGraphics block freed, then HideMaterials destroyed, then the base destructor.
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct CCharString { void* m_pStringData; };
struct SCharStringVector {
    CCharString* _M_start; CCharString* _M_finish; CCharString* _M_end_of_storage;
    ~SCharStringVector() { Log('H'); }
};
struct CAppearanceModifierGraphics {
    void* m_pData; long m_count; long m_capacity;
    ~CAppearanceModifierGraphics() { if (m_pData) { Log('F'); std::free(m_pData); } }
};
struct CBase_009fc550 { virtual void v00() {} ~CBase_009fc550() { Log('B'); } };
struct CDefPointeeBase : CBase_009fc550 { virtual void v00() {} ~CDefPointeeBase() {} };
struct CAppearanceModifierDef : CDefPointeeBase {
    SCharStringVector HideMaterials;
    CAppearanceModifierGraphics MorphGraphics;
    virtual void v00() {}
    ~CAppearanceModifierDef();
};
CAppearanceModifierDef::~CAppearanceModifierDef()
{
}
int main()
{
    { CAppearanceModifierDef d; d.MorphGraphics.m_pData = std::malloc(8); }
    if (std::strcmp(g_log, "FHB") != 0) { std::printf("0045641a_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    { CAppearanceModifierDef d; d.MorphGraphics.m_pData = 0; }
    if (std::strcmp(g_log, "HB") != 0) { std::printf("0045641a_TEST FAIL\n"); return 1; }
    std::printf("0045641a_TEST PASS\n");
    return 0;
}