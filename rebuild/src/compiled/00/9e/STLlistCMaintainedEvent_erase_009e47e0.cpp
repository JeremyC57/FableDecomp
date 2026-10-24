// _STL::list<CMaintainedEvent>::erase(iterator) -- STLport list erase: unlink the node, destroy its CMaintainedEvent through the virtual destructor, free the node and return an iterator to the next node
// Leaf: ?erase@?$list@VCMaintainedEvent@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E47E0, length 45 bytes.
// Label correction: the catalog's "EventNode_Unlink" is STLport `list<CMaintainedEvent>::erase(iterator)`: its callers
//   (CMouse::ProcessMaintainedEventsFromEvent 0x00A66FD0, at 0x00A671C9 / 0x00A67228 / 0x00A67367) pass the list in ecx, the
//   hidden result pointer and the iterator by value built in its argument slot (`push ecx ; mov eax,esp ; mov [eax],esi`,
//   so the iterator has a user copy constructor); the node value is the polymorphic 0x38-byte CMaintainedEvent (vtable
//   0x0129C0E0, same list type as CJoystick::MaintainedEventsList whose clear is the landed 0x009E4710).
// Callees: CMaintainedEvent's virtual destructor through vtable slot 0 (`push 0 ; call [eax]`: the explicit `_Destroy`),
//   free @ 0x00BFEA14 (the allocator's deallocate, no null test).
// Shape: STLport 4.x `erase` verbatim -- next/prev read from the node, prev->next then next->prev stored, `_Destroy(&n->_M_data)`,
//   deallocate, `return iterator(next)`; `this` (ecx) is never read.
extern "C" void __cdecl free(void* block);

class CMaintainedEvent
{
public:
    virtual ~CMaintainedEvent();
    unsigned char _body[0x34];
};

namespace _STL {

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

template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp>
class list {
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        free(__n);
        return iterator(__next_node);
    }
};

template class list<CMaintainedEvent>;

}