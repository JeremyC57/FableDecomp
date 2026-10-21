#include <cstdio>
class CWideString {
public:
    void* PStringData;
    CWideString();                                       // 0x0099AED0
};
class CCharString { public: void* PStringData; };
class CDateAndTime;
class CVirtualKeyboard;
class CNewFrontendGameComponent;
enum FRONTEND_SCREEN { FRONTEND_SCREEN_NONE };
namespace std {
template <class T> class allocator {
public:
    allocator() {}
};
template <class T, class A = allocator<T> > class list {
public:
    void* _M_node;
    explicit list(const A& a = A());                     // 0x0042AB01 for NUISystem::CActionParam*
};
template <class T> class deque {
public:
    unsigned char _body[0x28];
    deque();                                             // 0x0059B310 for NUISystem::CComponent*
};
template <class T, class C = deque<T> > class stack {
public:
    C c;
};
template <class T, class A> struct _STLP_alloc_proxy {
    T _M_data;
    _STLP_alloc_proxy(const A&, T p) : _M_data(p) {}
};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;
    explicit vector(const A& a = A()) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) {}
};
template <class K, class V> class map {
public:
    void* _body[3];
    map();                                               // 0x0059B32A (FRONTEND_SCREEN) / 0x00486055 (CWideString)
};
}
namespace NUISystem {
class CActionParam;
class CComponent;
class CObserver {
public:
    unsigned char _body[0x10];
    CObserver();                                         // 0x0052D9E0
    virtual ~CObserver();
};
class CFrontEndManager : public CObserver {
public:
    CWideString                                 MediaPlayerErrorString;          // +0x14
    std::list<CActionParam*>                    UIActionParams;                  // +0x18
    CNewFrontendGameComponent*                  PGameComponent;                  // +0x1c
    std::stack<CComponent*>                     m_stackScreens;                  // +0x20
    std::vector<CComponent*>                    m_vComponents;                   // +0x48
    std::map<FRONTEND_SCREEN, CComponent*>      m_mapScreens;                    // +0x54
    CVirtualKeyboard*                           m_pVirtualKeyboard;              // +0x60
    bool                                        m_bStartNewGame;                 // +0x64
    std::vector<CDateAndTime>                   m_vFileTimes;                    // +0x68
    std::vector<CCharString>                    FileRegionNames;                 // +0x74
    std::vector<CCharString>                    FileRegionMiniMapNames;          // +0x80
    std::vector<long>                           FilePlayTimes;                   // +0x8c
    CComponent*                                 PScreenToDeactivate;             // +0x98
    CComponent*                                 PScreenToActivate;               // +0x9c
    bool                                        GoToKeyboard;                    // +0xa0
    CComponent*                                 PScreenToDeactivateForStartGame; // +0xa4
    CComponent*                                 PScreenToDeactivateForLoadGame;  // +0xa8
    CWideString                                 GameToLoad;                      // +0xac
    CWideString                                 ProfileToDelete;                 // +0xb0
    unsigned long                               BlocksOnUNeeded;                 // +0xb4
    unsigned long                               BlocksOnTNeeded;                 // +0xb8
    unsigned long                               NumberSaveGames;                 // +0xbc
    bool                                        m_bQuitFromGame;                 // +0xc0
    std::map<CWideString, bool>                 Signatures;                      // +0xc4
    double                                      InstallationPauseStartTime;      // +0xd0
    bool                                        InstallationPaused;              // +0xd8
    bool                                        FinalInstallationNeverPause;     // +0xd9
    CFrontEndManager();
    virtual ~CFrontEndManager();
};
CFrontEndManager::CFrontEndManager()
    : m_pVirtualKeyboard(0), m_bStartNewGame(false), PScreenToDeactivate(0), PScreenToActivate(0), GoToKeyboard(false),
      PScreenToDeactivateForStartGame(0), PScreenToDeactivateForLoadGame(0), m_bQuitFromGame(false),
      InstallationPaused(false), FinalInstallationNeverPause(false)
{
}
}

#include <new>
static int g_order[16], g_n; static void Log(int x) { g_order[g_n++] = x; }
CWideString::CWideString() { PStringData = (void*)0x77; Log(2); }
template <class T, class A> std::list<T, A>::list(const A&) { _M_node = (void*)0x55; Log(3); }
template <class T> std::deque<T>::deque() { _body[0] = 0x44; Log(4); }
static int g_maps;
template <class K, class V> std::map<K, V>::map() { _body[0] = (void*)(long)(++g_maps); Log(5); }
NUISystem::CObserver::CObserver() { _body[0] = 0x11; Log(1); }
NUISystem::CObserver::~CObserver() {}
NUISystem::CFrontEndManager::~CFrontEndManager() {}
int main()
{
    static unsigned char raw[sizeof(NUISystem::CFrontEndManager)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xcd;
    NUISystem::CFrontEndManager* m = new (raw) NUISystem::CFrontEndManager;
    bool ok = m->MediaPlayerErrorString.PStringData == (void*)0x77 && m->UIActionParams._M_node == (void*)0x55
           && m->m_stackScreens.c._body[0] == 0x44 && m->m_vComponents._M_start == 0 && m->m_vComponents._M_end_of_storage._M_data == 0
           && m->m_mapScreens._body[0] == (void*)1 && m->Signatures._body[0] == (void*)2 && m->m_pVirtualKeyboard == 0
           && !m->m_bStartNewGame && m->FilePlayTimes._M_finish == 0 && m->FileRegionMiniMapNames._M_end_of_storage._M_data == 0
           && m->PScreenToDeactivate == 0 && m->PScreenToActivate == 0 && !m->GoToKeyboard && m->PScreenToDeactivateForLoadGame == 0
           && m->GameToLoad.PStringData == (void*)0x77 && m->ProfileToDelete.PStringData == (void*)0x77 && !m->m_bQuitFromGame
           && !m->InstallationPaused && !m->FinalInstallationNeverPause && m->BlocksOnUNeeded == 0xcdcdcdcd
           && (char*)&m->Signatures - (char*)m == 0xc4 && (char*)&m->InstallationPaused - (char*)m == 0xd8
           && g_n == 8 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3 && g_order[3] == 4 && g_order[4] == 5
           && g_order[5] == 2 && g_order[6] == 2 && g_order[7] == 5;
    std::printf(ok ? "005953e2_TEST PASS\n" : "005953e2_TEST FAIL\n");
    return ok ? 0 : 1;
}