// std::_Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const EClothingSuitPart& v, _Rb_tree_node_base* w) -- the set<EClothingSuitPart> tree's node insert: create the node, link it left or right of y (fixing the header's root/leftmost/rightmost), rebalance, count it
// Leaf: ?_M_insert@?$_Rb_tree@W4EClothingSuitPart@@W41@U?$_Identity@W4EClothingSuitPart@@@std@@U?$less@W4EClothingSuitPart@@@3@V?$allocator@W4EClothingSuitPart@@@3@@std@@AAE?AU?$_Rb_tree_iterator@W4EClothingSuitPart@@U?$_Nonconst_traits@W4EClothingSuitPart@@@std@@@2@PAU_Rb_tree_node_base@2@0ABW4EClothingSuitPart@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BA8CF, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, v, w),
//   ebp frame, in the packed size-TU run of set<EClothingSuitPart> instantiations.
// Identity (sweep1, 2026-10-25): its only caller is the tree's insert_unique 0x005BAD5B (call at 0x005BADBD; FINDINGS
//   chunk 454: "0x005BAD5B is set<EClothingSuitPart>'s _Rb_tree::insert_unique"), which passes (y, y, v, 0) or
//   (x, y, v, 0) exactly as STLport's insert_unique does; the key is compared signed at node +0x10 (`jl`) against
//   v, the node comes from _M_create_node 0x005BA3AC (0x14-byte node, dword value at +0x10; both its calls are here)
//   and the fix-up is the landed std::_Rb_global<bool>::_Rebalance 0x0042971D (__cdecl, node and &root). The catalog
//   name CRetailTree_longkey_buy005ba3ac::Insert is a generated stand-in (a VC6 Dinkumware `_Tree::_Insert` shape).
//   Access: public (QAE), as every _Rb_tree::_M_insert reloc binding in the tree spells it.
// Shape: STLport 4.6 _M_insert (as the landed 0x00AD42C0): `y == header || (w == 0 && (x != 0 || comp(v, key(y))))`,
//   _M_create_node out of line in both arms, `and [z+8],0 ; and [z+0Ch],0 ; mov [z+4],y`. Frame-pointer size TU.
#pragma optimize("y", off)
#pragma optimize("s", on)

enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE };

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> class allocator { };
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp> struct _Const_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

protected:
    _Link_type _M_create_node(const value_type& __x);                                                 // 0x005BA3AC
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const value_type& __v,
                                                                 _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == _M_header._M_data ||
        (__w == 0 &&                    // if w != 0, the remainder fails to false
         (__x != 0 ||                   // if x != 0, the remainder succeeds to true
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;             // also makes _M_leftmost() = z when y == header
        if (__y == _M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;        // maintain _M_leftmost() pointing to the min node
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;       // maintain _M_rightmost() pointing to the max node
    }
    __z->_M_parent = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, _M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}

template class _Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >;
}
