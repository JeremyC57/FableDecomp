#include <cstdio>
class CCharString { public: void* PStringData; };
class CWideString { public: void* PStringData; };
class CUserProfileManager {
public:
    unsigned char _pad00[0x0c];
    bool          UseNewProfileValues;                   // +0x0c
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();                                           // 0x00412130 for CWideString
};
template <class T> class deque {
public:
    unsigned char _body[0x28];
    T& back();                                           // 0x0059B039 for CComponent*
};
template <class T, class C = deque<T> > class stack {
public:
    C c;
    T& top() { return c.back(); }
};
}
struct CMiscThingsDef {
    virtual void v00();
    virtual void DestroySelf();                          // slot 1 (+4)
    long          RefCount;                              // +0x04
    void DecRef() { --RefCount; if (RefCount == 0) DestroySelf(); }
    unsigned char _pad08[0x1a0 - 8];
    CCharString   NewProfileSound;                       // +0x1a0 (name not recovered)
};
template <class T> class CDefPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
    __forceinline ~CDefPointer()
    {
        if (Object)
            Object->DecRef();
    }
};
namespace NUISystem {
class CObserver {
public:
    virtual ~CObserver();
};
class CUIInterface {
public:
    virtual ~CUIInterface(); virtual void _u001(); virtual void _u002(); virtual void _u003(); virtual void _u004(); virtual void _u005(); virtual void _u006(); virtual void _u007(); virtual void _u008(); virtual void _u009(); virtual void _u010(); virtual void _u011(); virtual void _u012(); virtual void _u013(); virtual void _u014(); virtual void _u015(); virtual void _u016(); virtual void _u017(); virtual void _u018(); virtual void _u019(); virtual void _u020(); virtual void _u021(); virtual void _u022(); virtual void _u023(); virtual void _u024(); virtual void _u025(); virtual void _u026(); virtual void _u027(); virtual void _u028(); virtual void _u029(); virtual void _u030(); virtual void _u031(); virtual void _u032(); virtual void _u033(); virtual void _u034(); virtual void _u035(); virtual void _u036(); virtual void _u037(); virtual void _u038(); virtual void _u039(); virtual void _u040(); virtual void _u041(); virtual void _u042(); virtual void _u043(); virtual void _u044(); virtual void _u045(); virtual void _u046(); virtual void _u047();
    virtual void ChangeState(unsigned long state);      // slot 48 (+0xc0)
};
class CComponent : public CUIInterface, public CObserver {
public:
};
class CObservable {
public:
    virtual void _o00(); virtual void _o01(); virtual void _o02(); virtual void _o03(); virtual void _o04();
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
};
class CManager : public CObservable {
public:
    virtual void _m006(); virtual void _m007(); virtual void _m008(); virtual void _m009(); virtual void _m010(); virtual void _m011(); virtual void _m012(); virtual void _m013(); virtual void _m014(); virtual void _m015(); virtual void _m016(); virtual void _m017(); virtual void _m018(); virtual void _m019(); virtual void _m020(); virtual void _m021(); virtual void _m022(); virtual void _m023(); virtual void _m024(); virtual void _m025(); virtual void _m026(); virtual void _m027(); virtual void _m028(); virtual void _m029(); virtual void _m030(); virtual void _m031(); virtual void _m032(); virtual void _m033(); virtual void _m034(); virtual void _m035(); virtual void _m036(); virtual void _m037(); virtual void _m038(); virtual void _m039();
    virtual void PlaySound(CCharString& name, bool flag);           // slot 40 (+0xa0)
    virtual void _v041();
    virtual CDefPointer<CMiscThingsDef> GetMiscThingsDef();         // slot 42 (+0xa8)
    static CManager* __fastcall GetInstance();           // 0x0041E5F2
};
class CFrontEndManager {
public:
    unsigned char           _pad00[0x20];
    std::stack<CComponent*> m_stackScreens;              // +0x20
    unsigned char           _pad48[0x64 - 0x48];
    bool                    m_bStartNewGame;             // +0x64
    unsigned char           _pad65[0xa0 - 0x65];
    bool                    GoToKeyboard;                // +0xa0
    void GetProfileNames(std::vector<CWideString>& names);   // 0x005955AB
    void GotoNewProfileScreen();
};
void CFrontEndManager::GotoNewProfileScreen()
{
    CTBaseSingleton<CUserProfileManager>::Get().UseNewProfileValues = true;
    std::vector<CWideString> names;
    GetProfileNames(names);
    m_bStartNewGame = true;
    GoToKeyboard = true;
    CManager::GetInstance()->PlaySound(CManager::GetInstance()->GetMiscThingsDef()->NewProfileSound, true);
    CComponent* top = m_stackScreens.top();
    top->ChangeState(6);
    CManager::GetInstance()->RemoveObserver(&static_cast<CObserver&>(*top));
}
}

