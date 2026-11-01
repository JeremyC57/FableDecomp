// NUISystem::CChangingStateComponent::~CChangingStateComponent() -- store the class's three vtables, destroy the StatesToDo list, then run ~CComponent
// Leaf: ??1CChangingStateComponent@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052CCF0, 42 bytes, thiscall, tail `jmp` to ~CComponent.
// Identity (proven): it stores the three CChangingStateComponent vtables 0x01245DE4 (+0x00), 0x01245DBC (+0x04), 0x01245DB4 (+0x18)
//   (RTTI .?AVCChangingStateComponent@NUISystem@@ over CComponent : CUIInterface, CObserver +4, CActionParam +0x18); the class's own
//   ??_G 0x0052D770 (slot 0) expands the same body; 32 callers are derived-class destructors. Ego_r.pdb / FableWin.pdb:
//   ??1CChangingStateComponent@NUISystem@@UAE@XZ (already the binding of caller 0x00541870; 0x00549A00's "??1CDraggableInto@...QAE"
//   binding of this address is wrong). The catalog's "CTCIdleScheduler::Clear" is propagated.
// Callees: std::_List_base<EChangingStateComponentState>::~_List_base 0x0052D5C0 (StatesToDo +0x13C; clear() inlined, free
//   0x00BFEA14; landed caller bindings say ??1CStateList@NUISystem / catalog "SortTreeRecursively"), NUISystem::CComponent::~CComponent 0x00532D90.
// Layout: retail CComponent is 0x134 bytes (its STLport containers are narrower than Ego_r's); StatesBeingDone +0x134, StatesDone
//   +0x138, StatesToDo +0x13C (one-word STLport list). No engine header exists for these NUISystem classes.
// Shape: user-declared inline virtual dtor (stores all three vtables), as the landed ??_G 0x0052D770.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
namespace std {
template <class _Tp> class allocator { public: };
template <class _Tp> struct less { };
template <class _Tp> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    ~_List_base();
    _List_node<_Tp>* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    ~set();
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}
namespace NUISystem {
enum EEvent { };
enum EActionParamType { };
class CUIInterface {                                        // vptr only
public:
    virtual ~CUIInterface();
};
class CObserver {                                           // +0x04 in CComponent, 0x14 bytes
public:
    virtual ~CObserver();
    std::set<EEvent> EventsToObserve;
    bool PreventObservation;
};
class CActionParam {                                        // +0x18 in CComponent
public:
    virtual ~CActionParam();
    EActionParamType ActionParamType;
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    virtual ~CComponent();                                  // 0x00532D90
    unsigned char _body[0x134 - 0x20];
};
class CChangingStateComponent : public CComponent {
public:
    enum EChangingStateComponentState { };
    virtual ~CChangingStateComponent() {}
    EChangingStateComponentState StatesBeingDone;           // +0x134
    EChangingStateComponentState StatesDone;                // +0x138
    std::list<EChangingStateComponentState> StatesToDo;     // +0x13c
    float ParentUpdateTime;
    unsigned long CurrentState;
    unsigned long TargetState;
    unsigned long RequestedState;
    float UpdateTime;
    bool PreviousUpdateChanged;
    unsigned long PreviousState;
};
}
void DestroyChangingStateComponent(NUISystem::CChangingStateComponent* p) { p->NUISystem::CChangingStateComponent::~CChangingStateComponent(); }