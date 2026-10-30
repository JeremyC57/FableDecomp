// NUISystem::CFrontEndManager::GotoPreviousScreen(bool force) -- back out of the current front-end screen
// Grade: DIFFER(517v517) -- with the same-TU map::operator[] stand-in the map-result spills now match retail ([ebp+8]); left: (1) retail derefs deque::back() at once and stores the key 0x14 after the back() call, here the reference is kept and the key store is scheduled first (GetActiveScreen() by value fixes the deref but swaps esi/edi = this/&m_mapScreens everywhere; operand order `top() == map[]` evaluates the map first, 516v517); (2) tail PlaySound push scheduling (`add eax,19Ch ; push 1 ; mov ecx,edi ; push eax`). 4 grades.
// Retail 0x00597BF2, 517 bytes, `ret 4` (Ego_r.pdb ?GotoPreviousScreen@CFrontEndManager@NUISystem@@QAEX_N@Z).
// Body: on the main menu (m_mapScreens[0x14]) only a forced call does anything: it re-enters screen 0x1A. Otherwise the
// profile manager reloads (CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0 -> Load 0x0040D350), the front end's
// attract-mode timer restarts (g_pNewFrontendGameComponent 0x013B871C -> ResetAttractModeTimer 0x0042DBEE) and the
// virtual keyboard is dropped (DestroyVirtualKeyboard(false) 0x00594FA9). With more than one screen stacked: from
// screen 0x1B go back to the profile menu (7) through GotoSpecificPreviousScreen 0x0059686D when it is on the stack
// (IsScreenOnStack 0x005959AB), else forward to the main menu clearing the stack; from any other screen pop it
// (slot 48 event 6, CManager observer removal, deque pop), publish the new top / the leaving screen and, when the new
// top is the virtual-keyboard screen (0x17), flag the profile manager (UseNewProfileValues) and build a
// CVirtualKeyboard (operator new 0x00BFEA1A, ctor 0x00851700, Initialize 0x00851770) into +0x60. A single screen
// with `force` re-enters 0x1A. Every path but the unforced main-menu one ends with the UI back sound (misc def
// +0x19C through CManager slots 42 / 40).
// Callees as the landed CFrontEndManager rows (map::operator[] 0x0059B5D7, deque back 0x0059B039 / pop_back 0x0059AEE5,
// iterator difference 0x0059A8F3, GotoNextScreen 0x00596763, CManager::GetInstance 0x0041E5F2).
// Opt: size TU with a frame pointer (`optimize("s")` + `("y",off)`), as the landed GotoNextScreen / GotoProfileMenu.
// Leaf: ?GotoPreviousScreen@CFrontEndManager@NUISystem@@QAEX_N@Z
#pragma optimize("s", on)
#pragma optimize("y", off)
namespace std {
template <class T> struct _Deque_iterator {
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
    int operator-(const _Deque_iterator& other) const;   // 0x0059A8F3 for CComponent*
};
template <class T> class deque {
public:
    _Deque_iterator<T> _M_start;                         // +0x00
    _Deque_iterator<T> _M_finish;                        // +0x10
    T**                _M_map;
    unsigned int       _M_map_size;
    unsigned int size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_finish._M_cur == _M_start._M_cur; }
    T& back();                                           // 0x0059B039
    void push_back(const T& x);                          // 0x0059B61C
    void pop_back();                                     // 0x0059AEE5
};
template <class K, class V> class map {
public:
    void* _body[3];
    V& operator[](const K& key);                         // 0x0059B5D7 for <FRONTEND_SCREEN, CComponent*>
};
template <class T, class C = deque<T> > class stack {
public:
    C c;
    unsigned int size() const { return c.size(); }
    bool empty() const { return c.empty(); }
    T& top() { return c.back(); }
    void push(const T& x) { c.push_back(x); }
    void pop() { c.pop_back(); }
};
}
enum FRONTEND_SCREEN {
    FRONTEND_SCREEN_PROFILE_MENU = 7, FRONTEND_SCREEN_MAIN_MENU = 0x14, FRONTEND_SCREEN_VIRTUAL_KEYBOARD = 0x17,
    FRONTEND_SCREEN_0x1A = 0x1a, FRONTEND_SCREEN_0x1B = 0x1b
};
class CCharString { public: void* PStringData; };
struct CMiscThingsDef {
    virtual void v00();
    virtual void DestroySelf();                          // slot 1 (+4)
    long          RefCount;                              // +0x04
    void DecRef() { --RefCount; if (RefCount == 0) DestroySelf(); }
    unsigned char _pad08[0x19c - 8];
    CCharString   ScreenBackSound;                       // +0x19c (name not recovered; the +0x1a0 sibling is the forward sound)
    CCharString   ScreenChangeSound;                     // +0x1a0 (name not recovered)
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
    virtual void _o00();
};
class CUIInterface {
public:
    virtual void _u00();
};
class CComponent : public CUIInterface, public CObserver {
public:
    virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005(); virtual void _v006();
    virtual void _v007(); virtual void _v008(); virtual void _v009(); virtual void _v010(); virtual void _v011(); virtual void _v012();
    virtual void _v013(); virtual void _v014(); virtual void _v015(); virtual void _v016(); virtual void _v017(); virtual void _v018();
    virtual void _v019(); virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023(); virtual void _v024();
    virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030();
    virtual void _v031(); virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035(); virtual void _v036();
    virtual void _v037(); virtual void _v038(); virtual void _v039(); virtual void _v040(); virtual void _v041(); virtual void _v042();
    virtual void _v043(); virtual void _v044(); virtual void _v045(); virtual void _v046(); virtual void _v047();
    virtual void OnScreenEvent(long event);              // slot 48 (+0xc0, name not recovered; CComponent's own is empty)
};
class CObservable {
public:
    virtual void _b00(); virtual void _b01(); virtual void _b02(); virtual void _b03(); virtual void _b04();
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
};
class CManager : public CObservable {
public:
    virtual void _m006(); virtual void _m007(); virtual void _m008(); virtual void _m009(); virtual void _m010(); virtual void _m011(); virtual void _m012(); virtual void _m013();
    virtual void _m014(); virtual void _m015(); virtual void _m016(); virtual void _m017(); virtual void _m018(); virtual void _m019(); virtual void _m020(); virtual void _m021();
    virtual void _m022(); virtual void _m023(); virtual void _m024(); virtual void _m025(); virtual void _m026(); virtual void _m027(); virtual void _m028(); virtual void _m029();
    virtual void _m030(); virtual void _m031(); virtual void _m032(); virtual void _m033(); virtual void _m034(); virtual void _m035(); virtual void _m036(); virtual void _m037();
    virtual void _m038(); virtual void _m039();
    virtual void PlaySound(CCharString& name, bool flag);           // slot 40 (+0xa0)
    virtual void _m041();
    virtual CDefPointer<CMiscThingsDef> GetMiscThingsDef();         // slot 42 (+0xa8)
    static CManager* __fastcall GetInstance();           // 0x0041E5F2
};
}   // namespace NUISystem (model)

