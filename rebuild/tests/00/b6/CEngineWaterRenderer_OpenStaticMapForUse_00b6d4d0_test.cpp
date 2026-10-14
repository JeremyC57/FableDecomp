#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_live = 0;
struct CCharString { const char* m_p; CCharString(const char* p) : m_p(p) { ++g_live; } CCharString(const CCharString& o) : m_p(o.m_p) { ++g_live; } ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& o) { m_p = o.m_p; return *this; } };
struct CStaticMapBankFile { int x; };
struct CEngineWaterRenderer { unsigned char _pad0[0x5a8]; CStaticMapBankFile* StaticMapBankFile; CCharString StaticMapSeaEntryPrefix;
    CEngineWaterRenderer() : StaticMapSeaEntryPrefix("") { }
    void OpenStaticMapForUse(CStaticMapBankFile* file, CCharString seaEntryPrefix) { StaticMapBankFile = file; StaticMapSeaEntryPrefix = seaEntryPrefix; } };
int main()
{
    CEngineWaterRenderer* r = new CEngineWaterRenderer(); CStaticMapBankFile f; CCharString prefix("SEA_");
    r->OpenStaticMapForUse(&f, prefix);
    bool ok = r->StaticMapBankFile == &f && std::strcmp(r->StaticMapSeaEntryPrefix.m_p, "SEA_") == 0 && g_live == 2;
    if (!ok) { std::printf("00b6d4d0_TEST FAIL\n"); return 1; }
    std::printf("00b6d4d0_TEST PASS\n"); return 0;
}