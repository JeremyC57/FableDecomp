// Behaviour test for 00f0b890: the model below is a copy of src/00f0b890.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CGameScriptInterface { public:
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {}
    virtual void v010() {} virtual void v011() {}
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30)
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};

class CQ_AmbushTradersScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    bool CheckForValidRegion();
};

bool CQ_AmbushTradersScript::CheckForValidRegion()
{
    return GSI->IsRegionLoaded("BowerstoneJail") || GSI->IsRegionLoaded("DesertedFarm")
        || GSI->IsRegionLoaded("GibbetWoods");
}
}

// ---- test doubles and driver ----
static const char* g_loaded = "";
static int g_ctors = 0, g_dtors = 0, g_queries = 0;
CCharString::CCharString(const char* text, long length) { ++g_ctors; PStringData = (void*)text; (void)length; }
CCharString::~CCharString() { ++g_dtors; }
bool CGameScriptInterface::IsRegionLoaded(const CCharString& region) const
{
    ++g_queries;
    return std::strcmp((const char*)region.PStringData, g_loaded) == 0;
}

static bool Run(const char* loaded, bool expect, int expectQueries)
{
    static CGameScriptInterface gsi;
    NScript::CQ_AmbushTradersScript s;
    s.GSI = &gsi; s.PMasterData = 0;
    g_loaded = loaded; g_ctors = g_dtors = g_queries = 0;
    bool r = s.CheckForValidRegion();
    return r == expect && g_queries == expectQueries && g_ctors == expectQueries && g_dtors == g_ctors;
}

int main()
{
    bool ok = true;
    ok = ok && Run("BowerstoneJail", true, 1);   // short-circuits after the first region
    ok = ok && Run("DesertedFarm", true, 2);
    ok = ok && Run("GibbetWoods", true, 3);
    ok = ok && Run("Oakvale", false, 3);
    if (!ok) { std::printf("00f0b890_TEST FAIL\n"); return 1; }
    std::printf("00f0b890_TEST PASS\n");
    return 0;
}