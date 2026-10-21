// NUISystem::CFrontEndManager::GotoNextScreen(CComponent*, bool) -- push a new screen: close the current one, optionally clear the stack, push the new one, play the screen-change sound
// Leaf: ?GotoNextScreen@CFrontEndManager@NUISystem@@QAEXPAVCComponent@2@_N@Z
// Retail: Fable.exe @ 0x00596763, 266 bytes, `ret 8`.
// Body: leaving = m_stackScreens.top() (deque::back 0x0059B039); DeactivateAllEditBoxes(0) (0x005952D8); if screen is
// m_mapScreens[0x1a] (map::operator[] 0x0059B5D7) and the 0x013B8394 sound object exists: its slot 46 (+0xb8)
// (0x100, 0.0f, 1000.0f) (float 0x01252370); leaving->OnScreenEvent(6) (CComponent slot 48, +0xc0);
// CManager::GetInstance() (0x0041E5F2)->RemoveObserver(leaving's CObserver base, +4) (slot 5); if clearStack: while
// m_stackScreens.size() (deque iterator difference 0x0059A8F3) pop() (0x0059AEE5); push(screen) (0x0059B61C);
// DeactivateAllEditBoxes(0); PScreenToActivate = screen; PScreenToDeactivate = leaving;
// GetInstance()->PlaySound(GetInstance()->GetMiscThingsDef()->ScreenChangeSound (+0x1a0), true) (CManager slots 40 / 42;
// the CDefPointer comes back in the dead `clearStack` slot and is released inline: --RefCount, slot 1 at zero).
// Names: DeactivateAllEditBoxes is the Ego_r.pdb CFrontEndManager (CComponent*) member that defaults to the top screen
// (0x005952D8 loads top() when passed 0); the 0x013B8394 object's class and slot 46 (a channel-mask / volume / time call,
// also made by the profile manager with masks 0x10..0x80) are not confirmed; CManager slots as the matched 0x00595845.
// Layout: retail CFrontEndManager m_stackScreens +0x20, m_mapScreens +0x54, PScreenToDeactivate +0x98, PScreenToActivate +0x9c.
// Opt: size TU (unaligned entry) with a frame pointer (`leave`).
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
enum FRONTEND_SCREEN { FRONTEND_SCREEN_0x1A = 0x1a };
class CCharString { public: void* PStringData; };
struct CMiscThingsDef {
    virtual void v00();
    virtual void DestroySelf();                          // slot 1 (+4)
    long          RefCount;                              // +0x04
    void DecRef() { --RefCount; if (RefCount == 0) DestroySelf(); }
    unsigned char _pad08[0x1a0 - 8];
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
class CSoundObject_013B8394 {                            // class not recovered
public:
    virtual void _s000(); virtual void _s001(); virtual void _s002(); virtual void _s003(); virtual void _s004(); virtual void _s005(); virtual void _s006(); virtual void _s007();
    virtual void _s008(); virtual void _s009(); virtual void _s010(); virtual void _s011(); virtual void _s012(); virtual void _s013(); virtual void _s014(); virtual void _s015();
    virtual void _s016(); virtual void _s017(); virtual void _s018(); virtual void _s019(); virtual void _s020(); virtual void _s021(); virtual void _s022(); virtual void _s023();
    virtual void _s024(); virtual void _s025(); virtual void _s026(); virtual void _s027(); virtual void _s028(); virtual void _s029(); virtual void _s030(); virtual void _s031();
    virtual void _s032(); virtual void _s033(); virtual void _s034(); virtual void _s035(); virtual void _s036(); virtual void _s037(); virtual void _s038(); virtual void _s039();
    virtual void _s040(); virtual void _s041(); virtual void _s042(); virtual void _s043(); virtual void _s044(); virtual void _s045();
    virtual void SetChannelVolume(unsigned long channels, float volume, float time);   // slot 46 (+0xb8)
};
extern CSoundObject_013B8394* g_pSound_013B8394;         // 0x013B8394
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
class CFrontEndManager {
public:
    unsigned char                          _pad00[0x20];
    std::stack<CComponent*>                m_stackScreens;   // +0x20
    unsigned char                          _pad48[0x54 - 0x48];
    std::map<FRONTEND_SCREEN, CComponent*> m_mapScreens;     // +0x54
    unsigned char                          _pad60[0x98 - 0x60];
    CComponent*                            PScreenToDeactivate;   // +0x98
    CComponent*                            PScreenToActivate;     // +0x9c
    void DeactivateAllEditBoxes(CComponent* screen);         // 0x005952D8
    void GotoNextScreen(CComponent* screen, bool clearStack);
};
void CFrontEndManager::GotoNextScreen(CComponent* screen, bool clearStack)
{
    CComponent* leaving = m_stackScreens.top();
    DeactivateAllEditBoxes(0);
    if (screen == m_mapScreens[FRONTEND_SCREEN_0x1A] && g_pSound_013B8394)
        g_pSound_013B8394->SetChannelVolume(0x100, 0.0f, 1000.0f);
    leaving->OnScreenEvent(6);
    CManager::GetInstance()->RemoveObserver(&static_cast<CObserver&>(*leaving));
    if (clearStack)
        while (m_stackScreens.size())
            m_stackScreens.pop();
    m_stackScreens.push(screen);
    DeactivateAllEditBoxes(0);
    PScreenToActivate = screen;
    PScreenToDeactivate = leaving;
    CManager::GetInstance()->PlaySound(CManager::GetInstance()->GetMiscThingsDef()->ScreenChangeSound, true);
}
}