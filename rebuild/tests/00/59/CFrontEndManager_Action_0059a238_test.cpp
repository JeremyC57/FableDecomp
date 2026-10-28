// Behaviour test for NUISystem::CFrontEndManager::Action (0x0059A238): the reconstruction (src/0059a238.cpp, verbatim)
// driven through recording doubles for every action type it handles -- the installer pause/unpause, each go-forward
// screen, load/new game, the delete-profile flows and their titles, the invalid-save message and the keyboard actions.
inline void* operator new(unsigned int, void* p) { return p; }
// NUISystem::CFrontEndManager::Action(const CCountedPointer<CAction>&) -- the front-end action dispatcher
// Retail 0x0059A238, 1486 bytes, `ret 4` (Ego_r.pdb ?Action@CFrontEndManager@NUISystem). Every action first stamps
// InstallationPauseStartTime (GFGetTime 0x009E1BC0) and, unless already paused / never to pause, pauses a running file
// installer (CTBaseSingleton<CFileInstaller> instance 0x013CA818, Get 0x00404440, Pause 0x009C31E0). Then the action
// type picks: a screen to go forward to (map::operator[] 0x0059B5D7 + GotoNextScreen 0x00596763); start a new game
// (0x0F: unpause the installer 0x009C3250 for good, game component slot 4, m_bCreateGame); the profile menu (0x10,
// 0x00597B20); remember the save to load (0x11: top of the screen stack 0x0059B039 to state 0, GameToLoad = the string
// parameter, CWideString::operator= 0x0099B7D0); saved games (0x42: 0x00598463 / NumberSaveGames, else start a new game
// and save the profile 0x0040A740); back (0x56, GotoPreviousScreen 0x00597BF2); delete the chosen profile (0xD6:
// CUserProfileManager 0x0040D2A0 DeleteProfile 0x00407040, RefreshAvailableProfiles 0x00596CCE, then
// GotoSpecificPreviousScreen 0x0059686D or the title screen + RestartAttractModeTimer 0x0042DBEE); the delete
// confirmation title "TEXT_GUI_MENU_DELETE_PROFILE" + " " + name (0xD7; GetTextBySymbol 0x009C95E0, operator+
// 0x0099BF30 / 0x0099BE70, slot 144); the invalid-save message (0xDC, ConstructString 0x009C7980); press start (0xE5,
// 0x00599D5C); new profile vs delete list (0xFA, 0x00597006); load the chosen profile (0x124, 0x0059899A); a fresh profile
// (0x125, 0x00409B70 + 0x00595845); the virtual keyboard (0x126 Confirm 0x00851920, 0x127 Cancel 0x00851860); the
// frontend's slot 4 (0x128); scoreboard options (0x138, 0x005963DB).
// Layout (retail CFrontEndManager, Ego_r.pdb minus the debug-only members): PGameComponent +0x1C, m_stackScreens +0x20,
// m_mapScreens +0x54, m_pVirtualKeyboard +0x60, m_bStartNewGame +0x64, PScreenToDeactivate +0x98,
// PScreenToDeactivateForStartGame/LoadGame +0xA4/+0xA8, GameToLoad +0xAC, ProfileToDelete +0xB0, NumberSaveGames +0xBC,
// InstallationPauseStartTime +0xD0, InstallationPaused +0xD8, FinalInstallationNeverPause +0xD9; CUserProfileManager
// ProfileName +0x08 / UseNewProfileValues +0x0C; CNewFrontendGameComponent m_bCreateGame +0x29, m_pTextBank +0x60
// (Ego_r.pdb 41 / 96); CActionParamString Value +0x08.
// Shape: frame-pointer size TU (`optimize("s")` + `("y",off)`, as the other CFrontEndManager rows). A same-TU noinline
// map::operator[] stand-in lets every screen-key temporary share the dead parameter word [ebp+8] (declared-only, each
// key gets its own slot: frame 0x74). The case holding the merged "go forward" tail is the first in the switch (0x13A);
// CWideString::operator== is the forced-inline PData test + Compare_Unsigned16Array (0x0040F0A0, bool); the game
// component of 0x0F is a local; the 0xD7 parameter is a CActionParamString* (its &Value is a spilled CSE, as retail).
// Leaf: ?Action@CFrontEndManager@NUISystem
#pragma optimize("s", on)
#pragma optimize("y", off)

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};

