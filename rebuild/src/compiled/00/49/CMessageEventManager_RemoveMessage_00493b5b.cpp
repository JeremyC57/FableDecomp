#pragma optimize("s", on)
#pragma optimize("y", off)
// CMessageEventManager::RemoveMessage(const CMessageEvent* event) -- erase the node of the message list (+4) that holds the given event (list::erase @ 0x004937CB with the iterator by value), nothing when it is not listed
// Leaf: ?RemoveMessage@CMessageEventManager@@QAEXPBVCMessageEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493B5B, length 48 bytes.
// Callees: std::list<CMessageEvent>::erase(iterator) @ 0x004937CB (thiscall; the 4-byte iterator has a user copy constructor so it is built in its argument slot -- `push ecx ; mov edx,esp ; mov [edx],eax` -- and the iterator result comes back by hidden pointer into the dead `event` argument slot). Size-optimised module with the frame pointer kept (`push ebp ; mov ebp,esp`): optimize("s") + optimize("y", off). Shape: the list in ecx, its sentinel in edx, the node in eax, `ret 4`.
// Fix (sweep 3): Ego_r/FableWin name ?RemoveMessage@CMessageEventManager@@QAEXPBVCMessageEvent@@@Z (CMessageEvent is a
//   class); Messages is the real STLport list<CMessageEvent> (landed erase 0x004937CB's own model), so the callee binds
//   std::list<CMessageEvent>::erase instead of the invented CMessageEventList::erase(CMessageIterator).
#include <stddef.h>

class CMessageEvent {                                    // 0x58 bytes, vtable 0x0125BE8C (Ego_r.pdb layout)
public:
    virtual ~CMessageEvent();                            // slot 0
    unsigned char _Data[0x54];                           // +0x04 .. +0x57 (Type +8, OriginatorUID +0x20, ... POriginator +0x48)
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };

template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _AllocProxy;
    _AllocProxy _M_node;                                   // +0x0: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator erase(iterator __position);                   // 0x004937CB
};

} // namespace std

class CWorld;
class CMessageEventManager {
public:
    CWorld*                   World;                       // +0x00
    std::list<CMessageEvent>  Messages;                    // +0x04
    void RemoveMessage(const CMessageEvent* event);
};

void CMessageEventManager::RemoveMessage(const CMessageEvent* event)
{
    typedef std::list<CMessageEvent>::_Node _Node;
    std::_List_node_base* head = Messages._M_node._M_data;
    for (std::_List_node_base* node = head->_M_next; node != head; node = node->_M_next) {
        if (&((_Node*)node)->_M_data == event) {
            Messages.erase(std::list<CMessageEvent>::iterator((_Node*)node));
            return;
        }
    }
}
