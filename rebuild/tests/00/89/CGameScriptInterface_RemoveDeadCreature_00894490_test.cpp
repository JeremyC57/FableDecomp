#include <cstdio>
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
class CThing {
public:
    void Kill(bool param);                         // 0x004C9B80
};
class CThingFilter_HasScriptName {
public:
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& creatureName) : Name(creatureName) {}
};
class CThingSearchTools {
public:
    template <class F, class T> bool GetPFirstThing(const F& filter, T** out) const;   // <CThingFilter_HasScriptName,CThing> 0x0056D050
};
class CWorld {
public:
    unsigned char _pad00[0x20];
    CThingSearchTools* PThingSearchTools;          // +0x20 scoped_ptr<CThingSearchTools>
    CThingSearchTools& GetThingSearchTools() { return *PThingSearchTools; }
};
class CGameScriptInterface {
public:
    virtual void RemoveDeadCreature(const CCharString& creatureName) const;
    CWorld* World;                                 // +0x04
};
void CGameScriptInterface::RemoveDeadCreature(const CCharString& creatureName) const
{
    CThing* thing = 0;
    if (World->GetThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName(creatureName + "_DEAD_CREATURE"), &thing))
        thing->Kill(false);
}

static char g_buf[64]; static int g_copies, g_dtors, g_kills, g_searches; static bool g_killArg = true; static CThing* g_killed;
CCharString::CCharString(const CCharString& o) { m_pData = o.m_pData; ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
CCharString __fastcall operator+(const CCharString& left, const char* right)
{
    const char* l = (const char*)left.m_pData; int n = 0;
    while (*l) g_buf[n++] = *l++;
    while (*right) g_buf[n++] = *right++;
    g_buf[n] = 0;
    struct Raw { void* p; } raw; raw.p = g_buf;
    return *(CCharString*)&raw;
}
void CThing::Kill(bool b) { ++g_kills; g_killArg = b; g_killed = this; }
static CThing* g_found; static bool g_match;
template <class F, class T> bool CThingSearchTools::GetPFirstThing(const F& f, T** out) const
{
    ++g_searches;
    const char* a = (const char*)f.Name.m_pData; const char* b = "HERO_DEAD_CREATURE";
    g_match = true; while (*a || *b) { if (*a++ != *b++) { g_match = false; break; } }
    if (g_found && g_match) { *out = g_found; return true; }
    return false;
}
int main()
{
    static CThing victim; CThingSearchTools tools; CWorld w; w.PThingSearchTools = &tools;
    CGameScriptInterface gsi; gsi.World = &w;
    struct Raw { void* p; } raw; raw.p = (void*)"HERO";
    const CCharString& creatureName = *(CCharString*)&raw;
    g_found = 0; gsi.RemoveDeadCreature(creatureName);
    bool ok = g_searches == 1 && g_match && g_kills == 0 && g_dtors == 2 && g_copies == 2;   // one copy is the mock operator+ return, one the filter member
    g_found = &victim; gsi.RemoveDeadCreature(creatureName);
    ok = ok && g_searches == 2 && g_kills == 1 && g_killed == &victim && !g_killArg && g_dtors == 4 && g_copies == 4;
    std::printf(ok ? "00894490_TEST PASS\n" : "00894490_TEST FAIL\n");
    return ok ? 0 : 1;
}