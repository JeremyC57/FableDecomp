// NUISystem::CFrontEndManager::AddProfileNameToScreens(const CWideString& profileName) -- prefix "<profile> - " to the title bars of the new game, load game, options, game options, video, scoreboard, redefine keys and audio options screens
// Leaf: ?AddProfileNameToScreens@
// Retail: Fable.exe @ 0x00595B24, 391 bytes, `ret 4`.
// Body: prefix = profileName + L" - " (operator+ 0x0099BF30, __fastcall hidden result, literal 0x0122E070; built in the
// dead profileName slot); AddPrefixToTitleBar (0x00595AD9) for screens 0 UI_TEXT_NEW_GAME, 0 UI_TEXT_LOAD_GAME,
// 24 / 1 UI_TEXT_OPTIONS_MENU_TITLE, 1 UI_TEXT_GAME_OPTIONS_MENU_TITLE, 5 UI_TEXT_VIDEO_MENU_TITLE, 25
// UI_TEXT_SCOREBOARD_MENU_TITLE, 22 UI_TEXT_REDEFINE_KEYS_MENU_TITLE, 4 UI_TEXT_AUDIO_OPTIONS_MENU_TITLE, each with a
// CCharString(name) temporary (0x0099EBF0 / ~ 0x0099EAE0); ~CWideString 0x0099B510.
// Layout: CFrontEndManager only as `this` for the out-of-line AddPrefixToTitleBar.
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
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