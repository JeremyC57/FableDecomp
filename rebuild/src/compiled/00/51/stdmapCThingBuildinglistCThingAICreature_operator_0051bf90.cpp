// std::map<CThingBuilding*, list<CThingAICreature*> >::operator[](CThingBuilding* const& k) -- STLport subscript: lower_bound, insert (k, empty list) when absent, return the building's creature list
// Leaf: ??A?$map@PAVCThingBuilding@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051BF90, 200 bytes, thiscall, `ret 4`, returns &(*it).second.
// Identity: the manifest's "std::map::insert" is propagated. Ego_r.pdb names ??A?$map@PAVCThingBuilding@@V?$list@PAVCThingAICrea...
//   (the only map-of-list subscript in the PDB name set); the 20 callers are CWorldMap::PopulateRegionWithVillagers
//   0x00502E90 and its helper 0x005029A0, which also use the landed AddToWorkBuildingList 0x005003E0 on the same
//   map<CThingBuilding*, list<CThingAICreature*> >; the landed pair copy constructor 0x00517590 is this value_type's.
// Callees: malloc 0x00BFEA0E (two list sentinels), list::_M_insert_dispatch(iterator, const_iterator, const_iterator,
//   const __false_type&) 0x00513B60 (the list copy constructor's insert(begin(), x.begin(), x.end())),
//   _Rb_tree::insert_unique(iterator, const value_type&) 0x0051B1E0, ~list 0x004AB4F0 (twice: the pair, then `_Tp()`).
// Shape: STLport 4.6 map::operator[] verbatim with `_M_lower_bound` inline (`jb` pointer compare).
//   `__default_constructed((list*)0)` builds the empty list temporary (allocate(1) = malloc(0xC), self-linked through
//   a local, published late), `value_type(k, _Tp())` copy-constructs it inline into the pair (second sentinel, then the
//   out-of-line _M_insert_dispatch with the empty `__false_type` tag homed in the dead `k` argument slot), and the
//   insert_unique result is returned through that same slot. The iterator has a user copy constructor (built in its
//   argument slot), the const_iterator only a converting one (plain pushes). Both temporaries die through the
//   out-of-line ~list, the pair first. ~list, _M_insert_dispatch and insert_unique need same-TU `noinline` stand-in
//   bodies: VC7.1 then sees that the temporaries' addresses are not captured, drops the proxies' `(_Node*)0` stores,
//   keeps the temporary's sentinel in esi (begin/end read from the register) and `this`/&k in ebx/ebp; with
//   declaration-only callees it re-reads the sentinel from memory and homes the temporary in the dead `k` slot.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CThingBuilding;
class CThingAICreature;

namespace _STL {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Alloc> struct _STLP_alloc_proxy : public _Alloc {
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
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
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x0: the sentinel
    _List_base(const allocator<_Node>& __a) : _M_node(__a, (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};

template <class _Tp> class list : public _List_base<_Tp> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;

    allocator<_Node> get_allocator() const { return allocator<_Node>(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }

    // Same-TU stand-in for 0x00513B60 (its own landed row), kept out of line.
    __declspec(noinline) void _M_insert_dispatch(iterator __pos, const_iterator __first, const_iterator __last,
                                                 const __false_type&)
    {
        for (; __first._M_node != __last._M_node; __first._M_node = __first._M_node->_M_next) {
            _Node* __n = this->_M_node.allocate(1);
            new (&__n->_M_data) _Tp(((_Node*)__first._M_node)->_M_data);
            __n->_M_next = __pos._M_node;
            __n->_M_prev = __pos._M_node->_M_prev;
            __pos._M_node->_M_prev->_M_next = __n;
            __pos._M_node->_M_prev = __n;
        }
    }
    void insert(iterator __pos, const_iterator __first, const_iterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }

    list() : _List_base<_Tp>(allocator<_Node>()) { }
    list(const list<_Tp>& __x) : _List_base<_Tp>(__x.get_allocator()) { insert(begin(), __x.begin(), __x.end()); }
    __declspec(noinline) ~list()                            // 0x004AB4F0 (same-TU stand-in, kept out of line)
    {
        _List_node_base* __cur = this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _List_node_base* __tmp = __cur;
            __cur = __cur->_M_next;
            free(__tmp);
        }
        if (this->_M_node._M_data)
            free(this->_M_node._M_data);
    }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    _Compare            _M_key_compare;                  // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Node*)__x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const _Key& __k) const
    {
        _Rb_tree_node_base* __y = _M_header;             // last node which is not less than __k
        _Rb_tree_node_base* __x = _M_header->_M_parent;  // the root
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return __y;
    }
    __declspec(noinline) iterator insert_unique(iterator __position, const value_type& __x);  // 0x0051B1E0
    iterator insert(iterator __position, const value_type& __x) { return insert_unique(__position, __x); }
    iterator lower_bound(const _Key& __k) { return iterator(_M_lower_bound(__k)); }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};

// Same-TU stand-in for _Rb_tree::insert_unique(iterator, const value_type&) 0x0051B1E0 (its own row), out of line.
#pragma auto_inline(off)
template <class _Key, class _Tp, class _Compare>
typename map<_Key, _Tp, _Compare>::iterator map<_Key, _Tp, _Compare>::insert_unique(iterator __position, const value_type& __x)
{
    if (_M_key_compare(__x.first, _S_key(__position._M_node)))
        ++_M_node_count;
    return __position;
}
#pragma auto_inline(on)

template list<CThingAICreature*>& map<CThingBuilding*, list<CThingAICreature*> >::operator[](CThingBuilding* const&);

} // namespace _STL