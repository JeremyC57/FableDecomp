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
// GotoSpecificPreviousScreen 0x0059686D or the title screen + ResetAttractModeTimer 0x0042DBEE); the delete
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
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// NF83 (round 7, q11038 binder): the local CTBaseSingleton<T> model (static PInstance) replaced by engine/CTBaseSingleton.h (ISALLOCATED +
//   GET_DECL, the NF76 0x00599E3F set): the installer test reads the private static PClass (0x013CA818) through the inline IsAllocated().
//   Side effect: engine/CCountedPointer.h is now included at global scope first, so the later include inside namespace NUISystem is a
//   no-op and the own leaf spells the global ?$CCountedPointer@VCAction@NUISystem@@@@ (NF81's "second fault" gone). Still open (q11014 /
//   q11037): the virtual non-const PDB leaf (needs the CObserver base, engine/CFrontEndManager.h) and the CScreenStack::top stand-in of
//   deque::back 0x0059B039 (needs the local std models replaced by stlport).
#include "fable_compiler.h"
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
    FABLE_FORCEINLINE bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return Compare_Unsigned16Array(PStringData, other.PStringData);
    }
    static bool __cdecl Compare_Unsigned16Array(const void* a, const void* b);   // 0x0040F0A0
};
CWideString __fastcall operator+(const CWideString& text, const unsigned short* suffix);   // 0x0099BF30
CWideString __fastcall operator+(const CWideString& text, const CWideString& suffix);  // 0x0099BE70
// NF75 (round 7, land1117fu q10935): Ego_r.pdb 0x00B156F0 / FableWin.pdb 0x02F6E6F0 ?ConstructString@NGameText@@YI_NAAVCWideString@@ABV2@@Z
//   (namespace NGameText; engine/NGameText_CDataBank.h FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING); was namespace-less.
namespace NGameText {
bool __fastcall ConstructString(CWideString& text, const CWideString& parameter);     // 0x009C7980
}

double __fastcall GFGetTime();                            // 0x009E1BC0

namespace std {
template <class T> struct less { };                              // NF35: STLport 4.6 map takes four template arguments
template <class T1, class T2> struct pair { };                   //   (?$map@..U?$less@..@std@@V?$allocator@U?$pair@$$CB..@std@@@5@@std@@,
template <class T> class allocator { };                          //   Ego_r/FableWin + stlport/fable_map.h); declarations only
template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > class map
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
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
};
#define FABLE_RETAIL_CTBASESINGLETON_ISALLOCATED
#define FABLE_RETAIL_CTBASESINGLETON_GET_DECL
#include "engine/CTBaseSingleton.h"          // PClass 0x013CA818 (CFileInstaller); Get 0x00404440 / 0x0040D2A0

namespace NGameText {
class CDataBank
{
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}

// NF230 (2026-11-22, q12622): the row-local CNewFrontendGameComponent model (slot 4 `v04`, m_bCreateGame +0x29, m_pTextBank +0x60,
//   ResetAttractModeTimer) and the stand-in ?g_pNewFrontendGameComponent@@3PAVCNewFrontendGameComponent@@A -> engine/CNewFrontendGameComponent.h:
//   0x013B871C is the private static ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A (Ego_d.pdb LF_STMEMBER private; the header's spelling) read through
//   the inline Get() (FableWin.pdb ?Get@CNewFrontendGameComponent@@SIPAV1@XZ); the bank through GetTextBank() (FableWin.pdb
//   ?GetTextBank@CNewFrontendGameComponent@@QBEABVCDataBank@NGameText@@XZ); 0x0F's slot 4 + `m_bCreateGame = true` is the inline
//   StartGame() (Ego_r.pdb ?StartGame@CNewFrontendGameComponent@@QAEXXZ, the NF76 STARTGAME opt-in, as 0x00599E3F); 0x128's slot 4 is the
//   public override SetQuit() (FableWin.pdb ?SetQuit@CNewFrontendGameComponent@@UAEXXZ; retail RTTI vtable slot 4 = 0x00430340: opt-in);
//   ResetAttractModeTimer from its opt-in (Ego_r + FableWin ?ResetAttractModeTimer@CNewFrontendGameComponent@@QAEXXZ 0x0042DBEE).
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_STARTGAME
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RESETATTRACTMODETIMER
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT
#include "engine/CNewFrontendGameComponent.h"

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
#include "engine/CCountedPointer.h"

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