class CWideString
{
public:
    void* PStringData;
    ~CWideString();                                       // 0x0099B510
    const CWideString& operator=(const CWideString& other);     // 0x0099B7D0
    __forceinline bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return Compare_Unsigned16Array(PStringData, other.PStringData);
    }
    static bool __cdecl Compare_Unsigned16Array(const void* a, const void* b);   // 0x0040F0A0
};
CWideString __fastcall operator+(const CWideString& text, const char* suffix);         // 0x0099BF30
CWideString __fastcall operator+(const CWideString& text, const CWideString& suffix);  // 0x0099BE70
bool __fastcall ConstructString(CWideString& text, const CWideString& parameter);     // 0x009C7980

double __fastcall GFGetTime();                            // 0x009E1BC0

namespace std {
template <class K, class V> class map
{
public:
    void* _body[3];
    V& operator[](const K& key);                          // 0x0059B5D7 for <FRONTEND_SCREEN, CComponent*>
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> class list
{
public:
    _List_node<T>* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& front() { return _M_node->_M_next->_M_data; }
    T& back() { return _M_node->_M_prev->_M_data; }
};
}

class CFileInstaller
{
public:
    void Pause();                                         // 0x009C31E0
    void Unpause();                                       // 0x009C3250
};
class CUserProfileManager
{
public:
    unsigned char _pad00[0x08];
    CWideString   ProfileName;                            // +0x08
    bool          UseNewProfileValues;                    // +0x0C
    void DeleteProfile(const CWideString& name);          // 0x00407040
    void Save();                                          // 0x0040A740
    void SetDefaultValuesForNewProfile();                 // 0x00409B70
};
template <class T> class CTBaseSingleton
{
public:
    static T* PInstance;                                  // 0x013CA818 for CFileInstaller
    static T& __fastcall Get();                           // 0x00404440 (CFileInstaller) / 0x0040D2A0 (CUserProfileManager)
};

namespace NGameText {
class CDataBank
{
public:
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
}

class CNewFrontendGameComponent
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04();                                   // slot 4 (+0x10)
    unsigned char         _pad04[0x29 - 4];
    bool                  m_bCreateGame;                  // +0x29
    unsigned char         _pad2a[0x60 - 0x2a];
    NGameText::CDataBank* m_pTextBank;                    // +0x60 (CCountedPointer Data)
    void RestartAttractModeTimer();                       // 0x0042DBEE (m_attractModeTimer at +0x100)
};
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C

class CVirtualKeyboard
{
public:
    void Cancel();                                        // 0x00851860
    void Confirm();                                       // 0x00851920
};

enum FRONTEND_SCREEN { FRONTEND_SCREEN_0 = 0 };

namespace NUISystem { class CComponent; }
NUISystem::CComponent*& __fastcall ScreenSlotStandIn(void* map, long key);   // stand-in helper (no retail counterpart)

#pragma auto_inline(off)
// Same-TU stand-in for map<FRONTEND_SCREEN, CComponent*>::operator[] (0x0059B5D7, a COMDAT of this TU in retail): it
// reads the key by value and keeps no pointer to it, so every screen-key temporary can share one frame word.
template <> __declspec(noinline) NUISystem::CComponent*& std::map<FRONTEND_SCREEN, NUISystem::CComponent*>::operator[](const FRONTEND_SCREEN& key)
{
    return ScreenSlotStandIn(this, key);
}
#pragma auto_inline(on)

namespace NUISystem {

class CActionParam;
class CActionParamString
{
public:
    unsigned char _pad00[8];
    CWideString   Value;                                  // +0x08
};
class CAction
{
public:
    long                      ActionType;                 // +0x00
    std::list<CActionParam*>  UIActionParams;             // +0x04
};
template <class T> class CCountedPointer
{
public:
    T* Data;
    void* Info;
    T* operator->() const { return Data; }
};

class CComponent
{
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);      // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47();
    virtual void SetCurrentState(long state);             // slot 48 (+0xC0)
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72();
    virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78();
    virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89(); virtual void v90();
    virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95(); virtual void v96();
    virtual void v97(); virtual void v98(); virtual void v99(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
};

class CScreenStack
{
public:
    CComponent*& top();                                   // 0x0059B039 (std::stack<CComponent*>::top)
};

class CFrontEndManager
{
public:
    unsigned char                          _pad00[0x1c];
    CNewFrontendGameComponent*             PGameComponent;                    // +0x1C
    CScreenStack                           m_stackScreens;                    // +0x20
    unsigned char                          _pad24[0x54 - 0x24];
    std::map<FRONTEND_SCREEN, CComponent*> m_mapScreens;                      // +0x54
    CVirtualKeyboard*                      m_pVirtualKeyboard;                // +0x60
    bool                                   m_bStartNewGame;                   // +0x64
    unsigned char                          _pad65[0x98 - 0x65];
    CComponent*                            PScreenToDeactivate;               // +0x98
    unsigned char                          _pad9c[0xa4 - 0x9c];
    CComponent*                            PScreenToDeactivateForStartGame;   // +0xA4
    CComponent*                            PScreenToDeactivateForLoadGame;    // +0xA8
    CWideString                            GameToLoad;                        // +0xAC
    CWideString                            ProfileToDelete;                   // +0xB0
    unsigned char                          _padb4[0xbc - 0xb4];
    unsigned long                          NumberSaveGames;                   // +0xBC
    unsigned char                          _padc0[0xd0 - 0xc0];
    double                                 InstallationPauseStartTime;        // +0xD0
    bool                                   InstallationPaused;                // +0xD8
    bool                                   FinalInstallationNeverPause;       // +0xD9

