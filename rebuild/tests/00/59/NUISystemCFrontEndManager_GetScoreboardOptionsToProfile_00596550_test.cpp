#include <cstdio>
enum FRONTEND_SCREEN { FRONTEND_SCREEN_25 = 25 };
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    ~CWideString();                                      // 0x0099B510
    const CWideString& operator=(const CWideString& other);    // 0x0099B7D0
    CCharString ConvertToCharString() const;             // 0x0099B2C0
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
class CUserProfileManager {
public:
    unsigned char _pad000[0x110];
    CWideString   ScoreBoardUserName;                    // +0x110
    CWideString   ScoreBoardPassword;                    // +0x114
    CWideString   ScoreBoardProxyIp;                     // +0x118
    long          ScoreBoardProxyPort;                   // +0x11c
    long          ScoreBoardServerPort;                  // +0x120
    static CUserProfileManager& __fastcall Get();        // 0x0040D2A0
    void SetScoreBoardUserName(const CWideString& name) { ScoreBoardUserName = name; }
    void SetScoreBoardPassword(const CWideString& password) { ScoreBoardPassword = password; }
    void SetScoreBoardProxyIp(const CWideString& ip) { ScoreBoardProxyIp = ip; }
    void SetScoreBoardProxyPort(long port) { ScoreBoardProxyPort = port; }
    void SetScoreBoardServerPort(long port) { ScoreBoardServerPort = port; }
};
namespace NUISystem {
class CComponent;
}
namespace std {
template <class T> class deque {
public:
    unsigned char _body[0x28];
    T& back();                                           // 0x0059B039 for NUISystem::CComponent*
};
template <class T, class Seq = deque<T> > class stack {
public:
    Seq c;
    T& top() { return c.back(); }
};
template <class K, class V> class map {
public:
    unsigned char _body[0x10];
    V& operator[](const K& key);                         // 0x0059B5D7 for FRONTEND_SCREEN -> CComponent*
};
}
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char                                   _pad00[0x20];
    std::stack<CComponent*>                         m_stackScreens;     // +0x20
    unsigned char                                   m_vComponents[0xc]; // +0x48 vector<CComponent*>
    std::map<FRONTEND_SCREEN, CComponent*>          m_mapScreens;       // +0x54
    void GetScoreboardOptionsToProfile();
