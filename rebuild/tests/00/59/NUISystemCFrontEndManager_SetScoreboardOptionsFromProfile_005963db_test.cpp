#include <cstdio>
class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString(const CWideString& s);                   // 0x0099B720
    ~CWideString();                                      // 0x0099B510
};
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    CWideString ToWideString() const;                    // 0x0099E480
};
CCharString __fastcall GFIntToCharString(long value);    // 0x0099F830
class CUserProfileManager {
public:
    unsigned char _pad000[0x110];
    CWideString   ScoreBoardUserName;                    // +0x110
    CWideString   ScoreBoardPassword;                    // +0x114
    CWideString   ScoreBoardProxyIp;                     // +0x118
    long          ScoreBoardProxyPort;                   // +0x11c
    long          ScoreBoardServerPort;                  // +0x120
    static CUserProfileManager& __fastcall Get();        // 0x0040D2A0
    __forceinline CWideString GetScoreBoardUserName() const { return ScoreBoardUserName; }
    __forceinline CWideString GetScoreBoardPassword() const { return ScoreBoardPassword; }
    __forceinline CWideString GetScoreBoardProxyIp() const { return ScoreBoardProxyIp; }
    __forceinline long GetScoreBoardProxyPort() const { return ScoreBoardProxyPort; }
    __forceinline long GetScoreBoardServerPort() const { return ScoreBoardServerPort; }
};
namespace NUISystem {
class CFrontEndManager {
public:
    void SetScoreboardOptionsFromProfile();
protected:
    void SetScoreboardEditBox(const CCharString& name, const CWideString& text);   // 0x0059630E
    friend struct CFrontEndManager_Test;                 // behaviour-test hook
};
void CFrontEndManager::SetScoreboardOptionsFromProfile()
{
    SetScoreboardEditBox("UI_SCOREBOARD_LOGIN_EDITBOX_FE", CUserProfileManager::Get().GetScoreBoardUserName());
    SetScoreboardEditBox("UI_SCOREBOARD_PASSWORD_EDITBOX_FE", CUserProfileManager::Get().GetScoreBoardPassword());
    SetScoreboardEditBox("UI_SCOREBOARD_PROXY_IP_EDITBOX_FE", CUserProfileManager::Get().GetScoreBoardProxyIp());
    SetScoreboardEditBox("UI_SCOREBOARD_PROXY_PORT_EDITBOX_FE", GFIntToCharString(CUserProfileManager::Get().GetScoreBoardProxyPort()).ToWideString());
    SetScoreboardEditBox("UI_SCOREBOARD_SERVER_PORT_EDITBOX_FE", GFIntToCharString(CUserProfileManager::Get().GetScoreBoardServerPort()).ToWideString());
}
}

#include <stdio.h>
#include <string.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); g_fail = 1; } }
static int g_wcopies, g_wdtors, g_cctors, g_cdtors, g_toWide, g_intCalls;
CWideString::CWideString(const CWideString& s) : PStringData(s.PStringData) { ++g_wcopies; }
CWideString::~CWideString() { ++g_wdtors; }
CCharString::CCharString(const char* text, long length) : PStringData((CCharStringData*)text) { ++g_cctors; check(length == -1, "length -1"); }
CCharString::~CCharString() { ++g_cdtors; }
struct WidePod { void* p; };
CWideString CCharString::ToWideString() const
{
    ++g_toWide;
    WidePod pod; pod.p = PStringData;                     // the "wide" text is the char buffer itself
    return *reinterpret_cast<CWideString*>(&pod);
}
static char g_intBuf[4][16];
CCharString __fastcall GFIntToCharString(long value)
{
    char* b = g_intBuf[g_intCalls++ & 3];
    sprintf(b, "%ld", value);
    return CCharString(b);
}
static unsigned char g_pmStorage[sizeof(CUserProfileManager)];
CUserProfileManager& __fastcall CUserProfileManager::Get() { return *reinterpret_cast<CUserProfileManager*>(g_pmStorage); }
static const char* g_names[8]; static const void* g_texts[8]; static int g_sets;
void NUISystem::CFrontEndManager::SetScoreboardEditBox(const CCharString& name, const CWideString& text)
{
    g_names[g_sets] = (const char*)name.PStringData; g_texts[g_sets] = text.PStringData; ++g_sets;
}
int main()
{
    CUserProfileManager& pm = CUserProfileManager::Get();
    static const wchar_t user[] = L"hero", pass[] = L"secret", ip[] = L"10.0.0.1";
    pm.ScoreBoardUserName.PStringData = (CWideStringData*)user;
    pm.ScoreBoardPassword.PStringData = (CWideStringData*)pass;
    pm.ScoreBoardProxyIp.PStringData = (CWideStringData*)ip;
    pm.ScoreBoardProxyPort = 8080; pm.ScoreBoardServerPort = 27015;
    NUISystem::CFrontEndManager fem;
    fem.SetScoreboardOptionsFromProfile();
    check(g_sets == 5, "five edit boxes");
    check(strcmp(g_names[0], "UI_SCOREBOARD_LOGIN_EDITBOX_FE") == 0 && g_texts[0] == user, "login");
    check(strcmp(g_names[1], "UI_SCOREBOARD_PASSWORD_EDITBOX_FE") == 0 && g_texts[1] == pass, "password");
    check(strcmp(g_names[2], "UI_SCOREBOARD_PROXY_IP_EDITBOX_FE") == 0 && g_texts[2] == ip, "proxy ip");
    check(strcmp(g_names[3], "UI_SCOREBOARD_PROXY_PORT_EDITBOX_FE") == 0 && strcmp((const char*)g_texts[3], "8080") == 0, "proxy port");
    check(strcmp(g_names[4], "UI_SCOREBOARD_SERVER_PORT_EDITBOX_FE") == 0 && strcmp((const char*)g_texts[4], "27015") == 0, "server port");
    check(g_toWide == 2 && g_intCalls == 2, "two conversions");
    check(g_wcopies == g_wdtors && g_cctors == g_cdtors, "temporaries balanced");
    if (!g_fail) std::printf("005963db_TEST PASS\n");
    return g_fail;
}