class CUserProfileManager {
public:
    unsigned char _pad00[0x0c];
    bool UseNewProfileValues;                            // +0x0C
    void Load();                                         // 0x0040D350
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
class CNewFrontendGameComponent {
public:
    void ResetAttractModeTimer();                        // 0x0042DBEE
};
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C
class CVirtualKeyboard {
public:
    unsigned char _body[0x10];
    CVirtualKeyboard(NUISystem::CComponent* entryScreen);   // 0x00851700
    void Initialize();                                   // 0x00851770
};
void* __cdecl operator new(unsigned int size);           // 0x00BFEA1A
// Same-TU stand-in for map<FRONTEND_SCREEN, CComponent*>::operator[] 0x0059B5D7 (the landed 0x00597006 model): it only
// reads the key, so VC7.1 may home the key temporaries in dead frame words.
NUISystem::CComponent** ScreenSlot_0059B5D7(void* map, long key);
#pragma auto_inline(off)
template <> __declspec(noinline) NUISystem::CComponent*& std::map<FRONTEND_SCREEN, NUISystem::CComponent*>::operator[](const FRONTEND_SCREEN& key)
{
    return *ScreenSlot_0059B5D7(this, key);
}
#pragma auto_inline(on)
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char                          _pad00[0x20];
    std::stack<CComponent*>                m_stackScreens;   // +0x20
    unsigned char                          _pad48[0x54 - 0x48];
    std::map<FRONTEND_SCREEN, CComponent*> m_mapScreens;     // +0x54
    CVirtualKeyboard*                      m_pVirtualKeyboard;   // +0x60
    unsigned char                          _pad64[0x98 - 0x64];
    CComponent*                            PScreenToDeactivate;   // +0x98
    CComponent*                            PScreenToActivate;     // +0x9c
    void DestroyVirtualKeyboard(bool goBack);             // 0x00594FA9
    bool IsScreenOnStack(CComponent* screen) const;       // 0x005959AB
    void GotoSpecificPreviousScreen(CComponent* screen);  // 0x0059686D
    void GotoNextScreen(CComponent* screen, bool clearStack);   // 0x00596763
    void GotoPreviousScreen(bool force);
};

void CFrontEndManager::GotoPreviousScreen(bool force)
{
    if (m_mapScreens[FRONTEND_SCREEN_MAIN_MENU] == m_stackScreens.top())
    {
        if (!force)
            return;
        GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_0x1A], false);
    }
    else
    {
        CTBaseSingleton<CUserProfileManager>::Get().Load();
        g_pNewFrontendGameComponent->ResetAttractModeTimer();
        DestroyVirtualKeyboard(false);
        if (m_stackScreens.size() > 1)
        {
            if (m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_0x1B])
            {
                if (IsScreenOnStack(m_mapScreens[FRONTEND_SCREEN_PROFILE_MENU]))
                    GotoSpecificPreviousScreen(m_mapScreens[FRONTEND_SCREEN_PROFILE_MENU]);
                else
                    GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_MAIN_MENU], true);
            }
            else
            {
                CComponent* leaving = m_stackScreens.top();
                leaving->OnScreenEvent(6);
                CManager::GetInstance()->RemoveObserver(&static_cast<CObserver&>(*leaving));
                m_stackScreens.pop();
                PScreenToActivate = m_stackScreens.top();
                PScreenToDeactivate = leaving;
                if (m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_VIRTUAL_KEYBOARD])
                {
                    CTBaseSingleton<CUserProfileManager>::Get().UseNewProfileValues = true;
                    m_pVirtualKeyboard = new CVirtualKeyboard(m_mapScreens[FRONTEND_SCREEN_VIRTUAL_KEYBOARD]);
                    m_pVirtualKeyboard->Initialize();
                }
            }
        }
        else if (force)
        {
            GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_0x1A], false);
        }
    }
    CManager::GetInstance()->PlaySound(CManager::GetInstance()->GetMiscThingsDef()->ScreenBackSound, true);
}
}
