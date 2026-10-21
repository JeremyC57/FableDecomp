// NUISystem::CFrontEndManager::CFrontEndManager() -- construct the front-end manager: observer base, containers, and cleared screen / profile / installation state
// Leaf: ??0CFrontEndManager@NUISystem@@QAE@XZ
// Retail: Fable.exe @ 0x005953E2, 203 bytes, returns this.
// Body: CObserver() base (0x0052D9E0, 0x14 bytes); vptr 0x012521A8 (RTTI CFrontEndManager : CObserver); member ctors in
// declaration order: MediaPlayerErrorString (CWideString() 0x0099AED0), UIActionParams (std::list ctor 0x0042AB01 with a
// default allocator temporary), m_stackScreens (deque ctor 0x0059B310), m_vComponents (inline zero), m_mapScreens (map ctor
// 0x0059B32A), m_pVirtualKeyboard = 0, m_bStartNewGame = false, the four file vectors (inline zero), the screen pointers
// and GoToKeyboard cleared, GameToLoad / ProfileToDelete (CWideString()), m_bQuitFromGame = false, Signatures (map ctor
// 0x00486055), InstallationPaused = FinalInstallationNeverPause = false. PGameComponent, the three block / save counts and
// InstallationPauseStartTime are left uninitialised, as retail.
// Layout: Ego_r.pdb CFrontEndManager member order with STLport sizes (list 4, deque 0x28, vector 0xc, map 0xc): the matched
// rows' m_stackScreens +0x20, m_bStartNewGame +0x64, PScreenToDeactivate +0x98, GoToKeyboard +0xa0, Signatures +0xc4.
// Opt: size TU (unaligned entry); the two upper vectors are zeroed through `lea eax` by the size optimiser. Shape: the
// vector ctor is STLport's (allocator argument, _M_end_of_storage an _STLP_alloc_proxy built by its own ctor): with a plain
// three-member init list the next lea for m_mapScreens is scheduled one store earlier.
#pragma optimize("s", on)
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