// _STL::map::operator[](const EIdleStateGroup& key) -- STLport map<EIdleStateGroup, CAIStateGroupBase*> subscript: the mapped state group for the key, inserting a null entry first when the key is absent
// Grade: RELOCATION_MATCH -- chunk-422 recipe: insert_unique(hint, v) 0x00801CB0 receives the pair temporary's address; a same-TU noinline stand-in that READS v but never keeps its address lets VC7.1 schedule the default value's stores as retail (the parked "pair zero-store scheduling" residue). A stand-in that ignores v lets VC drop the pair stores (101 bytes).
// Leaf: ??A?$map@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00802110, 115 bytes (+ int3 padding), __fastcall, `ret 4`, returns the
//   address of the mapped value. Callers: CTCIdleScheduler::RegisterIdleStateGroup 0x00801920 (`IdleStates[group] =
//   state` on this+0x0C) and two more in the CTCIdleScheduler TU (0x00801C15 / 0x00801C34).
// Callees: _Rb_tree::insert_unique(iterator, const value_type&) 0x00801CB0 (landed; iterator returned through a hidden
//   pointer, `ret 0Ch`). The sibling instantiation for map<EIdleStateGroup, long> is 0x00802190.
// Shape: STLport 4.x `iterator i = lower_bound(k); if (i == end() || key_comp()(k, (*i).first)) i = insert(i,
//   value_type(k, _Tp())); return (*i).second;` with lower_bound inlined (`!(key(x) < k)` keeps y = x and goes left; the
//   key is loaded once before the walk, signed `jl`). The insert's result iterator lands in the dead `k` argument slot,
//   the by-value hint iterator is copy-constructed in its argument slot, and the pair's key is stored before the pushes
//   while the zero `_Tp()` is stored after them.
// Types: Ego_r.pdb CTCIdleScheduler::IdleStates is map<EIdleStateGroup, CAIStateGroupBase*> (node key +0x10, value
//   +0x14); retail STLport trees hold the header node pointer first.
class CAIStateGroupBase;
enum EIdleStateGroup { IDLE_STATE_GROUP_NONE };

namespace _STL {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

template <class _Value> struct _Rb_tree_node {
    long                _M_color;
    _Rb_tree_node*      _M_parent;               // +0x04
    _Rb_tree_node*      _M_left;                 // +0x08
    _Rb_tree_node*      _M_right;                // +0x0c
    _Value              _M_value_field;          // +0x10
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node<_Value>* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    _Value& operator*() const { return _M_node->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Value> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>     _Node;
    typedef _Rb_tree_iterator<_Value> iterator;
    _Node*        _M_header;
    unsigned long _M_node_count;
    less<_Key>    _M_key_compare;                // empty, padded to a dword
    less<_Key> key_comp() const { return _M_key_compare; }
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    iterator lower_bound(const _Key& __k)
    {
        _Node* __y = _M_header;
        _Node* __x = _M_header->_M_parent;
        while (__x != 0)
            if (!(__x->_M_value_field.first < __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return iterator(__y);
    }
    iterator insert_unique(iterator __position, const _Value& __v);   // 0x00801CB0
};
#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x00801CB0 (reads v, never keeps its address).
template <class _Key, class _Value>
__declspec(noinline) typename _Rb_tree<_Key, _Value>::iterator
_Rb_tree<_Key, _Value>::insert_unique(iterator __position, const _Value& v)
{
    _M_node_count += *(const unsigned char*)&v.first + *(const unsigned char*)&v.second;
    return __position;
}
#pragma auto_inline(on)


template <class _Key, class _Tp> class map {
public:
    typedef pair<const _Key, _Tp>              value_type;
    typedef _Rb_tree<_Key, value_type>         _Rep_type;
    typedef typename _Rep_type::iterator       iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    less<_Key> key_comp() const { return _M_t.key_comp(); }
    iterator lower_bound(const _Key& __x) { return _M_t.lower_bound(__x); }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
};
}

template class _STL::map<EIdleStateGroup, CAIStateGroupBase*>;