    void GotoNewProfileScreen();                                  // 0x00595845
    void SetScoreboardOptionsFromProfile();                       // 0x005963DB
    void GotoNextScreen(CComponent* screen, bool clearStack);     // 0x00596763
    void GotoSpecificPreviousScreen(CComponent* screen);          // 0x0059686D
    long RefreshAvailableProfiles();                              // 0x00596CCE
    long RefreshAvailableProfilesForDelete();                     // 0x00597006
    void GotoProfileMenu();                                       // 0x00597B20
    void GotoPreviousScreen(bool all);                            // 0x00597BF2
    long RefreshAvailableSavedGamesForProfile();                  // 0x00598463
    void LoadProfile(const CWideString& name);                    // 0x0059899A
    void DoPressStart();                                          // 0x00599D5C
    void Action(const CCountedPointer<CAction>& action);
};

void CFrontEndManager::Action(const CCountedPointer<CAction>& action)
{
    InstallationPauseStartTime = GFGetTime();
    if (!InstallationPaused && !FinalInstallationNeverPause && CTBaseSingleton<CFileInstaller>::PInstance != 0)
    {
        CTBaseSingleton<CFileInstaller>::Get().Pause();
        InstallationPaused = true;
    }
    switch (action->ActionType)
    {
    case 0x13a:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x1a], false);
        break;
    case 0x09:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)1], false);
        break;
    case 0x0b:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)3], false);
        break;
    case 0x0c:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)4], false);
        break;
    case 0x0d:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)5], false);
        break;
    case 0x0e:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)6], false);
        break;
    case 0x0f:
        if (InstallationPaused && CTBaseSingleton<CFileInstaller>::PInstance != 0)
        {
            CTBaseSingleton<CFileInstaller>::Get().Unpause();
            InstallationPaused = false;
            FinalInstallationNeverPause = true;
        }
    {
        CNewFrontendGameComponent* game = PGameComponent;
        game->v04();
        game->m_bCreateGame = true;
    }
        break;
    case 0x10:
        GotoProfileMenu();
        break;
    case 0x11:
        if (!action->UIActionParams.empty())
        {
            CActionParamString* parameter = (CActionParamString*)action->UIActionParams.back();
            PScreenToDeactivateForLoadGame = m_stackScreens.top();
            PScreenToDeactivateForLoadGame->SetCurrentState(0);
            GameToLoad = parameter->Value;
        }
        break;
    case 0x42:
        if (RefreshAvailableSavedGamesForProfile() != 0 || NumberSaveGames > 0)
        {
            GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)8], false);
        }
        else
        {
            m_bStartNewGame = true;
            CTBaseSingleton<CUserProfileManager>::Get().Save();
            PScreenToDeactivateForStartGame = m_stackScreens.top();
            PScreenToDeactivateForStartGame->SetCurrentState(0);
        }
        break;
    case 0x43:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)9], false);
        break;
    case 0x56:
        GotoPreviousScreen(false);
        break;
    case 0xd6:
    {
        bool deletingCurrent = false;
        if (!CTBaseSingleton<CUserProfileManager>::Get().UseNewProfileValues
            && CTBaseSingleton<CUserProfileManager>::Get().ProfileName == ProfileToDelete)
            deletingCurrent = true;
        CTBaseSingleton<CUserProfileManager>::Get().DeleteProfile(ProfileToDelete);
        long count = RefreshAvailableProfiles();
        if (!deletingCurrent && count != 0)
        {
            GotoSpecificPreviousScreen(m_mapScreens[(FRONTEND_SCREEN)7]);
        }
        else
        {
            GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x14], true);
            g_pNewFrontendGameComponent->RestartAttractModeTimer();
            if (count > 0)
            {
                CComponent* screenToDeactivate = PScreenToDeactivate;
                GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)7], false);
                PScreenToDeactivate = screenToDeactivate;
            }
        }
        break;
    }
    case 0xd7:
        if (!action->UIActionParams.empty())
        {
            CActionParamString* parameter = (CActionParamString*)action->UIActionParams.back();
            ProfileToDelete = parameter->Value;
            CComponent* title = m_mapScreens[(FRONTEND_SCREEN)0x0a]->FindChild("UI_TEXT_DELETE_PROFILE_MENU_TITLE");
            if (title)
            {
                CWideString text = g_pNewFrontendGameComponent->m_pTextBank->GetTextBySymbol("TEXT_GUI_MENU_DELETE_PROFILE");
                title->SetText(text + " " + parameter->Value, true, true);
            }
            GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x0a], false);
        }
        break;
    case 0xdb:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x0f], false);
        break;
    case 0xdc:
        if (!action->UIActionParams.empty())
        {
            CComponent* message = m_mapScreens[(FRONTEND_SCREEN)0x10]->FindChild("UI_INVALID_SAVE_TEXT");
            CActionParamString* parameter = (CActionParamString*)action->UIActionParams.front();
            if (message && parameter)
            {
                CWideString text = g_pNewFrontendGameComponent->m_pTextBank->GetTextBySymbol("TEXT_GUI_MENU_INVALID_SAVE");
                ConstructString(text, parameter->Value);
                message->SetText(text, true, true);
            }
        }
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x10], false);
        break;
    case 0xe5:
        DoPressStart();
        break;
    case 0xfa:
        if (RefreshAvailableProfilesForDelete())
            GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x15], false);
        else
            GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x0c], false);
        break;
    case 0x11b:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x16], false);
        break;
    case 0x124:
        if (!action->UIActionParams.empty())
            LoadProfile(((CActionParamString*)action->UIActionParams.back())->Value);
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0], true);
        break;
    case 0x125:
        CTBaseSingleton<CUserProfileManager>::Get().SetDefaultValuesForNewProfile();
        GotoNewProfileScreen();
        break;
    case 0x126:
        if (m_pVirtualKeyboard)
            m_pVirtualKeyboard->Confirm();
        break;
    case 0x127:
        if (m_pVirtualKeyboard)
            m_pVirtualKeyboard->Cancel();
        break;
    case 0x128:
        g_pNewFrontendGameComponent->v04();
        break;
    case 0x129:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x18], false);
        break;
    case 0x138:
        SetScoreboardOptionsFromProfile();
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x19], false);
        break;
    case 0x141:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x1c], false);
        break;
    case 0xbd1:
        GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x14], false);
        break;
    }
}

}

