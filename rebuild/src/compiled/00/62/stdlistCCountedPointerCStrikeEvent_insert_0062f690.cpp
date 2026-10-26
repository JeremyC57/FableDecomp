// std::list<CCountedPointer<CStrikeEvent> >::insert(iterator position, const CCountedPointer<CStrikeEvent>& x) -- STLport single-element list insert for the combat action's strike-event list: allocate a node, copy x into it and link it in before position
// Leaf: ?insert@?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062F690, 52 bytes (+ int3 padding), `ret 0Ch` (hidden iterator result,
// position, &x).
// Identity: the only caller is the range insert 0x0062F650 (call at 0x0062F676), itself called only by the list copy
// constructor 0x0062F610 that the CCombatActionBase copy constructor uses for StrikeEvents, so the element is
// CCountedPointer<CCombatActionBase::CStrikeEvent> (8 bytes). The catalog label "std_list_PushBack_MorphParameter" is
// propagated from the identical-body C3DSkeletalMorphParameter instance.
// Callees: malloc @ 0x00BFEA0E (the node allocator, `add esp,4`), _Construct(CCountedPointer<T>* p, const
// CCountedPointer<T>& x) @ 0x0062F6D0 (out of line, __fastcall: null-checked placement copy + `inc [Info]`).
// Shape: STLport 4.x `insert(iterator pos, const T& x) { _Node* tmp = _M_create_node(x); n = pos._M_node; p = n->_M_prev;
// tmp->_M_next = n; tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp; return tmp; }` with _M_create_node inlined
// (allocate(1) = malloc(16), no null test, no EH frame). The STL TU is /Gr, so the free _Construct template takes its
// arguments in ecx/edx; the iterator (user copy constructor) returns through the hidden pointer. `this` is never read.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CCPPointerInfo;
template <class T> class CCountedPointer { public:      // 8 bytes: object + shared reference-count block
    T*              Data;                        // +0x00
    CCPPointerInfo* Info;                        // +0x04
};

class CCombatActionBase {
public:
    class CStrikeEvent;
};

namespace std {

template <class _T1, class _T2>
void __fastcall _Construct(_T1* __p, const _T2& __val);   // 0x0062F6D0 for CCountedPointer<CStrikeEvent>

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                        // +0x08
};

template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
};

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, typename _Traits::_Non_const_traits> iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x00 the sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }

    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
};

} // namespace std

template std::list<CCountedPointer<CCombatActionBase::CStrikeEvent> >::iterator
    std::list<CCountedPointer<CCombatActionBase::CStrikeEvent> >::insert(iterator, const CCountedPointer<CCombatActionBase::CStrikeEvent>&);