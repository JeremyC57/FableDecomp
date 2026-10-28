// NUISystem::CFrontEndManager::GetScoreboardOptionsToProfile() -- when the scoreboard options screen is on top of the screen stack, copy its login, password, proxy IP, proxy port and server port edit boxes back into the user profile manager
// Leaf: ?GetScoreboardOptionsToProfile@
// Retail: Fable.exe @ 0x00596550, 418 bytes, plain `ret`. Reverse twin of SetScoreboardOptionsFromProfile 0x005963DB.
// Body: m_stackScreens.top() (deque::back 0x0059B039) == m_mapScreens[25] (map::operator[] 0x0059B5D7; VC calls it first) ->
// CUserProfileManager::Get() (0x0040D2A0) setters fed by GetScoreboardEditBox(CCharString(name)) (0x00596379, 0x0099EBF0):
// the three strings through CWideString::operator= (0x0099B7D0), the two ports through
// GFCharStringToInt(text.ConvertToCharString()) (0x0099E7F0 __fastcall / 0x0099B2C0).
// Layout: CFrontEndManager m_stackScreens +0x20, m_mapScreens +0x54 (the landed CanAcceptOptionChanges 0x005966F2 model);
// CUserProfileManager scoreboard fields as in 0x005963DB (retail 4 below Ego_r.pdb: +0x110..+0x120).
// Shape: inline profile setters (the argument is evaluated before CUserProfileManager::Get()).
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
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