// ---- behaviour harness: the reconstruction above, run against recording doubles ----
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static char g_arena[16384];
static int g_arenaUsed = 0;
static char* Keep(const char* a, const char* b = "")
{
    char* p = g_arena + g_arenaUsed;
    strcpy(p, a); strcat(p, b);
    g_arenaUsed += (int)strlen(p) + 1;
    return p;
}

CCharString::CCharString(const char* text, long) : PStringData((void*)text) {}
CCharString::~CCharString() {}
CWideString::~CWideString() {}
const CWideString& CWideString::operator=(const CWideString& other) { PStringData = other.PStringData; return *this; }
bool __cdecl CWideString::Compare_Unsigned16Array(const void* a, const void* b) { return strcmp((const char*)a, (const char*)b) == 0; }
static CWideString Wide(const char* s) { CWideString w; w.PStringData = (void*)s; return w; }
CWideString __fastcall operator+(const CWideString& text, const char* suffix) { return Wide(Keep((const char*)text.PStringData, suffix)); }
CWideString __fastcall operator+(const CWideString& text, const CWideString& suffix)
{
    return Wide(Keep((const char*)text.PStringData, (const char*)suffix.PStringData));
}
bool __fastcall ConstructString(CWideString& text, const CWideString& parameter)
{
    text.PStringData = Keep((const char*)text.PStringData, (const char*)parameter.PStringData);
    return true;
}
static double g_now = 0.0;
double __fastcall GFGetTime() { return g_now; }

