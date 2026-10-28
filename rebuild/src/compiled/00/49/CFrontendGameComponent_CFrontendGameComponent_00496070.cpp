// CFrontendGameComponent::CFrontendGameComponent(CMainGameComponentParent* parent, long unused) -- construct the debug front end: game-component base, empty counted pointers / name lists / strings, the movement timer, then (with the default-profile flag) load the default profile and enter mode 1, else enter the profile list mode 12; reset the create flags, load the graphics and rebuild the key-to-number table
// Leaf: ??0CFrontendGameComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00496070, length 592 bytes, `ret 8` (the second argument is unused).
// Callees: CGameComponent::CGameComponent(CMainGameComponentParent*) @ 0x0051D8E0, CWideString::CWideString() @ 0x0099AED0
//   (manifest "CCharString" -- byte-identical body), GFGetTime @ 0x009E1BC0, CTBaseSingleton<CUserProfileManager>::Get
//   @ 0x0040D2A0, CUserProfileManager::GetDefaultProfileName @ 0x004065D0, SetProfileName(const CWideString&, bool)
//   @ 0x0040D400, ~CWideString @ 0x0099B510, the mode-10 preparation @ 0x00495890 (manifest "CBrushLibraryDialog::
//   FillInListBox" is propagated), the profile-name list refresh @ 0x004959E0 (`ecx` = the vector), InitGraphics @ 0x00494840.
// Globals: the default-profile flag @ 0x013B8644, the key-to-number table @ 0x013B87B0 (122 longs; DIK scan codes of the top-row
//   digits and the numeric keypad map to 0..9, DIK_A to 11 and DIK_B to 12).
// Layout: the landed ~CFrontendGameComponent 0x00495620 (retail = Ego_r.pdb less 4 per STLport vector); the body fields from
//   the PDB: CreateGame +0x10, CreateGameJoystickDeviceNumber +0x14, CreateEditor +0x18, CreateReleaseFrontEnd +0x19, Mode +0x1c,
//   NoConnections +0x20, NoSessions +0x24, String +0x168, StringPos +0x1e8, First/LastSessionString +0x1df0/+0x1df4,
//   NoSessionStrings +0x1df8.
// Shape: SetMode (out of line @ 0x00495FE0) is inlined twice and folds per constant mode; no /EHsc (no unwind frame).
//   The profile load goes through a static inline (name, load) helper that fetches the singleton itself: retail builds the
//   GetDefaultProfileName() temporary BEFORE pushing `true` and calls Get() for SetProfileName after the pushes (a direct
//   `Get().SetProfileName(Get().GetDefaultProfileName(), true)` pushes `true` first; a member wrapper calls Get() early).
//   The name vectors use the real STLport _Vector_base / _STLP_alloc_proxy shape (the "24i" lesson) so the CWideString
//   ctor's `lea ecx` lands after two of the three zero stores. The key table is written keypad first, then the top row.
#include <string.h>

