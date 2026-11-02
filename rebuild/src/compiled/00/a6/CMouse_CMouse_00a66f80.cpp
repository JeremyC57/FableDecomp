// CMouse::~CMouse() -- destroy the maintained-event list and the 256-entry input event store, then ~CBaseClassNonCopyable
// Leaf: ??1CMouse@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66F80, 73 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable`.
// Identity (proven): the destructor stores vtable 0x0129DBC4 first (RTTI .?AVCMouse@@ : CBaseClassNonCopyable : CBaseClass); its callers are
//   the slot-0 scalar deleting dtor 0x00A673B0 of that vtable and the derived CMouseDX destructor (vtable of CMouseDX : CMouse). Ego_r
//   (0x00C08EB0) and FableWin spell ??1CMouse@@UAE@XZ (public virtual); reloc_bindings already binds that spelling to this address.
//   Byte twin 0x00A66990 is ~CKeyboard (vtable of its own class): two symbols, no conflict. The catalog label (a forge stand-in) is wrong.
// Callees: std::_List_base<CMaintainedEvent>::clear 0x009E4710 (STLport, out of line; the node walk calls the element's virtual
//   dtor and frees each node with free), free 0x00BFEA14 (the list sentinel, null-tested: STLport allocator::deallocate),
//   CInputEvent::~CInputEvent 0x00A03B30 (an empty out-of-line body, Ego_r ??1CInputEvent@@QAE@XZ 0x00B6E5F0; the Ego_r distance to
//   SetAsNull 0x00B6E6A0 equals retail's 0x00A03B30 -> 0x00A03BE0), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Layout: engine/CMouse.h (retail opt-in FABLE_RETAIL_CMOUSE, corrected in this chunk): EventStore CInputEvent[256] +0x04 (0x34 each),
//   NoStoredEvents +0x3404, NoEventScansRunning +0x3408, MaintainedEventsList (STLport list, 4 bytes) +0x340C, InputManager +0x3410.
// Shape: a user destructor with an empty body (the vtable store comes from the user dtor); members are destroyed in reverse order:
//   the list (~_List_base: clear() + the sentinel's null-tested free), then the event array's counted ~CInputEvent loop.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();                                               // 0x009E4710 (out of line)
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
};
}   // namespace std

#include "rebuild_abi.h"
#include "engine/CBaseClass.h"

class CInputEvent {                                             // 0x34 bytes (Ego_r = retail: no containers)
public:
    ~CInputEvent();                                             // 0x00A03B30
    unsigned char Data[0x20];                                   // +0x00 UInputEventData
    long          DeviceType;                                   // +0x20 EInputDeviceType
    long          DeviceNumber;                                 // +0x24
    long          Type;                                         // +0x28 EInputEventType
    float         CurrentTime;                                  // +0x2c
    float         StartTime;                                    // +0x30
};
class CMaintainedEvent : public CBaseClass {                    // RTTI CMaintainedEvent : CBaseClass (0x38 bytes)
public:
    CInputEvent Event;                                          // +0x04
};

#define FABLE_RETAIL_CMOUSE
#include "engine/CMouse.h"

CMouse::~CMouse()
{
}