static int g_log[16], g_n;   // 1 Get, 2 GetProfileNames, 3 ~vector, 4 PlaySound, 5 GetDef, 6 Destroy, 7 ChangeState, 8 RemoveObserver
static void Log(int x) { g_log[g_n++] = x; }
static CUserProfileManager g_upm;
template <class T> T& __fastcall CTBaseSingleton<T>::Get() { Log(1); return g_upm; }
template <class T> std::vector<T>::~vector() { Log(3); }
static NUISystem::CComponent* g_top;
template <class T> T& std::deque<T>::back() { return *(T*)&g_top; }
static std::vector<CWideString>* g_names;
void NUISystem::CFrontEndManager::GetProfileNames(std::vector<CWideString>& names) { Log(2); g_names = &names; }
void CMiscThingsDef::v00() {}
void CMiscThingsDef::DestroySelf() { Log(6); }
static CMiscThingsDef g_def;
NUISystem::CObserver::~CObserver() {}
NUISystem::CUIInterface::~CUIInterface() {}
static unsigned long g_state; static NUISystem::CObserver* g_removed; static CCharString* g_sound; static bool g_flag;
void NUISystem::CUIInterface::ChangeState(unsigned long s) { Log(7); g_state = s; }
#define U(n) void NUISystem::CUIInterface::_u##n() {}
U(001) U(002) U(003) U(004) U(005) U(006) U(007) U(008) U(009) U(010) U(011) U(012) U(013) U(014) U(015) U(016) U(017)
U(018) U(019) U(020) U(021) U(022) U(023) U(024) U(025) U(026) U(027) U(028) U(029) U(030) U(031) U(032) U(033) U(034)
U(035) U(036) U(037) U(038) U(039) U(040) U(041) U(042) U(043) U(044) U(045) U(046) U(047)
#define O(n) void NUISystem::CObservable::_o##n() {}
O(00) O(01) O(02) O(03) O(04)
void NUISystem::CObservable::RemoveObserver(NUISystem::CObserver* o) { Log(8); g_removed = o; }
#define M(n) void NUISystem::CManager::_m##n() {}
M(006) M(007) M(008) M(009) M(010) M(011) M(012) M(013) M(014) M(015) M(016) M(017) M(018) M(019) M(020) M(021) M(022)
M(023) M(024) M(025) M(026) M(027) M(028) M(029) M(030) M(031) M(032) M(033) M(034) M(035) M(036) M(037) M(038) M(039)
void NUISystem::CManager::_v041() {}
void NUISystem::CManager::PlaySound(CCharString& n, bool f) { Log(4); g_sound = &n; g_flag = f; }
static CMiscThingsDef* g_defp = &g_def;
CDefPointer<CMiscThingsDef> NUISystem::CManager::GetMiscThingsDef() { Log(5); ++g_def.RefCount; return *(CDefPointer<CMiscThingsDef>*)&g_defp; }
static NUISystem::CManager g_mgr;
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return &g_mgr; }
int main()
{
    static NUISystem::CComponent comp; g_top = &comp;
    static unsigned char raw[sizeof(NUISystem::CFrontEndManager)]; NUISystem::CFrontEndManager& m = *(NUISystem::CFrontEndManager*)raw;
    g_def.RefCount = 0;
    m.GotoNewProfileScreen();
    bool ok = g_upm.UseNewProfileValues && m.m_bStartNewGame && m.GoToKeyboard && g_sound == &g_def.NewProfileSound && g_flag
           && g_state == 6 && g_removed == static_cast<NUISystem::CObserver*>(&comp) && (void*)g_removed == (char*)&comp + 4
           && g_n == 8 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 5 && g_log[3] == 4 && g_log[4] == 6
           && g_log[5] == 7 && g_log[6] == 8 && g_log[7] == 3 && g_def.RefCount == 0;
    g_def.RefCount = 1; g_n = 0; m.GotoNewProfileScreen();   // def still referenced elsewhere: no DestroySelf
    ok = ok && g_n == 7 && g_def.RefCount == 1;
    std::printf(ok ? "00595845_TEST PASS\n" : "00595845_TEST FAIL\n");
    return ok ? 0 : 1;
}