    protected:
    void SetScoreboardOptionsFromProfile();
    public:
                       // 0x005963DB
    void GotoNextScreen(CComponent* screen, bool clearStack);     // 0x00596763
    void GotoSpecificPreviousScreen(CComponent* screen);          // 0x0059686D
protected:
    // NF75 (round 7, land1117fu q10937 / q10938): Ego_r.pdb / FableWin.pdb protected (IAEJXZ; engine/CFrontEndManager.h
    //   REFRESHAVAILABLEPROFILES), the spellings the self rows 0x00596CCE / 0x00597006 now define; were public (QAE).
    long RefreshAvailableProfiles();                              // 0x00596CCE
    long RefreshAvailableProfilesForDelete();                     // 0x00597006
    // NF78 (round 7, q10954): GotoProfileMenu and DoPressStart are protected in Ego_r.pdb / FableWin.pdb (IAEXXZ); were public (QAE).
    void GotoProfileMenu();                                       // 0x00597B20
    void DoPressStart();                                          // 0x00599D5C
    // NF81 (round 7, q10982 / q10983): Ego_r.pdb / FableWin.pdb protected (?GotoNewProfileScreen@CFrontEndManager@NUISystem@@IAEXXZ,
    //   ?LoadProfile@CFrontEndManager@NUISystem@@IAEXABVCWideString@@@Z); were public (QAE). Access only.
    void GotoNewProfileScreen();                                  // 0x00595845
    void LoadProfile(const CWideString& name);                    // 0x0059899A
public:
    void GotoPreviousScreen(bool all);                            // 0x00597BF2
    protected: long RefreshAvailableSavedGamesForProfile(); public:                  // 0x00598463
    void Action(const CCountedPointer<CAction>& action);
};

void CFrontEndManager::Action(const CCountedPointer<CAction>& action)
{
    InstallationPauseStartTime = GFGetTime();
    if (!InstallationPaused && !FinalInstallationNeverPause && CTBaseSingleton<CFileInstaller>::IsAllocated())
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
        if (InstallationPaused && CTBaseSingleton<CFileInstaller>::IsAllocated())
        {
            CTBaseSingleton<CFileInstaller>::Get().Unpause();
            InstallationPaused = false;
            FinalInstallationNeverPause = true;
        }
    {
        CNewFrontendGameComponent* game = PGameComponent;
        game->StartGame();
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
            CUserProfileManager::Get().Save();
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
        if (!CUserProfileManager::Get().UseNewProfileValues
            && CUserProfileManager::Get().ProfileName == ProfileToDelete)
            deletingCurrent = true;
        CUserProfileManager::Get().DeleteProfile(ProfileToDelete);
        long count = RefreshAvailableProfiles();
        if (!deletingCurrent && count != 0)
        {
            GotoSpecificPreviousScreen(m_mapScreens[(FRONTEND_SCREEN)7]);
        }
        else
        {
            GotoNextScreen(m_mapScreens[(FRONTEND_SCREEN)0x14], true);
            CNewFrontendGameComponent::Get()->ResetAttractModeTimer();
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
                CWideString text = CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_DELETE_PROFILE");
                title->SetText(text + L" " + parameter->Value, true, true);
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
                CWideString text = CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_INVALID_SAVE");
                NGameText::ConstructString(text, parameter->Value);
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
        CUserProfileManager::Get().SetDefaultValuesForNewProfile();
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
        CNewFrontendGameComponent::Get()->SetQuit();
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