protected:
    CWideString GetScoreboardEditBox(const CCharString& name);         // 0x00596379
    friend struct CFrontEndManager_Test;                 // behaviour-test hook
};
void CFrontEndManager::GetScoreboardOptionsToProfile()
{
    if (m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_25]) {
        CUserProfileManager::Get().SetScoreBoardUserName(GetScoreboardEditBox("UI_SCOREBOARD_LOGIN_EDITBOX_FE"));
        CUserProfileManager::Get().SetScoreBoardPassword(GetScoreboardEditBox("UI_SCOREBOARD_PASSWORD_EDITBOX_FE"));
        CUserProfileManager::Get().SetScoreBoardProxyIp(GetScoreboardEditBox("UI_SCOREBOARD_PROXY_IP_EDITBOX_FE"));
        CUserProfileManager::Get().SetScoreBoardProxyPort(GFCharStringToInt(GetScoreboardEditBox("UI_SCOREBOARD_PROXY_PORT_EDITBOX_FE").ConvertToCharString()));
        CUserProfileManager::Get().SetScoreBoardServerPort(GFCharStringToInt(GetScoreboardEditBox("UI_SCOREBOARD_SERVER_PORT_EDITBOX_FE").ConvertToCharString()));
    }
}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); g_fail = 1; } }
static NUISystem::CComponent* g_top; static NUISystem::CComponent* g_mapval; static int g_key = -1;
template <> NUISystem::CComponent*& std::deque<NUISystem::CComponent*>::back() { return g_top; }
template <> NUISystem::CComponent*& std::map<FRONTEND_SCREEN, NUISystem::CComponent*>::operator[](const FRONTEND_SCREEN& k) { g_key = k; return g_mapval; }
static int g_cctors, g_cdtors, g_wdtors, g_assigns, g_converts;
CCharString::CCharString(const char* text, long length) : PStringData((CCharStringData*)text) { ++g_cctors; check(length == -1, "length -1"); }
CCharString::~CCharString() { ++g_cdtors; }
CWideString::~CWideString() { ++g_wdtors; }
const CWideString& CWideString::operator=(const CWideString& other) { ++g_assigns; PStringData = other.PStringData; return *this; }
struct Pod { void* p; };
CCharString CWideString::ConvertToCharString() const
{
    ++g_converts;
    Pod pod; pod.p = PStringData;                        // the edit box text stands in as narrow text
    return *reinterpret_cast<CCharString*>(&pod);
}
long __fastcall GFCharStringToInt(const CCharString& text) { return atol((const char*)text.PStringData); }
static unsigned char g_pmStorage[sizeof(CUserProfileManager)];
CUserProfileManager& __fastcall CUserProfileManager::Get() { return *reinterpret_cast<CUserProfileManager*>(g_pmStorage); }
static const char* g_asked[8]; static int g_gets;
CWideString NUISystem::CFrontEndManager::GetScoreboardEditBox(const CCharString& name)
{
    const char* n = (const char*)name.PStringData;
    g_asked[g_gets++] = n;
    Pod pod;
    if (strstr(n, "LOGIN")) pod.p = (void*)"hero";
    else if (strstr(n, "PASSWORD")) pod.p = (void*)"secret";
    else if (strstr(n, "PROXY_IP")) pod.p = (void*)"10.0.0.1";
    else if (strstr(n, "PROXY_PORT")) pod.p = (void*)"8080";
    else pod.p = (void*)"27015";
    return *reinterpret_cast<CWideString*>(&pod);
}
int main()
{
    CUserProfileManager& pm = CUserProfileManager::Get();
    static NUISystem::CFrontEndManager fem;
    NUISystem::CComponent* screen25 = (NUISystem::CComponent*)0x1000; NUISystem::CComponent* other = (NUISystem::CComponent*)0x2000;
    // another screen on top: nothing copied
    g_top = other; g_mapval = screen25;
    fem.GetScoreboardOptionsToProfile();
    check(g_key == 25 && g_gets == 0 && g_assigns == 0, "not on top");
    // scoreboard options on top
    g_top = screen25;
    fem.GetScoreboardOptionsToProfile();
    check(g_gets == 5 && g_assigns == 3 && g_converts == 2, "copied");
    check(strcmp(g_asked[0], "UI_SCOREBOARD_LOGIN_EDITBOX_FE") == 0 && strcmp(g_asked[1], "UI_SCOREBOARD_PASSWORD_EDITBOX_FE") == 0
       && strcmp(g_asked[2], "UI_SCOREBOARD_PROXY_IP_EDITBOX_FE") == 0 && strcmp(g_asked[3], "UI_SCOREBOARD_PROXY_PORT_EDITBOX_FE") == 0
       && strcmp(g_asked[4], "UI_SCOREBOARD_SERVER_PORT_EDITBOX_FE") == 0, "edit box names");
    check(strcmp((const char*)pm.ScoreBoardUserName.PStringData, "hero") == 0 && strcmp((const char*)pm.ScoreBoardPassword.PStringData, "secret") == 0
       && strcmp((const char*)pm.ScoreBoardProxyIp.PStringData, "10.0.0.1") == 0, "strings");
    check(pm.ScoreBoardProxyPort == 8080 && pm.ScoreBoardServerPort == 27015, "ports");
    check(g_cctors == 5 && g_cdtors == 5 + 2 && g_wdtors >= 5, "temporaries destroyed");
    if (!g_fail) std::printf("00596550_TEST PASS\n");
    return g_fail;
}