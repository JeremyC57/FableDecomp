// std::list::operator=(const list& x) -- (list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>) STLport list assignment of weapon-trail geometry: overwrite the common prefix element-wise (28-byte entries), then erase this list's surplus or insert x's remaining tail
// Leaf: ??4?$list@VCWeaponTrailEntry@CEnginePrimitiveWeaponTrail@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076FF90, 125 bytes (+3 int3 padding), thiscall, `ret 4`, returns this.
// Identity: the manifest's "std::vector<CLipSyncPhoneme>::operator=" is propagated: the body walks sentinel-linked nodes
//   (`mov eax,[eax]`) and assigns 7-dword payloads at node +8, i.e. a list of the 28-byte
//   CEnginePrimitiveWeaponTrail::CWeaponTrailEntry (Ego_r.pdb: C3DVector Top, Bottom; float CreationTime); the callees
//   are that list's erase(first, last) 0x0076FBB0 and its range insert 0x0076FD40 (the one the landed
//   NCreatureHitNotification::CFlash copy constructor 0x00864F80 calls). Callers: the weapon-trail / hit-flash code
//   (0x00770213..0x007707B6 and CTCCreatureHitNotification 0x00864FD9..0x00865727).
// Callees: list::erase(iterator, iterator) 0x0076FBB0, list::_M_insert_dispatch(iterator, const_iterator,
//   const_iterator, const __false_type&) 0x0076FD40, both out of line.
// Shape: STLport 4.6 operator= verbatim, as the pointer-list sibling 0x0067CF40, but here `insert(last1, first2, last2)`
//   is the member-template forwarder to `_M_insert_dispatch(pos, first, last, __false_type())`, whose tag lands on the
//   dead `x` argument slot (where last2 is homed); the payload assignment is the implicit CWeaponTrailEntry operator=
//   (`mov ecx,7 ; rep movsd`). Iterators: the mutable one has the user copy constructor (built in its slot), the
//   const_iterators are pushed directly.
class C3DVector {
public:
    float X, Y, Z;
};
class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {                               // 28 bytes
    public:
        C3DVector Top;                                      // +0x00
        C3DVector Bottom;                                   // +0x0C
        float     CreationTime;                             // +0x18
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;

    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
    _Self operator++(int)
    {
        _Self __tmp = *this;
        this->_M_incr();
        return __tmp;
    }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;

    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }

    iterator erase(iterator __first, iterator __last);                          // 0x0076FBB0
    void _M_insert_dispatch(iterator __pos, const_iterator __first, const_iterator __last,
                            const __false_type&);                               // 0x0076FD40
    void insert(iterator __pos, const_iterator __first, const_iterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
    list& operator=(const list& __x);
};

template <class _Tp, class _Alloc>
list<_Tp, _Alloc>& list<_Tp, _Alloc>::operator=(const list<_Tp, _Alloc>& __x)
{
    if (this != &__x) {
        iterator __first1 = begin();
        iterator __last1 = end();
        const_iterator __first2 = __x.begin();
        const_iterator __last2 = __x.end();
        while (__first1 != __last1 && __first2 != __last2)
            *__first1++ = *__first2++;
        if (__first2 == __last2)
            erase(__first1, __last1);
        else
            insert(__last1, __first2, __last2);
    }
    return *this;
}

} // namespace std

template std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>&
std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::operator=(const std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>&);