void CFileInstaller::Pause() { Log("pause"); }
void CFileInstaller::Unpause() { Log("unpause"); }
static CFileInstaller g_installer;
template <> CFileInstaller* CTBaseSingleton<CFileInstaller>::PInstance = 0;
template <> CFileInstaller& __fastcall CTBaseSingleton<CFileInstaller>::Get() { return g_installer; }

static CUserProfileManager g_profiles;
template <> CUserProfileManager& __fastcall CTBaseSingleton<CUserProfileManager>::Get() { return g_profiles; }
void CUserProfileManager::DeleteProfile(const CWideString& name) { Log(Keep("delete:", (const char*)name.PStringData)); }
void CUserProfileManager::Save() { Log("save"); }
void CUserProfileManager::SetDefaultValuesForNewProfile() { Log("defaults"); }

CWideString NGameText::CDataBank::GetTextBySymbol(const CCharString& symbol)
{
    if (strcmp((const char*)symbol.PStringData, "TEXT_GUI_MENU_DELETE_PROFILE") == 0) return Wide("Delete");
    if (strcmp((const char*)symbol.PStringData, "TEXT_GUI_MENU_INVALID_SAVE") == 0) return Wide("Bad save ");
    return Wide("?");
}
void CNewFrontendGameComponent::v00() {} void CNewFrontendGameComponent::v01() {} void CNewFrontendGameComponent::v02() {}
void CNewFrontendGameComponent::v03() {}
void CNewFrontendGameComponent::v04() { Log(this == g_pNewFrontendGameComponent ? "frontend4" : "game4"); }
void CNewFrontendGameComponent::RestartAttractModeTimer() { Log("timer"); }
static NGameText::CDataBank g_bank;
static CNewFrontendGameComponent g_frontend;
static CNewFrontendGameComponent g_game;
CNewFrontendGameComponent* g_pNewFrontendGameComponent = &g_frontend;

