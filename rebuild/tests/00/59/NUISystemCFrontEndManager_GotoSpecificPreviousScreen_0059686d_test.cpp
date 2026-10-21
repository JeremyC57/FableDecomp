#include <cstdio>
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
    static CManager* GetInstance();                      // 0x0041E5F2
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
    CTBaseSingleton<CUserProfileManager>::Get()->Load();
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

void NUISystem::CComponent::_v001() {}
void NUISystem::CComponent::_v002() {}
void NUISystem::CComponent::_v003() {}
void NUISystem::CComponent::_v004() {}
void NUISystem::CComponent::_v005() {}
void NUISystem::CComponent::_v006() {}
void NUISystem::CComponent::_v007() {}
void NUISystem::CComponent::_v008() {}
void NUISystem::CComponent::_v009() {}
void NUISystem::CComponent::_v010() {}
void NUISystem::CComponent::_v011() {}
void NUISystem::CComponent::_v012() {}
void NUISystem::CComponent::_v013() {}
void NUISystem::CComponent::_v014() {}
void NUISystem::CComponent::_v015() {}
void NUISystem::CComponent::_v016() {}
void NUISystem::CComponent::_v017() {}
void NUISystem::CComponent::_v018() {}
void NUISystem::CComponent::_v019() {}
void NUISystem::CComponent::_v020() {}
void NUISystem::CComponent::_v021() {}
void NUISystem::CComponent::_v022() {}
void NUISystem::CComponent::_v023() {}
void NUISystem::CComponent::_v024() {}
void NUISystem::CComponent::_v025() {}
void NUISystem::CComponent::_v026() {}
void NUISystem::CComponent::_v027() {}
void NUISystem::CComponent::_v028() {}
void NUISystem::CComponent::_v029() {}
void NUISystem::CComponent::_v030() {}
void NUISystem::CComponent::_v031() {}
void NUISystem::CComponent::_v032() {}
void NUISystem::CComponent::_v033() {}
void NUISystem::CComponent::_v034() {}
void NUISystem::CComponent::_v035() {}
void NUISystem::CComponent::_v036() {}
void NUISystem::CComponent::_v037() {}
void NUISystem::CComponent::_v038() {}
void NUISystem::CComponent::_v039() {}
void NUISystem::CComponent::_v040() {}
void NUISystem::CComponent::_v041() {}
void NUISystem::CComponent::_v042() {}
void NUISystem::CComponent::_v043() {}
void NUISystem::CComponent::_v044() {}
void NUISystem::CComponent::_v045() {}
void NUISystem::CComponent::_v046() {}
void NUISystem::CComponent::_v047() {}
void NUISystem::CObserver::_o00() {}
void NUISystem::CUIInterface::_u00() {}
static int g_events[8]; static NUISystem::CComponent* g_evOn[8]; static int g_nev;
void NUISystem::CComponent::OnScreenEvent(long e) { g_evOn[g_nev] = this; g_events[g_nev++] = (int)e; }
void NUISystem::CManager::_m00() {} void NUISystem::CManager::_m01() {} void NUISystem::CManager::_m02() {}
void NUISystem::CManager::_m03() {} void NUISystem::CManager::_m04() {}
static NUISystem::CObserver* g_removed; static int g_loads;
void NUISystem::CManager::RemoveObserver(NUISystem::CObserver* o) { g_removed = o; }
static NUISystem::CManager g_mgr;
NUISystem::CManager* NUISystem::CManager::GetInstance() { return &g_mgr; }
static CUserProfileManager g_upm;
template <class T> T* CTBaseSingleton<T>::Get() { return &g_upm; }
void CUserProfileManager::Load() { ++g_loads; }
typedef NUISystem::CComponent* PC;
static PC g_store[16];
template <class T> int std::_Deque_iterator<T>::operator-(const std::_Deque_iterator<T>& o) const { return int(_M_cur - o._M_cur); }
template <class T> T& std::deque<T>::back() { return _M_finish._M_cur[-1]; }
template <class T> void std::deque<T>::push_back(const T& x) { *_M_finish._M_cur++ = x; }
template <class T> void std::deque<T>::pop_back() { --_M_finish._M_cur; }
static NUISystem::CFrontEndManager g_fem;
static void Setup(PC* items, int n) { for (int i = 0; i < n; ++i) g_store[i] = items[i]; g_fem.m_stackScreens.c._M_start._M_cur = g_store; g_fem.m_stackScreens.c._M_finish._M_cur = g_store + n; }
static int Depth() { return int(g_fem.m_stackScreens.c._M_finish._M_cur - g_store); }
int main()
{
    static NUISystem::CComponent a, b, c, d, x;
    PC s1[4] = { &a, &b, &c, &d };
    Setup(s1, 4); g_fem.PScreenToDeactivate = 0; g_fem.PScreenToActivate = 0;
    g_fem.GotoSpecificPreviousScreen(&b);                 // unwind to b
    bool ok = g_loads == 1 && Depth() == 2 && g_store[1] == &b && g_nev == 1 && g_evOn[0] == &d && g_events[0] == 6
        && g_removed == (NUISystem::CObserver*)&d && g_fem.PScreenToDeactivate == &d && g_fem.PScreenToActivate == &b;
    PC s2[3] = { &a, &b, &c };
    Setup(s2, 3); g_fem.GotoSpecificPreviousScreen(&x);   // not on the stack: runs empty, pushes x
    ok = ok && g_loads == 2 && Depth() == 1 && g_store[0] == &x && g_fem.PScreenToDeactivate == &c && g_fem.PScreenToActivate == &x;
    PC s3[1] = { &a };
    Setup(s3, 1); g_fem.PScreenToDeactivate = 0; g_fem.GotoSpecificPreviousScreen(&a);   // one screen: nothing happens
    ok = ok && g_loads == 3 && Depth() == 1 && g_fem.PScreenToDeactivate == 0 && g_nev == 2;
    std::printf(ok ? "0059686d_TEST PASS\n" : "0059686d_TEST FAIL\n");
    return ok ? 0 : 1;
}