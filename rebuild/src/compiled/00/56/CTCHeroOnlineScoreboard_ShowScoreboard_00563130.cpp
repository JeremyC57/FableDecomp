// CTCHeroOnlineScoreboard::ShowScoreboard() -- resets the scoreboard (connection dropped, TaskState 100) and, when the hero has a GUI component, creates the two "connecting" message components, registers them with the GUI, then either reports "not signed in" or starts the state machine (SelectedClan cleared, ScoreboardState 1)
// Leaf: ShowScoreboard
// Retail: Fable.exe @ 0x00563130, 736 bytes, `ret`. Ego_r.pdb `?ShowScoreboard@CTCHeroOnlineScoreboard@@QAEXXZ`.
// Callees: ClearScoreboard 0x00562E10, LHHttp2::~LHHttp2 0x008410D0 + operator delete 0x00BFE9BC (inline scoped_ptr
// reset), CVectorMap::LowerBound 0x0040F020 (inline CThing::GetTC for interface 0x11), CFrontEndManager::GetInstance
// 0x0041E5F2, CFrontEndManager::CreateComponent(const CCharString&, CComponent*) 0x0041DB1D, CCharString(const char*,
// long) 0x0099EBF0 / ~ 0x0099EAE0, the components' slots 43 (+0xac) and 48 (+0xc0, given a bool), the GUI component's
// slot 82 (+0x148, returns its component vector), vector<CComponent*>::_M_insert_overflow 0x00485DBC,
// CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, CWideString(const CWideString&) 0x0099B720 / ~ 0x0099B510,
// CWideString::ConvertToCharString 0x0099B2C0, CBasicString<char>::operator==(const char*) 0x004115A0,
// CDataBank::GetTextBySymbol 0x009C95E0 (text bank +0x14 of 0x013B86A0), DisplayErrorMessage 0x0055EF00,
// CWideString::operator=(const CWideString&) 0x0099B7D0 with the empty wide string 0x013BCA24.
// Layout: as the landed AbortWithMessage 0x00563410 / ResetScoreboard 0x00562E70: PHttp +0x0c, SelectedClan +0x3c,
// ScoreboardState +0x50, TaskState +0x54, PClanMenuEntry +0x5c, PConnectingMessage +0x60, PConnectingMessage1 +0x64.
// CUserProfileManager's signed-in user name +0x110 and password +0x114 (CWideString).
// Shape: GetSignedInUserName/GetSignedInPassword are inline by-value CCharString getters, compared to "" with the
// inline CCharString==; the `||` gives the conditional-temporary flags word.
#include <new>
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    CCharString ConvertToCharString() const;            // 0x0099B2C0
};
extern const CWideString g_emptyWideString;             // 0x013BCA24

struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CEngineGlobals {
    unsigned char _pad[0x14];
    CDataBank*    m_pTextBank;                          // +0x14
};
extern CEngineGlobals* g_pEngineGlobals_013b86a0;       // 0x013B86A0

class CUserProfileManager { public:
    unsigned char _pad[0x110];
    CWideString   SignedInUserName;                     // +0x110
    CWideString   SignedInPassword;                     // +0x114
};
CUserProfileManager* GetUserProfileManager();           // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)

struct CComponent {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void Initialise();                          // slot 43 (+0xac)
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void SetVisible(bool visible);              // slot 48 (+0xc0)
};

namespace std {
struct __true_type { __true_type() { } };
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fillLen, bool atEnd);   // 0x00485DBC
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
}

struct CTCGUI {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual std::vector<CComponent*>* GetComponents();  // slot 82 (+0x148)
};

struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
    CComponent* CreateComponent(const CCharString& name, CComponent* parent);   // 0x0041DB1D
};

struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  UsingComponents[9];                  // +0x20
    CInterfaceMap  Interfaces;                          // +0x44
    template<class T> __forceinline T* GetTC(long id)
    {
        long key = id;
        CInterfaceMap* m = &Interfaces;
        CInterfaceMap::SEntry* p = m->LowerBound(&key);
        if (p == m->last || id < p->type)
            p = m->last;
        return (T*)p->component;
    }
};

namespace LHNetworkLib {
class LHHttp2 { public:
    ~LHHttp2();                                         // 0x008410D0
};
}
struct CHttpScopedPtr {
    LHNetworkLib::LHHttp2* px;
    void reset(LHNetworkLib::LHHttp2* p = 0)
    {
        LHNetworkLib::LHHttp2* old = px;
        px = p;
        delete old;
    }
};

class CTCHeroOnlineScoreboard {
public:
    virtual void v00();
    CThing*        Thing;                               // +0x04
    long           m_8;                                 // +0x08
    CHttpScopedPtr PHttp;                               // +0x0c
    unsigned char  _pad10[0x3c - 0x10];
    CWideString    SelectedClan;                        // +0x3c
    unsigned char  _pad40[0x50 - 0x40];
    long           ScoreboardState;                     // +0x50
    long           TaskState;                           // +0x54
    CComponent*    PScoreboardScreen;                   // +0x58
    CComponent*    PClanMenuEntry;                      // +0x5c
    CComponent*    PConnectingMessage;                  // +0x60
    CComponent*    PConnectingMessage1;                 // +0x64
    void ClearScoreboard();                             // 0x00562E10
    void DisplayErrorMessage(const CWideString& text);  // 0x0055EF00
    CCharString GetSignedInUserName() { CWideString name = GetUserProfileManager()->SignedInUserName; return name.ConvertToCharString(); }
    CCharString GetSignedInPassword() { CWideString password = GetUserProfileManager()->SignedInPassword; return password.ConvertToCharString(); }
    void ShowScoreboard();
};

void CTCHeroOnlineScoreboard::ShowScoreboard()
{
    PClanMenuEntry = 0;
    ClearScoreboard();
    PHttp.reset();
    TaskState = 100;
    if (Thing->UsingComponents[0] & 0x20000) {
        CTCGUI* gui = Thing->GetTC<CTCGUI>(0x11);
        CFrontEndManager* manager = CFrontEndManager::GetInstance();
        PConnectingMessage = manager->CreateComponent("UI_CONNECTING", 0);
        PConnectingMessage->Initialise();
        PConnectingMessage->SetVisible(false);
        gui->GetComponents()->push_back(PConnectingMessage);
        PConnectingMessage1 = manager->CreateComponent("UI_CONNECTING1", 0);
        PConnectingMessage1->Initialise();
        gui->GetComponents()->push_back(PConnectingMessage1);
        PConnectingMessage1->SetVisible(true);
        bool notSignedIn = GetSignedInUserName() == "" || GetSignedInPassword() == "";
        if (notSignedIn) {
            CWideString text = g_pEngineGlobals_013b86a0->m_pTextBank->GetTextBySymbol("TEXT_GUI_SCOREBOARD_ERROR_LOGIN_DETAILS_MISSING");
            DisplayErrorMessage(text);
            return;
        }
        ScoreboardState = 1;
        SelectedClan = g_emptyWideString;
    }
}