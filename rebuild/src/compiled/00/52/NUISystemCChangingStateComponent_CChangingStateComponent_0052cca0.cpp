// NUISystem::CChangingStateComponent::CChangingStateComponent(const CChangingStateComponent& other) -- copy constructor: copies the CComponent base and starts an empty StatesToDo list
// Leaf: ??0CChangingStateComponent@NUISystem@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052CCA0, 70 bytes, `ret 4`, no EH frame.
// Identity: the body stores NUISystem::CChangingStateComponent's own vtables (RTTI 0x01245DE4 +0, 0x01245DBC +4,
//   0x01245DB4 +0x18) after calling CComponent's copy constructor ??0CComponent@NUISystem@@QAE@ABV01@@Z (landed 0x00533720)
//   with the argument, so it is Ego_r.pdb ??0CChangingStateComponent@NUISystem@@QAE@ABV01@@Z. 21 callers: the copy
//   constructors of the derived UI components (CSprite, CTextSlider, ...). The landed 0x00533720 calls this row "the
//   CDraggableInto copy-style constructor"; the stored vtables say otherwise.
// Callees: CComponent::CComponent(const CComponent&) 0x00533720; malloc 0x00BFEA0E (the STLport list header node, 0x0C).
// Layout: retail CComponent is 0x134 bytes (landed 0x00533720: CUIInterface +0, CObserver +4 (0x14), CActionParam +0x18);
//   Ego_r.pdb CChangingStateComponent then has StatesBeingDone / StatesDone and the
//   list<EChangingStateComponentState> StatesToDo (retail 4-byte STLport list at +0x13C, PDB +0x164). Only the base and
//   the list are constructed: the user copy constructor leaves the remaining members uninitialised (GetDataFromDef in the
//   base copy constructor fills the component from its def). No engine header for the class.
// Shape: STLport 4.6 `_List_base()`: `_M_node(0)` (the `mov [esi+13Ch],0` dead store before the call), then a LOCAL node
//   self-linked before it is stored (as landed 0x00533720).
#include <stdlib.h>

namespace std {
template <class T> class allocator { public: };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T, class A> class _List_base {
public:
    _List_node<T>* _M_node;
    _List_base() : _M_node(0)
    {
        _List_node<T>* n = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node = n;
    }
};
template <class T, class A = allocator<T> > class list : public _List_base<T, A> {
};
} // namespace std

namespace NUISystem {

class CUIInterface {                            // vtable 0x01246044
public:
    virtual ~CUIInterface();
};
class CObserver {                               // +0x04, 0x14 bytes in retail
public:
    virtual ~CObserver();
    unsigned char EventsToObserve[0xc];         // +0x04 std::set<EEvent>
    bool          PreventObservation;           // +0x10
};
class CActionParam {                            // +0x18
public:
    virtual ~CActionParam();
    long ActionParamType;                       // +0x1C
};

class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    CComponent(const CComponent& other);        // 0x00533720
    unsigned char _pad020[0x134 - 0x20];        // retail sizeof 0x134
};

class CChangingStateComponent : public CComponent {
public:
    enum EChangingStateComponentState { ECHANGING_STATE_NONE = 0 };
    CChangingStateComponent(const CChangingStateComponent& other);
    virtual ~CChangingStateComponent();
    EChangingStateComponentState            StatesBeingDone;   // +0x134
    EChangingStateComponentState            StatesDone;        // +0x138
    std::list<EChangingStateComponentState> StatesToDo;        // +0x13C
    float                                   ParentUpdateTime;  // +0x140
    unsigned long                           CurrentState;      // +0x144
    unsigned long                           TargetState;       // +0x148
    unsigned long                           RequestedState;    // +0x14C
    float                                   UpdateTime;        // +0x150
    bool                                    PreviousUpdateChanged;   // +0x154
    unsigned long                           PreviousState;     // +0x158
};

CChangingStateComponent::CChangingStateComponent(const CChangingStateComponent& other)
    : CComponent(other)
{
}

} // namespace NUISystem