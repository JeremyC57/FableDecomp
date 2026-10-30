// std::list<CMaintainedEvent>::insert(iterator) -- STLport list insert of a default-constructed CMaintainedEvent before the position; returns an iterator to the new node
// Leaf: ?insert@?$list@VCMaintainedEvent@@@std@@QAE?AU?$_List_iterator@VCMaintainedEvent@@@2@U32@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4A20, length 120 bytes.
// Label correction: the catalog's "CInputEvent_AllocNode" is the one-argument STLport `list<CMaintainedEvent>::insert(iterator)`
//   (`ret 8`: hidden result pointer + the iterator by value), i.e. `insert(pos, _Tp())` with the two-argument insert and
//   `_M_create_node` inlined. Same list type as 0x009E47E0 (erase) and CJoystick::MaintainedEventsList.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 / CBaseClass::~CBaseClass @ 0x0099A2E0, CInputEvent::CInputEvent() @ 0x00A04410, the
//   CInputEvent copy constructor @ 0x009E4850 (landed), CInputEvent::~CInputEvent @ 0x00A03B30 (empty, out of line),
//   malloc @ 0x00BFEA0E (the 0x40-byte node: two links + the 0x38-byte event).
// Model: CMaintainedEvent = CBase (vptr) + a CInputEvent at +4, both special members implicit: the temporary's default
//   constructor is CBase() + vtable 0x0129C0E0 + CInputEvent() (the landed 0x009E45B0 shape), the node copy is the
//   null-tested placement new (vtable store + the out-of-line CInputEvent copy constructor), and the implicit destructor
//   of the temporary drops its vtable reset (~CInputEvent then ~CBase, after the result store).
// Shape: STLport 4.x insert verbatim (`_Construct` = placement new, no EH: this module has no /EHsc).
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CInputEvent
{
public:
    unsigned char _data[0x34];
    CInputEvent();                                 // 0x00A04410
    CInputEvent(const CInputEvent& other);         // 0x009E4850
    ~CInputEvent();                                // 0x00A03B30
};

class CMaintainedEvent : public CBaseClass              // vtable 0x0129C0E0, 0x38 bytes
{
public:
    CInputEvent Event;                             // +0x04
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp>
struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp>
struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) {}
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp>
class list {
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    iterator insert(iterator __position) { return insert(__position, _Tp()); }
};

template list<CMaintainedEvent>::iterator list<CMaintainedEvent>::insert(iterator);

}