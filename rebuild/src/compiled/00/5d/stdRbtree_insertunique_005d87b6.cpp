// std::_Rb_tree::insert_unique(iterator __position, const value_type& __v) -- (_Rb_tree<unsigned long, pair<const unsigned long, long>, ...>, the map<unsigned long, long> tree) STLport 4.6 hinted unique insert
// Leaf: ?insert_unique@?$_Rb_tree@KU?$pair@$$CBKJ@std@@U?$_Select1st@U?$pair@$$CBKJ@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKJ@std@@U?$_Nonconst_traits@U?$pair@$$CBKJ@std@@@2@@2@U32@ABU?$pair@$$CBKJ@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D87B6, 319 bytes, thiscall, `ret 0Ch` (hidden result, hint by value, value), ebp frame.
// Identity (proven): its only caller is map<unsigned long, long>::insert(iterator, const value_type&) 0x005D8916 (the landed
//   operator[] 0x005D8933 calls that), a 29-byte forwarder that copies the hint and calls this body; the body is STLport 4.6 _tree.c
//   insert_unique(iterator, const _Value&) statement for statement (begin / end / middle hint branches, the
//   __comp_v_pos / __comp_pos_v bools, the fourth `_M_insert` argument as the "skip compare" marker). The tree is
//   Ego_r.pdb CTCInventoryStats::CreaturesKilled (map<unsigned long, long>).
// Callees: _Rb_tree::_M_insert(_Base_ptr, _Base_ptr, const value_type&, _Base_ptr) 0x0045CA8F, _Rb_tree::insert_unique(const
//   value_type&) 0x0045CA09 (pair<iterator, bool> through a hidden pointer, .first read back), _Rb_global<bool>::_M_increment
//   0x004292C0 and _M_decrement 0x004294E2 (static __cdecl).
// Label correction: catalogued as the gap label "BinarySearchTree_InsertOrFindNode".
// `optimize("y", off)` + `optimize("s", on)`, as the landed operator[] 0x005D8933 from the same TU, plus `optimize("a", on)`
//   (assume no aliasing): retail keeps v.first in edi across the _M_increment call of the interior branch instead of
//   reloading it through &v; without it VC7.1 reloads, keeps &v in a register and swaps ebx/edi (294 v 319 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma optimize("a", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base*);   // 0x004292C0
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base*);   // 0x004294E2
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    _Self& operator--() { _M_node = _Rb_global_inst::_M_decrement(_M_node); return *this; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Value value_type;
    typedef unsigned int size_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    // map key access spelled directly (as the landed hinted insert 0x00AD5030): STLport's `_KeyOfValue()(_S_value(x))`
    //   temporary makes VC7.1 load the node key before v.first; the map's _Select1st reads the same `.first`.
    static const _Key& _S_key(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field.first; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    size_type size() const { return _M_node_count; }
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x0045CA8F
    pair<iterator, bool> insert_unique(const value_type& __x);                                    // 0x0045CA09
    iterator insert_unique(iterator __position, const value_type& __x);
    _STLP_alloc_proxy<_Link_type, allocator<_Rb_tree_node<_Value> > > _M_header;
    size_type _M_node_count;
    _Compare _M_key_compare;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const _Value& __v)
{
    if (__position._M_node == this->_M_header._M_data->_M_left) {   // begin()
        if (size() <= 0)
            return insert_unique(__v).first;
        if (_M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
            return _M_insert(__position._M_node, __position._M_node, __v);
        else {
            bool __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
            if (__comp_pos_v == false)
                return __position;
            iterator __after = __position;
            ++__after;
            if (__after._M_node == this->_M_header._M_data)
                return _M_insert(0, __position._M_node, __v, __position._M_node);
            if (_M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            } else {
                return insert_unique(__v).first;
            }
        }
    } else if (__position._M_node == this->_M_header._M_data) {   // end()
        if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
            return _M_insert(0, _M_rightmost(), __v, __position._M_node);
        else
            return insert_unique(__v).first;
    } else {
        iterator __before = __position;
        --__before;
        bool __comp_v_pos = _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node));
        if (__comp_v_pos && _M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v))) {
            if (_S_right(__before._M_node) == 0)
                return _M_insert(0, __before._M_node, __v, __before._M_node);
            else
                return _M_insert(__position._M_node, __position._M_node, __v);
        } else {
            iterator __after = __position;
            ++__after;
            bool __comp_pos_v = !__comp_v_pos;
            if (!__comp_v_pos) __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
            if ((!__comp_v_pos) && __comp_pos_v
                && (__after._M_node == this->_M_header._M_data || _M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node)))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            } else {
                if (__comp_v_pos == __comp_pos_v)
                    return __position;
                else
                    return insert_unique(__v).first;
            }
        }
    }
}
template class _Rb_tree<unsigned long, pair<const unsigned long, long>, _Select1st<pair<const unsigned long, long> >,
                        less<unsigned long>, allocator<pair<const unsigned long, long> > >;
}