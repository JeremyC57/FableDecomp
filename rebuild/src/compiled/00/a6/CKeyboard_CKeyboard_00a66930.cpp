// CKeyboard::CKeyboard(CInputManager& inputManager, bool) -- build the CBaseClassNonCopyable base, default-construct the 256-entry CInputEvent store, create the empty maintained-event list, zero both counters and remember the input manager (the bool is unused in the retail body)
// Leaf: ??0CKeyboard@@QAE@AAVCInputManager@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66930, 96 bytes, `ret 8`, vtable 0x0129DBA8 (RTTI CKeyboard).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CInputEvent::CInputEvent @ 0x00A04410 (256 x 0x34,
//   an inline counted loop: CInputEvent has no destructor), malloc @ 0x00BFEA0E (0x40-byte STLport list sentinel).
// Label: the manifest's `NHeroInformationScreens_Keyboard_Constructor` is a guess; FableWin.pdb names
//   `??0CKeyboard@@QAE@AAVCInputManager@@_N@Z`.
// Layout (Ego_r.pdb, list shrunk to the 4-byte STLport list): EventStore +0x4, NoStoredEvents +0x3404,
//   NoEventScansRunning +0x3408, MaintainedEventsList +0x340C, InputManager +0x3410 (the PDB's +0x3418 is the 12-byte list).
// Shape: the two counters and the manager pointer are body assignments (NoStoredEvents first), after the member list's inline
//   _List_base ctor (zero store, malloc, self-link, publish).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable();                            // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
class CInputManager;
class CInputEvent {                                     // 0x34 bytes
public:
    unsigned char Data[0x34];
    CInputEvent();                                      // 0x00A04410
};
class CMaintainedEvent {                                // 0x38 bytes (list node 0x40)
public:
    unsigned char Data[0x38];
};

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                        // +0x08
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef allocator<_Node> _Node_allocator_type;
    _List_base(const allocator_type&) : _M_node(_Node_allocator_type(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x00 the sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) {}
};

}   // namespace _STL

class CKeyboard : public CBaseClassNonCopyable {
public:
    CInputEvent                     EventStore[256];        // +0x0004
    long                            NoStoredEvents;         // +0x3404
    long                            NoEventScansRunning;    // +0x3408
    _STL::list<CMaintainedEvent>    MaintainedEventsList;   // +0x340c
    CInputManager*                  InputManager;           // +0x3410
    CKeyboard(CInputManager& inputManager, bool);
    virtual ~CKeyboard();
};

CKeyboard::CKeyboard(CInputManager& inputManager, bool)
{
    NoStoredEvents = 0;
    NoEventScansRunning = 0;
    InputManager = &inputManager;
}