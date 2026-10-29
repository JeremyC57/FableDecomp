// NUISystem::CFrontEndManager::GotoNewProfileScreen() -- start creating a new profile: flag the profile manager, refresh the profile names, play the new-profile sound and close the current screen
// Leaf: ?GotoNewProfileScreen@CFrontEndManager@NUISystem@@QAEXXZ
// Retail: Fable.exe @ 0x00595845, 176 bytes, plain `ret`; callers 0x00599D8A, 0x0059A70A.
// Body: CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0) .UseNewProfileValues = true; a local
// vector<CWideString> filled by GetProfileNames (0x005955AB) and destroyed by ~vector 0x00412130; m_bStartNewGame and
// GoToKeyboard = true; CManager::GetInstance() (0x0041E5F2) ->PlaySound(GetInstance()->GetMiscThingsDef()->[+0x1a0], true)
// (CManager slots 40 / 42; the def comes back as a 4-byte intrusive CDefPointer released inline: --RefCount, slot 1 at
// zero); top = m_stackScreens.top() (deque::back 0x0059B039); top->ChangeState(6) (CComponent slot 48, +0xc0);
// GetInstance()->RemoveObserver(top's CObserver base, +4, no null check) (CObservable slot 5, +0x14).
// Names: slot 40 / 42 are among Ego_r.pdb CManager virtuals PlaySound(CCharString&, ...) / PlaySample and GetMiscThingsDef /
// GetIconsDef (both return CDefPointer); the def class and its +0x1a0 sound-name member, slot 48 ChangeState and slot 5
// RemoveObserver are not confirmed.
// Layout: retail CFrontEndManager (Ego_r.pdb - 0x14 past the STLport containers): m_stackScreens +0x20,
// m_bStartNewGame +0x64, PScreenToDeactivate +0x98, PScreenToActivate +0x9c, GoToKeyboard +0xa0. Ego_r.pdb
// CUserProfileManager UseNewProfileValues +0xc.
// Opt: size TU (unaligned entry) with a frame pointer (`leave`). Shape: the CDefPointer dtor needs __forceinline and must
// release through an inline def method (--RefCount; if (RefCount == 0) DestroySelf()): retail keeps the object in ecx and
// &RefCount in eax (`add eax,4; dec [eax]; cmp [eax],0`); the open-coded release reloads Object instead. The component's
// CObserver base is formed from *top (reference conversion), matching retail's `add esi,4` with no null check.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString { public: void* PStringData; };
class CWideString { public: void* PStringData; };
class CUserProfileManager {
public:
    unsigned char _pad00[0x0c];
    bool          UseNewProfileValues;                   // +0x0c
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
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
    CUserProfileManager::Get().UseNewProfileValues = true;
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