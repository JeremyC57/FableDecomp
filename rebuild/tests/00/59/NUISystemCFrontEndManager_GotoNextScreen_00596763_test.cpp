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

#include <stdio.h>
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
void CSoundObject_013B8394::_s000() {}
void CSoundObject_013B8394::_s001() {}
void CSoundObject_013B8394::_s002() {}
void CSoundObject_013B8394::_s003() {}
void CSoundObject_013B8394::_s004() {}
void CSoundObject_013B8394::_s005() {}
void CSoundObject_013B8394::_s006() {}
void CSoundObject_013B8394::_s007() {}
void CSoundObject_013B8394::_s008() {}
void CSoundObject_013B8394::_s009() {}
void CSoundObject_013B8394::_s010() {}
void CSoundObject_013B8394::_s011() {}
void CSoundObject_013B8394::_s012() {}
void CSoundObject_013B8394::_s013() {}
void CSoundObject_013B8394::_s014() {}
void CSoundObject_013B8394::_s015() {}
void CSoundObject_013B8394::_s016() {}
void CSoundObject_013B8394::_s017() {}
void CSoundObject_013B8394::_s018() {}
void CSoundObject_013B8394::_s019() {}
void CSoundObject_013B8394::_s020() {}
void CSoundObject_013B8394::_s021() {}
void CSoundObject_013B8394::_s022() {}
void CSoundObject_013B8394::_s023() {}
void CSoundObject_013B8394::_s024() {}
void CSoundObject_013B8394::_s025() {}
void CSoundObject_013B8394::_s026() {}
void CSoundObject_013B8394::_s027() {}
void CSoundObject_013B8394::_s028() {}
void CSoundObject_013B8394::_s029() {}
void CSoundObject_013B8394::_s030() {}
void CSoundObject_013B8394::_s031() {}
void CSoundObject_013B8394::_s032() {}
void CSoundObject_013B8394::_s033() {}
void CSoundObject_013B8394::_s034() {}
void CSoundObject_013B8394::_s035() {}
void CSoundObject_013B8394::_s036() {}
void CSoundObject_013B8394::_s037() {}
void CSoundObject_013B8394::_s038() {}
void CSoundObject_013B8394::_s039() {}
void CSoundObject_013B8394::_s040() {}
void CSoundObject_013B8394::_s041() {}
void CSoundObject_013B8394::_s042() {}
void CSoundObject_013B8394::_s043() {}
void CSoundObject_013B8394::_s044() {}
void CSoundObject_013B8394::_s045() {}
void NUISystem::CManager::_m006() {}
void NUISystem::CManager::_m007() {}
void NUISystem::CManager::_m008() {}
void NUISystem::CManager::_m009() {}
void NUISystem::CManager::_m010() {}
void NUISystem::CManager::_m011() {}
void NUISystem::CManager::_m012() {}
void NUISystem::CManager::_m013() {}
void NUISystem::CManager::_m014() {}
void NUISystem::CManager::_m015() {}
void NUISystem::CManager::_m016() {}
void NUISystem::CManager::_m017() {}
void NUISystem::CManager::_m018() {}
void NUISystem::CManager::_m019() {}
void NUISystem::CManager::_m020() {}
void NUISystem::CManager::_m021() {}
void NUISystem::CManager::_m022() {}
void NUISystem::CManager::_m023() {}
void NUISystem::CManager::_m024() {}
void NUISystem::CManager::_m025() {}
void NUISystem::CManager::_m026() {}
void NUISystem::CManager::_m027() {}
void NUISystem::CManager::_m028() {}
void NUISystem::CManager::_m029() {}
void NUISystem::CManager::_m030() {}
void NUISystem::CManager::_m031() {}
void NUISystem::CManager::_m032() {}
void NUISystem::CManager::_m033() {}
void NUISystem::CManager::_m034() {}
void NUISystem::CManager::_m035() {}
void NUISystem::CManager::_m036() {}
void NUISystem::CManager::_m037() {}
void NUISystem::CManager::_m038() {}
void NUISystem::CManager::_m039() {}
void NUISystem::CManager::_m041() {}
void NUISystem::CObserver::_o00() {}
void NUISystem::CUIInterface::_u00() {}
void NUISystem::CObservable::_b00() {} void NUISystem::CObservable::_b01() {} void NUISystem::CObservable::_b02() {}
void NUISystem::CObservable::_b03() {} void NUISystem::CObservable::_b04() {}
static int g_events; static NUISystem::CComponent* g_evOn; static long g_ev;
void NUISystem::CComponent::OnScreenEvent(long e) { ++g_events; g_evOn = this; g_ev = e; }
static NUISystem::CObserver* g_removed;
void NUISystem::CObservable::RemoveObserver(NUISystem::CObserver* o) { g_removed = o; }
static int g_volCalls; static unsigned long g_volCh; static float g_vol = -1, g_volT;
void CSoundObject_013B8394::SetChannelVolume(unsigned long c, float v, float t) { ++g_volCalls; g_volCh = c; g_vol = v; g_volT = t; }
static CSoundObject_013B8394 g_snd;
CSoundObject_013B8394* g_pSound_013B8394;
static int g_destroyed;
void CMiscThingsDef::v00() {}
void CMiscThingsDef::DestroySelf() { ++g_destroyed; }
static CMiscThingsDef g_def;
static CCharString* g_played; static int g_playFlag = -1;
void NUISystem::CManager::PlaySound(CCharString& n, bool f) { g_played = &n; g_playFlag = f ? 1 : 0; }
static CMiscThingsDef* g_defPtr = &g_def;
CDefPointer<CMiscThingsDef> NUISystem::CManager::GetMiscThingsDef() { ++g_def.RefCount; return *(CDefPointer<CMiscThingsDef>*)&g_defPtr; }
static NUISystem::CManager g_mgr;
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return &g_mgr; }
static int g_deact; static NUISystem::CComponent* g_deactArg = (NUISystem::CComponent*)1;
void NUISystem::CFrontEndManager::DeactivateAllEditBoxes(NUISystem::CComponent* s) { ++g_deact; g_deactArg = s; }
typedef NUISystem::CComponent* PC;
static PC g_store[16];
static PC g_map[64];
template <class T> int std::_Deque_iterator<T>::operator-(const std::_Deque_iterator<T>& o) const { return int(_M_cur - o._M_cur); }
template <class T> T& std::deque<T>::back() { return _M_finish._M_cur[-1]; }
template <class T> void std::deque<T>::push_back(const T& x) { *_M_finish._M_cur++ = x; }
template <class T> void std::deque<T>::pop_back() { --_M_finish._M_cur; }
template <class K, class V> V& std::map<K, V>::operator[](const K& k) { return g_map[(int)k]; }
static NUISystem::CFrontEndManager g_fem;
static void Setup(PC* items, int n) { for (int i = 0; i < n; ++i) g_store[i] = items[i]; g_fem.m_stackScreens.c._M_start._M_cur = g_store; g_fem.m_stackScreens.c._M_finish._M_cur = g_store + n; }
static int Depth() { return int(g_fem.m_stackScreens.c._M_finish._M_cur - g_store); }
int main()
{
    static NUISystem::CComponent a, b, c, special;
    g_map[0x1a] = &special;
    g_def.RefCount = 1;
    PC s1[2] = { &a, &b };
    Setup(s1, 2);
    g_fem.GotoNextScreen(&c, false);          // plain push
    bool ok = Depth() == 3 && g_store[2] == &c && g_events == 1 && g_evOn == &b && g_ev == 6 && g_removed == (NUISystem::CObserver*)&b
        && g_deact == 2 && g_deactArg == 0 && g_fem.PScreenToActivate == &c && g_fem.PScreenToDeactivate == &b
        && g_played == &g_def.ScreenChangeSound && g_playFlag == 1 && g_def.RefCount == 1 && g_volCalls == 0;
    Setup(s1, 2);
    g_fem.GotoNextScreen(&special, true);     // clear stack; special screen but no sound object
    ok = ok && Depth() == 1 && g_store[0] == &special && g_volCalls == 0;
    g_pSound_013B8394 = &g_snd;
    Setup(s1, 2);
    g_fem.GotoNextScreen(&special, false);    // special screen with the sound object
    ok = ok && Depth() == 3 && g_volCalls == 1 && g_volCh == 0x100 && g_vol == 0.0f && g_volT == 1000.0f;
    g_def.RefCount = 0;                       // the returned reference is the last one
    Setup(s1, 2);
    g_fem.GotoNextScreen(&c, false);
    ok = ok && g_destroyed == 1;
    printf(ok ? "00596763_TEST PASS\n" : "00596763_TEST FAIL\n");
    return ok ? 0 : 1;
}