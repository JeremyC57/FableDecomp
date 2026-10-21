// NUISystem::CList::ProcessEvent(EEventType) -- scroll the list up or down on input events once the input delay has passed, unless the manager is busy or nothing should take input
// Leaf: ?ProcessEvent@CList@NUISystem@@UAEXW4EEventType@2@@Z
// Retail: Fable.exe @ 0x0053673B, 184 bytes, `ret 4`; CList vtable 0x012462BC (the CObserver-in-CList table) slot 1, so
// `this` is the CObserver base at +4 and CList members are addressed 4 lower (Time +0x2c, LastInputTime +0x15c,
// InputDelay +0x1e8 from the observer; the primary-vtable calls go through `add esi,-4`).
// Body: if Time - LastInputTime < InputDelay return; if CManager::GetInstance() (0x0041E5F2)->slot 11 (+0x2c, 0x0042D4F0:
// the bool at +0xc9) and event == 0x19 return; if GetInstance()->[+0xa4] return; ignore = !LoadAndReturnDef() (CComponent
// slot 108, +0x1b0, Ego_r.pdb ?LoadAndReturnDef@CComponent@NUISystem@@UBE?AV?$CDefPointer@$$CBVCUIDef@NUISystem@@@@XZ)
// ->[+0x20a] && the global component list at 0x013B8A98 is not empty; the def temporary is released (--RefCount, slot 1 at
// zero); if ignore return; event 0 -> slot 182 (+0x2d8), event 1 -> slot 183 (+0x2dc).
// Names: Time / LastInputTime / InputDelay are Ego_r.pdb CComponent / CList members (Ego_r +0x38 / +0x190 / +0x230 under
// the STLport container shrink); the manager slot-11 query, the +0xa4 manager flag, the def flag +0x20a, the global list
// (98 users, a list of CComponent*) and the slot 182 / 183 targets (ScrollUpProcess / ScrollDownProcess candidates) are
// not confirmed.
// Opt: size TU (unaligned entry) with a frame pointer (`leave`).
#pragma optimize("s", on)
#pragma optimize("y", off)
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
struct CUIDef {
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