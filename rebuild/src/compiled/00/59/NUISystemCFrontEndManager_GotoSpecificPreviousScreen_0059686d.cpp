// NUISystem::CFrontEndManager::GotoSpecificPreviousScreen(CComponent* screen) -- leave the current screen and unwind the screen stack back to a given screen (pushing it when the stack runs out)
// Leaf: ?GotoSpecificPreviousScreen@CFrontEndManager@NUISystem@@QAEXPAVCComponent@2@@Z
// Retail: Fable.exe @ 0x0059686D, 170 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0) then its 0x0040D350 (catalog label
// CUserProfileManager::Load, not verified); if m_stackScreens.size() > 1 (deque iterator difference 0x0059A8F3, out of
// line): leaving = top() (deque::back 0x0059B039); leaving->slot 48 (+0xc0)(6); NUISystem::CManager::GetInstance()
// (0x0041E5F2, the lazily-created UI manager, vtable 0x01230134) slot 5 (+0x14, a CObservable observer-list removal)
// with leaving's CObserver base (+4); pop() (deque::pop_back 0x0059AEE5); pop while the stack is non-empty and its top
// is not `screen`; if it ran empty push(screen) (deque::push_back 0x0059B61C); PScreenToDeactivate = leaving,
// PScreenToActivate = screen.
// Layout: retail CFrontEndManager m_stackScreens +0x20 (STLport deque: start iterator +0, finish iterator +0x10),
// PScreenToDeactivate +0x98 / PScreenToActivate +0x9c (Ego_r.pdb +0x2c / +0xac / +0xb0). CComponent RTTI bases:
// CUIInterface +0, CObserver +4, CActionParam +0x18.
// Opt: size TU (unaligned entry) with a frame pointer.
// Shape: the unwind loop keeps the remaining count in a local re-read after each pop (`count = size()`), and `screen` is
// copied to a local: that gives retail's duplicated size() call, `screen` held in ebx and `this` spilled to [ebp-4]
// (a `while (size() > 0)` condition cross-jumps the two size() calls, 9 bytes shorter).
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
class CUserProfileManager {
public:
    void Load();                                         // 0x0040D350 (catalog name)
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
};
template <class T> class CTBaseSingleton {
public:
    static T* Get();                                     // 0x0040D2A0 for CUserProfileManager
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
class CManager {
public:
    virtual void _m00(); virtual void _m01(); virtual void _m02(); virtual void _m03(); virtual void _m04();
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2
};
class CFrontEndManager {
public:
    unsigned char           _pad00[0x20];
    std::stack<CComponent*> m_stackScreens;              // +0x20
    unsigned char           _pad48[0x98 - 0x48];
    CComponent*             PScreenToDeactivate;         // +0x98
    CComponent*             PScreenToActivate;           // +0x9c
    void GotoSpecificPreviousScreen(CComponent* screen);
};

void CFrontEndManager::GotoSpecificPreviousScreen(CComponent* screen)
{
    CUserProfileManager::Get().Load();
    if (m_stackScreens.size() > 1)
    {
        CComponent* leaving = m_stackScreens.top();
        leaving->OnScreenEvent(6);
        CManager::GetInstance()->RemoveObserver(leaving);
        m_stackScreens.pop();
        unsigned int count = m_stackScreens.size();
        CComponent* target = screen;
        while (count > 0)
        {
            if (m_stackScreens.top() == target)
                break;
            m_stackScreens.pop();
            count = m_stackScreens.size();
        }
        if (m_stackScreens.empty())
            m_stackScreens.push(screen);
        PScreenToDeactivate = leaving;
        PScreenToActivate = target;
    }
}
}