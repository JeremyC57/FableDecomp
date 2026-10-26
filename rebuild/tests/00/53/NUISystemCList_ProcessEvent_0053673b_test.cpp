#include <cstdio>
namespace std {
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> class list {
public:
    _List_node<T>* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
}
namespace NUISystem {
enum EEventType { EVENT_UP = 0, EVENT_DOWN = 1, EVENT_25 = 0x19 };
class CUIDef { public:
    virtual void v00();
    virtual void DestroySelf() const;                    // slot 1 (+4)
    mutable long  RefCount;                              // +0x04
    unsigned char _pad08[0x20a - 8];
    bool          Flag20A;                               // +0x20a (name not recovered)
    void DecRef() const { --RefCount; if (RefCount == 0) DestroySelf(); }
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
    __forceinline ~CDefPointer() { if (Object) Object->DecRef(); }
};
class CComponent;
extern std::list<CComponent*> g_ComponentList;          // 0x013B8A98 (name not recovered)
class CManager {
public:
    virtual void _m000(); virtual void _m001(); virtual void _m002(); virtual void _m003(); virtual void _m004(); virtual void _m005(); virtual void _m006(); virtual void _m007(); virtual void _m008(); virtual void _m009(); virtual void _m010();
    virtual bool IsFlagC9() const;                       // slot 11 (+0x2c, 0x0042D4F0)
    unsigned char _pad04[0xa4 - 4];
    bool          FlagA4;                                // +0xa4
    static CManager* __fastcall GetInstance();           // 0x0041E5F2
};
class CUIInterface {
public:
    virtual ~CUIInterface();
    virtual void _c001(); virtual void _c002(); virtual void _c003(); virtual void _c004(); virtual void _c005(); virtual void _c006(); virtual void _c007(); virtual void _c008(); virtual void _c009(); virtual void _c010(); virtual void _c011(); virtual void _c012(); virtual void _c013(); virtual void _c014(); virtual void _c015(); virtual void _c016(); virtual void _c017(); virtual void _c018(); virtual void _c019(); virtual void _c020(); virtual void _c021(); virtual void _c022(); virtual void _c023(); virtual void _c024(); virtual void _c025(); virtual void _c026(); virtual void _c027(); virtual void _c028(); virtual void _c029(); virtual void _c030(); virtual void _c031(); virtual void _c032(); virtual void _c033(); virtual void _c034(); virtual void _c035(); virtual void _c036(); virtual void _c037(); virtual void _c038(); virtual void _c039(); virtual void _c040(); virtual void _c041(); virtual void _c042(); virtual void _c043(); virtual void _c044(); virtual void _c045(); virtual void _c046(); virtual void _c047(); virtual void _c048(); virtual void _c049(); virtual void _c050(); virtual void _c051(); virtual void _c052(); virtual void _c053(); virtual void _c054(); virtual void _c055(); virtual void _c056(); virtual void _c057(); virtual void _c058(); virtual void _c059(); virtual void _c060(); virtual void _c061(); virtual void _c062(); virtual void _c063(); virtual void _c064(); virtual void _c065(); virtual void _c066(); virtual void _c067(); virtual void _c068(); virtual void _c069(); virtual void _c070(); virtual void _c071(); virtual void _c072(); virtual void _c073(); virtual void _c074(); virtual void _c075(); virtual void _c076(); virtual void _c077(); virtual void _c078(); virtual void _c079(); virtual void _c080(); virtual void _c081(); virtual void _c082(); virtual void _c083(); virtual void _c084(); virtual void _c085(); virtual void _c086(); virtual void _c087(); virtual void _c088(); virtual void _c089(); virtual void _c090(); virtual void _c091(); virtual void _c092(); virtual void _c093(); virtual void _c094(); virtual void _c095(); virtual void _c096(); virtual void _c097(); virtual void _c098(); virtual void _c099(); virtual void _c100(); virtual void _c101(); virtual void _c102(); virtual void _c103(); virtual void _c104(); virtual void _c105(); virtual void _c106(); virtual void _c107();
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const;    // slot 108 (+0x1b0)
    virtual void _l109(); virtual void _l110(); virtual void _l111(); virtual void _l112(); virtual void _l113(); virtual void _l114(); virtual void _l115(); virtual void _l116(); virtual void _l117(); virtual void _l118(); virtual void _l119(); virtual void _l120(); virtual void _l121(); virtual void _l122(); virtual void _l123(); virtual void _l124(); virtual void _l125(); virtual void _l126(); virtual void _l127(); virtual void _l128(); virtual void _l129(); virtual void _l130(); virtual void _l131(); virtual void _l132(); virtual void _l133(); virtual void _l134(); virtual void _l135(); virtual void _l136(); virtual void _l137(); virtual void _l138(); virtual void _l139(); virtual void _l140(); virtual void _l141(); virtual void _l142(); virtual void _l143(); virtual void _l144(); virtual void _l145(); virtual void _l146(); virtual void _l147(); virtual void _l148(); virtual void _l149(); virtual void _l150(); virtual void _l151(); virtual void _l152(); virtual void _l153(); virtual void _l154(); virtual void _l155(); virtual void _l156(); virtual void _l157(); virtual void _l158(); virtual void _l159(); virtual void _l160(); virtual void _l161(); virtual void _l162(); virtual void _l163(); virtual void _l164(); virtual void _l165(); virtual void _l166(); virtual void _l167(); virtual void _l168(); virtual void _l169(); virtual void _l170(); virtual void _l171(); virtual void _l172(); virtual void _l173(); virtual void _l174(); virtual void _l175(); virtual void _l176(); virtual void _l177(); virtual void _l178(); virtual void _l179(); virtual void _l180(); virtual void _l181();
    virtual void ScrollUpProcess();                      // slot 182 (+0x2d8)
    virtual void ScrollDownProcess();                    // slot 183 (+0x2dc)
};
class CObserver {
public:
    virtual ~CObserver();
    virtual void ProcessEvent(EEventType event) = 0;     // slot 1
};
class CList : public CUIInterface, public CObserver {
public:
    unsigned char _pad08[0x30 - 8];
    float         Time;                                  // +0x30
    unsigned char _pad34[0x160 - 0x34];
    float         LastInputTime;                         // +0x160
    unsigned char _pad164[0x1ec - 0x164];
    float         InputDelay;                            // +0x1ec
    void ProcessEvent(EEventType event);
};
void CList::ProcessEvent(EEventType event)
{
    if (Time - LastInputTime < InputDelay)
        return;
    if (CManager::GetInstance()->IsFlagC9() && event == EVENT_25)
        return;
    if (CManager::GetInstance()->FlagA4)
        return;
    bool ignore = !LoadAndReturnDef()->Flag20A && !g_ComponentList.empty();
    if (ignore)
        return;
    switch (event)
    {
    case EVENT_UP:
        ScrollUpProcess();
        break;
    case EVENT_DOWN:
        ScrollDownProcess();
        break;
    }
}
}

void NUISystem::CUIInterface::_c001() {}
void NUISystem::CUIInterface::_c002() {}
void NUISystem::CUIInterface::_c003() {}
void NUISystem::CUIInterface::_c004() {}
void NUISystem::CUIInterface::_c005() {}
void NUISystem::CUIInterface::_c006() {}
void NUISystem::CUIInterface::_c007() {}
void NUISystem::CUIInterface::_c008() {}
void NUISystem::CUIInterface::_c009() {}
void NUISystem::CUIInterface::_c010() {}
void NUISystem::CUIInterface::_c011() {}
void NUISystem::CUIInterface::_c012() {}
void NUISystem::CUIInterface::_c013() {}
void NUISystem::CUIInterface::_c014() {}
void NUISystem::CUIInterface::_c015() {}
void NUISystem::CUIInterface::_c016() {}
void NUISystem::CUIInterface::_c017() {}
void NUISystem::CUIInterface::_c018() {}
void NUISystem::CUIInterface::_c019() {}
void NUISystem::CUIInterface::_c020() {}
void NUISystem::CUIInterface::_c021() {}
void NUISystem::CUIInterface::_c022() {}
void NUISystem::CUIInterface::_c023() {}
void NUISystem::CUIInterface::_c024() {}
void NUISystem::CUIInterface::_c025() {}
void NUISystem::CUIInterface::_c026() {}
void NUISystem::CUIInterface::_c027() {}
void NUISystem::CUIInterface::_c028() {}
void NUISystem::CUIInterface::_c029() {}
void NUISystem::CUIInterface::_c030() {}
void NUISystem::CUIInterface::_c031() {}
void NUISystem::CUIInterface::_c032() {}
void NUISystem::CUIInterface::_c033() {}
void NUISystem::CUIInterface::_c034() {}
void NUISystem::CUIInterface::_c035() {}
void NUISystem::CUIInterface::_c036() {}
void NUISystem::CUIInterface::_c037() {}
void NUISystem::CUIInterface::_c038() {}
void NUISystem::CUIInterface::_c039() {}
void NUISystem::CUIInterface::_c040() {}
void NUISystem::CUIInterface::_c041() {}
void NUISystem::CUIInterface::_c042() {}
void NUISystem::CUIInterface::_c043() {}
void NUISystem::CUIInterface::_c044() {}
void NUISystem::CUIInterface::_c045() {}
void NUISystem::CUIInterface::_c046() {}
void NUISystem::CUIInterface::_c047() {}
void NUISystem::CUIInterface::_c048() {}
void NUISystem::CUIInterface::_c049() {}
void NUISystem::CUIInterface::_c050() {}
void NUISystem::CUIInterface::_c051() {}
void NUISystem::CUIInterface::_c052() {}
void NUISystem::CUIInterface::_c053() {}
void NUISystem::CUIInterface::_c054() {}
void NUISystem::CUIInterface::_c055() {}
void NUISystem::CUIInterface::_c056() {}
void NUISystem::CUIInterface::_c057() {}
void NUISystem::CUIInterface::_c058() {}
void NUISystem::CUIInterface::_c059() {}
void NUISystem::CUIInterface::_c060() {}
void NUISystem::CUIInterface::_c061() {}
void NUISystem::CUIInterface::_c062() {}
void NUISystem::CUIInterface::_c063() {}
void NUISystem::CUIInterface::_c064() {}
void NUISystem::CUIInterface::_c065() {}
void NUISystem::CUIInterface::_c066() {}
void NUISystem::CUIInterface::_c067() {}
void NUISystem::CUIInterface::_c068() {}
void NUISystem::CUIInterface::_c069() {}
void NUISystem::CUIInterface::_c070() {}
void NUISystem::CUIInterface::_c071() {}
void NUISystem::CUIInterface::_c072() {}
void NUISystem::CUIInterface::_c073() {}
void NUISystem::CUIInterface::_c074() {}
void NUISystem::CUIInterface::_c075() {}
void NUISystem::CUIInterface::_c076() {}
void NUISystem::CUIInterface::_c077() {}
void NUISystem::CUIInterface::_c078() {}
void NUISystem::CUIInterface::_c079() {}
void NUISystem::CUIInterface::_c080() {}
void NUISystem::CUIInterface::_c081() {}
void NUISystem::CUIInterface::_c082() {}
void NUISystem::CUIInterface::_c083() {}
void NUISystem::CUIInterface::_c084() {}
void NUISystem::CUIInterface::_c085() {}
void NUISystem::CUIInterface::_c086() {}
void NUISystem::CUIInterface::_c087() {}
void NUISystem::CUIInterface::_c088() {}
void NUISystem::CUIInterface::_c089() {}
void NUISystem::CUIInterface::_c090() {}
void NUISystem::CUIInterface::_c091() {}
void NUISystem::CUIInterface::_c092() {}
void NUISystem::CUIInterface::_c093() {}
void NUISystem::CUIInterface::_c094() {}
void NUISystem::CUIInterface::_c095() {}
void NUISystem::CUIInterface::_c096() {}
void NUISystem::CUIInterface::_c097() {}
void NUISystem::CUIInterface::_c098() {}
void NUISystem::CUIInterface::_c099() {}
void NUISystem::CUIInterface::_c100() {}
void NUISystem::CUIInterface::_c101() {}
void NUISystem::CUIInterface::_c102() {}
void NUISystem::CUIInterface::_c103() {}
void NUISystem::CUIInterface::_c104() {}
void NUISystem::CUIInterface::_c105() {}
void NUISystem::CUIInterface::_c106() {}
void NUISystem::CUIInterface::_c107() {}
void NUISystem::CUIInterface::_l109() {}
void NUISystem::CUIInterface::_l110() {}
void NUISystem::CUIInterface::_l111() {}
void NUISystem::CUIInterface::_l112() {}
void NUISystem::CUIInterface::_l113() {}
void NUISystem::CUIInterface::_l114() {}
void NUISystem::CUIInterface::_l115() {}
void NUISystem::CUIInterface::_l116() {}
void NUISystem::CUIInterface::_l117() {}
void NUISystem::CUIInterface::_l118() {}
void NUISystem::CUIInterface::_l119() {}
void NUISystem::CUIInterface::_l120() {}
void NUISystem::CUIInterface::_l121() {}
void NUISystem::CUIInterface::_l122() {}
void NUISystem::CUIInterface::_l123() {}
void NUISystem::CUIInterface::_l124() {}
void NUISystem::CUIInterface::_l125() {}
void NUISystem::CUIInterface::_l126() {}
void NUISystem::CUIInterface::_l127() {}
void NUISystem::CUIInterface::_l128() {}
void NUISystem::CUIInterface::_l129() {}
void NUISystem::CUIInterface::_l130() {}
void NUISystem::CUIInterface::_l131() {}
void NUISystem::CUIInterface::_l132() {}
void NUISystem::CUIInterface::_l133() {}
void NUISystem::CUIInterface::_l134() {}
void NUISystem::CUIInterface::_l135() {}
void NUISystem::CUIInterface::_l136() {}
void NUISystem::CUIInterface::_l137() {}
void NUISystem::CUIInterface::_l138() {}
void NUISystem::CUIInterface::_l139() {}
void NUISystem::CUIInterface::_l140() {}
void NUISystem::CUIInterface::_l141() {}
void NUISystem::CUIInterface::_l142() {}
void NUISystem::CUIInterface::_l143() {}
void NUISystem::CUIInterface::_l144() {}
void NUISystem::CUIInterface::_l145() {}
void NUISystem::CUIInterface::_l146() {}
void NUISystem::CUIInterface::_l147() {}
void NUISystem::CUIInterface::_l148() {}
void NUISystem::CUIInterface::_l149() {}
void NUISystem::CUIInterface::_l150() {}
void NUISystem::CUIInterface::_l151() {}
void NUISystem::CUIInterface::_l152() {}
void NUISystem::CUIInterface::_l153() {}
void NUISystem::CUIInterface::_l154() {}
void NUISystem::CUIInterface::_l155() {}
void NUISystem::CUIInterface::_l156() {}
void NUISystem::CUIInterface::_l157() {}
void NUISystem::CUIInterface::_l158() {}
void NUISystem::CUIInterface::_l159() {}
void NUISystem::CUIInterface::_l160() {}
void NUISystem::CUIInterface::_l161() {}
void NUISystem::CUIInterface::_l162() {}
void NUISystem::CUIInterface::_l163() {}
void NUISystem::CUIInterface::_l164() {}
void NUISystem::CUIInterface::_l165() {}
void NUISystem::CUIInterface::_l166() {}
void NUISystem::CUIInterface::_l167() {}
void NUISystem::CUIInterface::_l168() {}
void NUISystem::CUIInterface::_l169() {}
void NUISystem::CUIInterface::_l170() {}
void NUISystem::CUIInterface::_l171() {}
void NUISystem::CUIInterface::_l172() {}
void NUISystem::CUIInterface::_l173() {}
void NUISystem::CUIInterface::_l174() {}
void NUISystem::CUIInterface::_l175() {}
void NUISystem::CUIInterface::_l176() {}
void NUISystem::CUIInterface::_l177() {}
void NUISystem::CUIInterface::_l178() {}
void NUISystem::CUIInterface::_l179() {}
void NUISystem::CUIInterface::_l180() {}
void NUISystem::CUIInterface::_l181() {}
void NUISystem::CManager::_m000() {}
void NUISystem::CManager::_m001() {}
void NUISystem::CManager::_m002() {}
void NUISystem::CManager::_m003() {}
void NUISystem::CManager::_m004() {}
void NUISystem::CManager::_m005() {}
void NUISystem::CManager::_m006() {}
void NUISystem::CManager::_m007() {}
void NUISystem::CManager::_m008() {}
void NUISystem::CManager::_m009() {}
void NUISystem::CManager::_m010() {}
NUISystem::CUIInterface::~CUIInterface() {}
NUISystem::CObserver::~CObserver() {}
static int g_up, g_down, g_destroy, g_loads; static bool g_c9, g_def20a;
void NUISystem::CUIDef::v00() {}
void NUISystem::CUIDef::DestroySelf() const { ++g_destroy; }
static NUISystem::CUIDef g_def; static NUISystem::CUIDef* g_defp = &g_def;
NUISystem::CDefPointer<const NUISystem::CUIDef> NUISystem::CUIInterface::LoadAndReturnDef() const
{ ++g_loads; ++g_def.RefCount; g_def.Flag20A = g_def20a; return *(NUISystem::CDefPointer<const NUISystem::CUIDef>*)&g_defp; }
void NUISystem::CUIInterface::ScrollUpProcess() { ++g_up; }
void NUISystem::CUIInterface::ScrollDownProcess() { ++g_down; }
bool NUISystem::CManager::IsFlagC9() const { return g_c9; }
static NUISystem::CManager g_mgr;
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return &g_mgr; }
static std::_List_node<NUISystem::CComponent*> g_head, g_one;
std::list<NUISystem::CComponent*> NUISystem::g_ComponentList;
static NUISystem::CList g_list;
static void Fire(NUISystem::EEventType e) { NUISystem::CObserver* o = &g_list; o->ProcessEvent(e); }
int main()
{
    using namespace NUISystem;
    g_head._M_next = g_head._M_prev = &g_head; g_ComponentList._M_node = &g_head;   // empty
    g_def.RefCount = 0; g_mgr.FlagA4 = false; g_c9 = false; g_def20a = false;
    g_list.Time = 10.0f; g_list.LastInputTime = 9.0f; g_list.InputDelay = 2.0f;
    Fire(EVENT_UP); bool ok = g_loads == 0;                                  // within the delay
    g_list.InputDelay = 1.0f;
    Fire(EVENT_UP); ok = ok && g_up == 1 && g_loads == 1 && g_destroy == 1;   // delay reached exactly, empty list
    Fire(EVENT_DOWN); ok = ok && g_down == 1;
    Fire((EEventType)7); ok = ok && g_up == 1 && g_down == 1 && g_loads == 3;
    g_c9 = true; Fire((EEventType)0x19); ok = ok && g_loads == 3; Fire(EVENT_UP); ok = ok && g_up == 2; g_c9 = false;
    g_mgr.FlagA4 = true; Fire(EVENT_UP); ok = ok && g_up == 2 && g_loads == 4; g_mgr.FlagA4 = false;
    g_head._M_next = &g_one;                                                   // non-empty list
    Fire(EVENT_UP); ok = ok && g_up == 2 && g_loads == 5 && g_destroy == 5;   // ignored, def still released
    g_def20a = true; Fire(EVENT_UP); ok = ok && g_up == 3;
    std::printf(ok ? "0053673b_TEST PASS\n" : "0053673b_TEST FAIL\n");
    return ok ? 0 : 1;
}