// NUISystem::CFrontEndManager::SetScoreboardOptionsFromProfile() -- fill the scoreboard options screen's login, password, proxy IP, proxy port and server port edit boxes from the user profile manager
// Leaf: ?SetScoreboardOptionsFromProfile@
// Retail: Fable.exe @ 0x005963DB, 373 bytes, plain `ret`.
// Body: SetScoreboardEditBox (0x0059630E) with CCharString(name) (0x0099EBF0, length -1) and the profile's
// ScoreBoardUserName / ScoreBoardPassword / ScoreBoardProxyIp copies (CWideString copy 0x0099B720, from
// CUserProfileManager::Get() 0x0040D2A0) then GFIntToCharString(ScoreBoardProxyPort / ScoreBoardServerPort) (0x0099F830,
// __fastcall hidden result).ToWideString() (0x0099E480); ~CWideString 0x0099B510, ~CCharString 0x0099EAE0.
// Layout: CFrontEndManager as the twin 0x0059630E. CUserProfileManager scoreboard fields sit 4 bytes below the Ego_r.pdb
// offsets in retail (+0x110 ScoreBoardUserName, +0x114 Password, +0x118 ProxyIp, +0x11C ProxyPort, +0x120 ServerPort;
// Ego_r +0x114..+0x124), so a local model.
// Names: the five UI_SCOREBOARD_*_EDITBOX_FE literals (0x01252350 / 0x0125232C / 0x01252308 / 0x012522E4 / 0x012522BC).
// Shape: the profile strings come through by-value getters (retail copies each into a temporary); the size TU only
// inlines them as __forceinline.
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
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