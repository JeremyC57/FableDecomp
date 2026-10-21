#include <cstdio>
enum FRONTEND_SCREEN { FRONTEND_SCREEN_NONE };
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
};
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30
namespace NUISystem {
class CFrontEndManager {
public:
    void AddProfileNameToScreens(const CWideString& profileName);
protected:
    void AddPrefixToTitleBar(FRONTEND_SCREEN screen, const CCharString& title_bar_name, const CWideString& prefix);   // 0x00595AD9
    friend struct CFrontEndManager_Test;                 // behaviour-test hook
};
void CFrontEndManager::AddProfileNameToScreens(const CWideString& profileName)
{
    CWideString prefix = profileName + L" - ";
    AddPrefixToTitleBar((FRONTEND_SCREEN)0, "UI_TEXT_NEW_GAME", prefix);
    AddPrefixToTitleBar((FRONTEND_SCREEN)0, "UI_TEXT_LOAD_GAME", prefix);
    AddPrefixToTitleBar((FRONTEND_SCREEN)24, "UI_TEXT_OPTIONS_MENU_TITLE", prefix);
    AddPrefixToTitleBar((FRONTEND_SCREEN)1, "UI_TEXT_OPTIONS_MENU_TITLE", prefix);
    AddPrefixToTitleBar((FRONTEND_SCREEN)1, "UI_TEXT_GAME_OPTIONS_MENU_TITLE", prefix);
    AddPrefixToTitleBar((FRONTEND_SCREEN)5, "UI_TEXT_VIDEO_MENU_TITLE", prefix);
    AddPrefixToTitleBar((FRONTEND_SCREEN)25, "UI_TEXT_SCOREBOARD_MENU_TITLE", prefix);
    AddPrefixToTitleBar((FRONTEND_SCREEN)22, "UI_TEXT_REDEFINE_KEYS_MENU_TITLE", prefix);
    AddPrefixToTitleBar((FRONTEND_SCREEN)4, "UI_TEXT_AUDIO_OPTIONS_MENU_TITLE", prefix);
}
}

#include <string.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); g_fail = 1; } }
static int g_cctors, g_cdtors, g_wdtors, g_plus;
CCharString::CCharString(const char* text, long length) : PStringData((CCharStringData*)text) { ++g_cctors; check(length == -1, "length -1"); }
CCharString::~CCharString() { ++g_cdtors; }
CWideString::~CWideString() { ++g_wdtors; }
static const CWideString* g_plusLeft; static const unsigned short* g_plusRight; static int g_prefixTag;
struct Pod { void* p; };
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right)
{
    ++g_plus; g_plusLeft = &left; g_plusRight = right;
    Pod pod; pod.p = &g_prefixTag;
    return *reinterpret_cast<CWideString*>(&pod);
}
static int g_screens[16]; static const char* g_titles[16]; static const void* g_prefixes[16]; static int g_calls;
void NUISystem::CFrontEndManager::AddPrefixToTitleBar(FRONTEND_SCREEN screen, const CCharString& name, const CWideString& prefix)
{
    g_screens[g_calls] = screen; g_titles[g_calls] = (const char*)name.PStringData; g_prefixes[g_calls] = prefix.PStringData; ++g_calls;
}
int main()
{
    static NUISystem::CFrontEndManager fem;
    Pod namePod; namePod.p = 0; CWideString& profile = *reinterpret_cast<CWideString*>(&namePod);
    fem.AddProfileNameToScreens(profile);
    check(g_plus == 1 && g_plusLeft == &profile && g_plusRight[0] == ' ' && g_plusRight[1] == '-' && g_plusRight[2] == ' ' && g_plusRight[3] == 0, "prefix built");
    static const int screens[9] = { 0, 0, 24, 1, 1, 5, 25, 22, 4 };
    static const char* titles[9] = { "UI_TEXT_NEW_GAME", "UI_TEXT_LOAD_GAME", "UI_TEXT_OPTIONS_MENU_TITLE", "UI_TEXT_OPTIONS_MENU_TITLE",
        "UI_TEXT_GAME_OPTIONS_MENU_TITLE", "UI_TEXT_VIDEO_MENU_TITLE", "UI_TEXT_SCOREBOARD_MENU_TITLE", "UI_TEXT_REDEFINE_KEYS_MENU_TITLE",
        "UI_TEXT_AUDIO_OPTIONS_MENU_TITLE" };
    check(g_calls == 9, "nine title bars");
    for (int i = 0; i < 9; ++i)
        check(g_screens[i] == screens[i] && strcmp(g_titles[i], titles[i]) == 0 && g_prefixes[i] == &g_prefixTag, titles[i]);
    check(g_cctors == 9 && g_cdtors == 9 && g_wdtors >= 1, "temporaries destroyed");
    if (!g_fail) std::printf("00595b24_TEST PASS\n");
    return g_fail;
}