void CVirtualKeyboard::Cancel() { Log("kbcancel"); }
void CVirtualKeyboard::Confirm() { Log("kbconfirm"); }
static CVirtualKeyboard g_keyboard;

namespace NUISystem {
#define V(n) void CComponent::v##n() {}
V(00) V(01) V(02) V(04) V(05) V(06) V(07) V(08) V(09)
V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29)
V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(49)
V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69)
V(70) V(71) V(72) V(73) V(74) V(75) V(76) V(77) V(78) V(79) V(80) V(81) V(82) V(83) V(84) V(85) V(86) V(87) V(88) V(89)
V(90) V(91) V(92) V(93) V(94) V(95) V(96) V(97) V(98) V(99) V(100) V(101) V(102) V(103) V(104) V(105) V(106) V(107) V(108)
V(109) V(110) V(111) V(112) V(113) V(114) V(115) V(116) V(117) V(118) V(119) V(120) V(121) V(122) V(123) V(124) V(125)
V(126) V(127) V(128) V(129) V(130) V(131) V(132) V(133) V(134) V(135) V(136) V(137) V(138) V(139) V(140) V(141) V(142) V(143)
#undef V
CComponent* CComponent::FindChild(CCharString) { return 0; }
void CComponent::SetCurrentState(long) {}
void CComponent::SetText(const CWideString&, bool, bool) {}
}
using namespace NUISystem;

struct TestComponent : CComponent
{
    const char* Name;
    TestComponent* Child;
    const char* ChildName;
    virtual CComponent* FindChild(CCharString name)
    {
        return (Child && strcmp((const char*)name.PStringData, ChildName) == 0) ? Child : 0;
    }
    virtual void SetCurrentState(long state) { char b[48]; sprintf(b, "%s.state=%ld", Name, state); Log(Keep(b)); }
    virtual void SetText(const CWideString& text, bool a, bool b)
    {
        Log(Keep(Name, Keep(".text=", (const char*)text.PStringData)));
        if (!a || !b) Log("badflags");
    }
};

static TestComponent g_screens[0x20];
static char g_screenNames[0x20][8];
static TestComponent g_top, g_title, g_message;
NUISystem::CComponent*& __fastcall ScreenSlotStandIn(void*, long key)
{
    static CComponent* slots[0x20];
    slots[key] = &g_screens[key];
    return slots[key];
}
CComponent*& CScreenStack::top() { static CComponent* t = &g_top; return t; }

static long g_savedGames = 0, g_deletable = 0, g_profilesLeft = 0;
static const char* ScreenName(CComponent* c)
{
    for (int i = 0; i < 0x20; ++i) if (c == &g_screens[i]) return g_screenNames[i];
    return "?";
}
void CFrontEndManager::GotoNewProfileScreen() { Log("newprofilescreen"); }
void CFrontEndManager::SetScoreboardOptionsFromProfile() { Log("scoreboard"); }
void CFrontEndManager::GotoNextScreen(CComponent* screen, bool clearStack)
{
    Log(Keep(clearStack ? "next!" : "next", ScreenName(screen)));
    PScreenToDeactivate = screen;
}
void CFrontEndManager::GotoSpecificPreviousScreen(CComponent* screen) { Log(Keep("back to", ScreenName(screen))); }
long CFrontEndManager::RefreshAvailableProfiles() { Log("refresh"); return g_profilesLeft; }
long CFrontEndManager::RefreshAvailableProfilesForDelete() { return g_deletable; }
void CFrontEndManager::GotoProfileMenu() { Log("profilemenu"); }
void CFrontEndManager::GotoPreviousScreen(bool all) { Log(all ? "previous!" : "previous"); }
long CFrontEndManager::RefreshAvailableSavedGamesForProfile() { return g_savedGames; }
void CFrontEndManager::LoadProfile(const CWideString& name) { Log(Keep("load:", (const char*)name.PStringData)); }
void CFrontEndManager::DoPressStart() { Log("pressstart"); }

