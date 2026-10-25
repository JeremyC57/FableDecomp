// std::_Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >::insert_unique(const EClothingSuitPart& v) -- the set<EClothingSuitPart> tree's unique insert: walk down to the insertion parent, step back one node when the walk ended left, insert unless that node equals v
// Leaf: ?insert_unique@?$_Rb_tree@W4EClothingSuitPart@@W41@U?$_Identity@W4EClothingSuitPart@@@std@@U?$less@W4EClothingSuitPart@@@3@V?$allocator@W4EClothingSuitPart@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@W4EClothingSuitPart@@U?$_Nonconst_traits@W4EClothingSuitPart@@@std@@@std@@_N@2@ABW4EClothingSuitPart@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BAD5B, 134 bytes, thiscall `ret 8` (hidden pair<iterator, bool> result + v),
//   ebp frame, unaligned in the packed size-TU run of set<EClothingSuitPart> instantiations.
// Identity (sweep1, 2026-10-25): FINDINGS chunk 454 / hand454 README ("0x005BAD5B is set<EClothingSuitPart>'s
//   _Rb_tree::insert_unique"); its only caller is the landed std::set<EClothingSuitPart>::insert 0x005BAF94 (at
//   0x005BAFA0), whose reloc binding already names this Leaf -> 005bad5b; the 15 set::insert callers are all in
//   CTCInventoryClothing::GetCurrentTotalArmorValue. Callees: the landed std::_Rb_global<bool>::_M_decrement 0x004294E2
//   (__cdecl, `pop ecx`) and the tree's _M_insert 0x005BA8CF (its only caller is this body). The catalog name
//   CRetailTree_longkey_ins005ba8cf::insert is a generated stand-in (a VC6 Dinkumware `_Tree::insert` shape).
// Shape: STLport 4.6 insert_unique (as the landed 0x00485E5C): `__comp` in dl starting true, the enum key compared
//   signed at node +0x10 (`setl`), `__j` in eax, `--__j` through _Rb_global_inst::_M_decrement, _M_insert's iterator
//   homed in the dead `v` argument slot. Frame-pointer size TU.
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
    static _Base_ptr __cdecl _M_decrement(_Base_ptr __x);   // 0x004294E2
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
    _Self& operator--() { _M_node = _Rb_global_inst::_M_decrement(_M_node); return *this; }
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
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    iterator begin() { return iterator(_M_leftmost()); }

    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x005BA8CF
    pair<iterator, bool> insert_unique(const value_type& __v);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const value_type& __v)
{
    _Link_type __y = _M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}

template class _Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >;
}