struct CMainGameComponentParent;
struct CBase {
    virtual ~CBase();                                      // 0x0099A300
};
struct CGameComponentInterface {
    virtual void v00();
};
class CGameComponent : public CBase, public CGameComponentInterface { public:   // vtables 0x0122ECB0 / 0x0122ECA4
    bool m_bShutDown;                                      // +0x08
    CGameComponent(CMainGameComponentParent* parent);      // 0x0051D8E0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CWideString { public:
    void* PStringData;
    CWideString();                                         // 0x0099AED0
    ~CWideString();                                        // 0x0099B510
    const CWideString& operator=(const CWideString& other);
};
struct CWideStringAllocator {                              // STLport allocator<CWideString> (empty)
    CWideStringAllocator() { }
};
struct CWideStringStorageProxy : CWideStringAllocator {    // STLport _STLP_alloc_proxy<CWideString*, ...>
    CWideString* _M_data;
    CWideStringStorageProxy(const CWideStringAllocator& a, CWideString* p) : CWideStringAllocator(a), _M_data(p) { }
};
struct CWideStringVectorBase {                             // STLport _Vector_base<CWideString>
    CWideString*            _M_start;
    CWideString*            _M_finish;
    CWideStringStorageProxy _M_end_of_storage;
    CWideStringVectorBase(const CWideStringAllocator& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
struct CWideStringVector : CWideStringVectorBase {         // STLport vector<CWideString>
    CWideStringVector(const CWideStringAllocator& a = CWideStringAllocator()) : CWideStringVectorBase(a) { }
};
void __fastcall GetSaveNames(CWideStringVector* names);    // 0x00495830
void __fastcall GetProfileNames(CWideStringVector* names); // 0x004959E0
class CTexture { public:
    void* PTexture;
    void* PData;
    CTexture() : PTexture(0), PData(0) { }
};
template <class T> struct CTBaseSingleton {
    static T& __fastcall Get();                            // 0x0040D2A0 for CUserProfileManager
};
class CUserProfileManager { public:
    CWideString GetDefaultProfileName() const;             // 0x004065D0
    void SetProfileName(const CWideString& name, bool load);   // 0x0040D400
    static void LoadProfile(const CWideString& name, bool load) { CTBaseSingleton<CUserProfileManager>::Get().SetProfileName(name, load); }
};
double __fastcall GFGetTime();                             // 0x009E1BC0
extern bool g_bLoadDefaultProfile;                         // 0x013B8644
extern CWideString g_EmptyWideString;
extern long g_KeyToNumber[122];                            // 0x013B87B0

class CGraphicDataBank; class CFontBank;
enum EGameMode { GAME_MODE_NONE = 0 };
enum ECursorMovement { CURSOR_MOVEMENT_NONE = 0 };

struct CFrontendGameComponent : CGameComponent {           // vtables 0x01238768 / 0x0123875C
    unsigned char                     _pad00c[0x4];
    bool                              CreateGame;                 // +0x10
    long                              CreateGameJoystickDeviceNumber;   // +0x14
    bool                              CreateEditor;               // +0x18
    bool                              CreateReleaseFrontEnd;      // +0x19
    long                              Mode;                       // +0x1c
    long                              NoConnections;              // +0x20
    long                              NoSessions;                 // +0x24
    unsigned char                     _pad028[0x168 - 0x28];      // Connections, Sessions
    char                              String[128];                // +0x168
    long                              StringPos;                  // +0x1e8
    unsigned char                     _pad1ec[0x1df0 - 0x1ec];    // CurrentConnection, TempBuffer, SessionString
    long                              FirstSessionString;         // +0x1df0
    long                              LastSessionString;          // +0x1df4
    long                              NoSessionStrings;           // +0x1df8
    CCountedPointer<CGraphicDataBank> PTempGraphics;              // +0x1dfc
    CCountedPointer<const CFontBank>  MainFont;                   // +0x1e04
    CWideStringVector                 ProfileNames;               // +0x1e0c
    long                              ProfileNamesOffset;         // +0x1e18
    CWideStringVector                 SaveNames;                  // +0x1e1c
    long                              SaveNamesOffset;            // +0x1e28
    CWideString                       SaveToLoad;                 // +0x1e2c
    CWideStringVector                 WorldNames;                 // +0x1e30
    long                              WorldNamesOffset;           // +0x1e3c
    CWideString                       WorldToLoad;                // +0x1e40
    long                              CursorHighlight;            // +0x1e44
    double                            TimeLastMovementHappened;   // +0x1e48
    bool                              StartedRepeatingMovement;   // +0x1e50
    ECursorMovement                   CurrentMovement;            // +0x1e54
    CTexture                          SplashScreen;               // +0x1e58

    CFrontendGameComponent(CMainGameComponentParent* parent, long unused);
    virtual ~CFrontendGameComponent();
    void SetMode(long mode)
    {
        Mode = mode;
        CursorHighlight = 1;
        memset(String, 0, sizeof(String));
        StringPos = 0;
        if (Mode == 10) {
            GetWorldNames();
            return;
        }
        if (mode == 11) {
            GetSaveNames(&SaveNames);
            WorldToLoad = g_EmptyWideString;
            SaveNamesOffset = 0;
            return;
        }
        if (mode == 12) {
            GetProfileNames(&ProfileNames);
            ProfileNamesOffset = 0;
        }
    }
private:
    bool InitGraphics();   // 0x00494840
    void GetWorldNames();   // 0x00495890
};

CFrontendGameComponent::CFrontendGameComponent(CMainGameComponentParent* parent, long unused)
    : CGameComponent(parent), CursorHighlight(1), TimeLastMovementHappened(GFGetTime()), StartedRepeatingMovement(false),
      CurrentMovement(CURSOR_MOVEMENT_NONE)
{
    Mode = 0;
    m_bShutDown = false;
    NoConnections = 0;
    NoSessions = 0;
    FirstSessionString = 0;
    LastSessionString = 0;
    NoSessionStrings = 0;
    if (g_bLoadDefaultProfile) {
        CUserProfileManager::LoadProfile(CTBaseSingleton<CUserProfileManager>::Get().GetDefaultProfileName(), true);
        SetMode(1);
    } else {
        SetMode(12);
    }
    m_bShutDown = false;
    CreateGame = false;
    CreateGameJoystickDeviceNumber = 0;
    CreateEditor = false;
    CreateReleaseFrontEnd = false;
    InitGraphics();
    memset(g_KeyToNumber, 0, sizeof(g_KeyToNumber));
    g_KeyToNumber[82] = 0;
    g_KeyToNumber[79] = 1;
    g_KeyToNumber[80] = 2;
    g_KeyToNumber[81] = 3;
    g_KeyToNumber[75] = 4;
    g_KeyToNumber[76] = 5;
    g_KeyToNumber[77] = 6;
    g_KeyToNumber[71] = 7;
    g_KeyToNumber[72] = 8;
    g_KeyToNumber[73] = 9;
    g_KeyToNumber[11] = 0;
    g_KeyToNumber[2] = 1;
    g_KeyToNumber[3] = 2;
    g_KeyToNumber[4] = 3;
    g_KeyToNumber[5] = 4;
    g_KeyToNumber[6] = 5;
    g_KeyToNumber[7] = 6;
    g_KeyToNumber[8] = 7;
    g_KeyToNumber[9] = 8;
    g_KeyToNumber[10] = 9;
    g_KeyToNumber[30] = 11;
    g_KeyToNumber[48] = 12;
}