static CFrontEndManager g_manager;
static CAction g_action;
static std::_List_node<CActionParam*> g_sentinel, g_node1, g_node2;
static CActionParamString g_param1, g_param2;

static void SetParams(int n)
{
    g_action.UIActionParams._M_node = &g_sentinel;
    g_sentinel._M_next = g_sentinel._M_prev = &g_sentinel;
    if (n >= 1)
    {
        g_node1._M_data = (CActionParam*)&g_param1;
        g_sentinel._M_next = g_sentinel._M_prev = &g_node1;
        g_node1._M_next = g_node1._M_prev = &g_sentinel;
    }
    if (n >= 2)
    {
        g_node2._M_data = (CActionParam*)&g_param2;
        g_node1._M_next = &g_node2; g_node2._M_prev = &g_node1;
        g_node2._M_next = &g_sentinel; g_sentinel._M_prev = &g_node2;
    }
}

static int Run(long type, int params, const char* expect)
{
    g_log[0] = 0;
    g_action.ActionType = type;
    SetParams(params);
    CCountedPointer<CAction> action;
    action.Data = &g_action;
    action.Info = 0;
    g_manager.Action(action);
    if (strcmp(g_log, expect) != 0)
    {
        printf("action %lx: got [%s] want [%s]\n", type, g_log, expect);
        return 0;
    }
    return 1;
}

