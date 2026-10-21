#include <cstdio>
namespace NUISystem {
class CUIInterface {
public:
    virtual ~CUIInterface();
};
class CObserver {
public:
    virtual ~CObserver();
    virtual void _slot01(); virtual void _slot02(); virtual void _slot03(); virtual void _slot04(); virtual void _slot05();
    virtual void ObserveAllEvents();                     // slot 6 (+0x18)
};
class CComponent : public CUIInterface, public CObserver {
public:
    long _body[4];
};
class CObservable {
public:
    virtual ~CObservable();
    virtual void _slot01();
    virtual void AddObserver(CObserver* observer);       // slot 2 (+8)
};
class CManager : public CObservable {
public:
    static CManager* __fastcall GetInstance();           // 0x0041E5F2
};
}
namespace std {
template <class T> class deque {
public:
    unsigned char _body[0x28];
    void push_back(const T& x);                          // 0x0059B61C for NUISystem::CComponent*
};
template <class T, class Seq = deque<T> > class stack {
public:
    Seq c;
    void push(const T& x) { c.push_back(x); }
};
}
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char           _pad00[0x20];
    std::stack<CComponent*> m_stackScreens;              // +0x20
    void InitScreenStack(CComponent* screen);
};
void CFrontEndManager::InitScreenStack(CComponent* screen)
{
    CManager* manager = CManager::GetInstance();
    CComponent* component = screen;
    manager->AddObserver(component);
    component->ObserveAllEvents();
    m_stackScreens.push(screen);
}
}

static int g_step = 0, g_ok = 1;
static NUISystem::CComponent* g_expect = 0;
namespace NUISystem {
CUIInterface::~CUIInterface() {}
CObserver::~CObserver() {}
void CObserver::_slot01() {} void CObserver::_slot02() {} void CObserver::_slot03() {} void CObserver::_slot04() {} void CObserver::_slot05() {}
void CObserver::ObserveAllEvents() { if (g_step != 1 || this != static_cast<CObserver*>(g_expect)) g_ok = 0; g_step = 2; }
CObservable::~CObservable() {}
void CObservable::_slot01() {}
void CObservable::AddObserver(CObserver* o) { if (g_step != 0 || o != static_cast<CObserver*>(g_expect)) g_ok = 0; g_step = 1; }
static CManager g_mgr;
CManager* __fastcall CManager::GetInstance() { return &g_mgr; }
}
static NUISystem::CComponent* g_pushed = 0;
namespace std { template <> void deque<NUISystem::CComponent*>::push_back(NUISystem::CComponent* const& x) { if (g_step != 2) g_ok = 0; g_pushed = x; g_step = 3; } }
int main()
{
    NUISystem::CComponent c;
    NUISystem::CFrontEndManager m;
    g_expect = &c;
    m.InitScreenStack(&c);
    if (g_ok && g_step == 3 && g_pushed == &c) std::printf("0059672a_TEST PASS\n");
    return 0;
}