int main()
{
    for (int i = 0; i < 0x20; ++i)
    {
        new (&g_screens[i]) TestComponent();
        sprintf(g_screenNames[i], "%x", i);
        g_screens[i].Name = g_screenNames[i];
        g_screens[i].Child = 0;
    }
    new (&g_top) TestComponent(); g_top.Name = "top"; g_top.Child = 0;
    new (&g_title) TestComponent(); g_title.Name = "title"; g_title.Child = 0;
    new (&g_message) TestComponent(); g_message.Name = "message"; g_message.Child = 0;
    g_screens[0x0a].Child = &g_title; g_screens[0x0a].ChildName = "UI_TEXT_DELETE_PROFILE_MENU_TITLE";
    g_screens[0x10].Child = &g_message; g_screens[0x10].ChildName = "UI_INVALID_SAVE_TEXT";
    g_manager.PGameComponent = &g_game;
    g_frontend.m_pTextBank = &g_bank;
    g_param1.Value = Wide("Alice");
    g_param2.Value = Wide("Bob");
    g_profiles.ProfileName = Wide("Alice");

    int ok = 1;
    // The installer is paused on the first action while one is running, and the pause time is stamped every time.
    CTBaseSingleton<CFileInstaller>::PInstance = &g_installer;
    g_now = 12.5;
    ok &= Run(0x09, 0, "pause;next1;");
    if (!g_manager.InstallationPaused || g_manager.InstallationPauseStartTime != 12.5) { printf("pause state\n"); ok = 0; }
    ok &= Run(0x0b, 0, "next3;");
    ok &= Run(0x0c, 0, "next4;");
    ok &= Run(0x0d, 0, "next5;");
    ok &= Run(0x0e, 0, "next6;");
    // Starting a game unpauses the installer for good.
    ok &= Run(0x0f, 0, "unpause;game4;");
    if (g_manager.InstallationPaused || !g_manager.FinalInstallationNeverPause || !g_game.m_bCreateGame) { printf("start game state\n"); ok = 0; }
    ok &= Run(0x09, 0, "next1;");
    ok &= Run(0x10, 0, "profilemenu;");
    ok &= Run(0x11, 0, "");
    ok &= Run(0x11, 2, "top.state=0;");
    if (g_manager.PScreenToDeactivateForLoadGame != &g_top || strcmp((const char*)g_manager.GameToLoad.PStringData, "Bob")) { printf("load game\n"); ok = 0; }
    g_savedGames = 1;
    ok &= Run(0x42, 0, "next8;");
    g_savedGames = 0; g_manager.NumberSaveGames = 3;
    ok &= Run(0x42, 0, "next8;");
    g_manager.NumberSaveGames = 0;
    ok &= Run(0x42, 0, "save;top.state=0;");
    if (!g_manager.m_bStartNewGame || g_manager.PScreenToDeactivateForStartGame != &g_top) { printf("new game\n"); ok = 0; }
    ok &= Run(0x43, 0, "next9;");
    ok &= Run(0x56, 0, "previous;");
    // Choosing a profile to delete names it in the confirmation title.
    ok &= Run(0xd7, 0, "");
    ok &= Run(0xd7, 2, "title.text=Delete Bob;nexta;");
    if (strcmp((const char*)g_manager.ProfileToDelete.PStringData, "Bob")) { printf("profile to delete\n"); ok = 0; }
    // Deleting another profile with some left: back to the profile list.
    g_profilesLeft = 2;
    ok &= Run(0xd6, 0, "delete:Bob;refresh;back to7;");
    // Deleting the current profile: title screen, attract timer, then the list (the title screen stays the one to deactivate).
    g_manager.ProfileToDelete = Wide(Keep("Alice"));
    ok &= Run(0xd6, 0, "delete:Alice;refresh;next!14;timer;next7;");
    if (g_manager.PScreenToDeactivate != &g_screens[0x14]) { printf("kept screen\n"); ok = 0; }
    // ... unless the profile manager is on new-profile values; with none left, only the title screen.
    g_profiles.UseNewProfileValues = true; g_profilesLeft = 0;
    ok &= Run(0xd6, 0, "delete:Alice;refresh;next!14;timer;");
    g_profiles.UseNewProfileValues = false;
    ok &= Run(0xdb, 0, "nextf;");
    ok &= Run(0xdc, 0, "next10;");
    ok &= Run(0xdc, 1, "message.text=Bad save Alice;next10;");
    ok &= Run(0xe5, 0, "pressstart;");
    g_deletable = 1;
    ok &= Run(0xfa, 0, "next15;");
    g_deletable = 0;
    ok &= Run(0xfa, 0, "nextc;");
    ok &= Run(0x11b, 0, "next16;");
    ok &= Run(0x124, 1, "load:Alice;next!0;");
    ok &= Run(0x124, 0, "next!0;");
    ok &= Run(0x125, 0, "defaults;newprofilescreen;");
    ok &= Run(0x126, 0, "");
    ok &= Run(0x127, 0, "");
    g_manager.m_pVirtualKeyboard = &g_keyboard;
    ok &= Run(0x126, 0, "kbconfirm;");
    ok &= Run(0x127, 0, "kbcancel;");
    ok &= Run(0x128, 0, "frontend4;");
    ok &= Run(0x129, 0, "next18;");
    ok &= Run(0x138, 0, "scoreboard;next19;");
    ok &= Run(0x13a, 0, "next1a;");
    ok &= Run(0x141, 0, "next1c;");
    ok &= Run(0xbd1, 0, "next14;");
    ok &= Run(0x0a, 0, "");
    ok &= Run(0x200, 0, "");
    if (ok)
        printf("FRONTEND_ACTION_0059A238_TEST PASS\n");
    else
        printf("FRONTEND_ACTION_0059A238_TEST did not pass\n");
    return ok ? 0 : 1;
}
