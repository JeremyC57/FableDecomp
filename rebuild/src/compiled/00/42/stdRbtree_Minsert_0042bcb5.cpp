// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- (map<CCharString, unsigned long >) STLport node insert: create the node, link it left or right of y (fixing the header's root/leftmost/rightmost), rebalance, count it
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@K@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@K@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042BCB5, 149 bytes, thiscall `ret 14h` (hidden iterator result + x, y, v, w), ebp frame (a size-optimised /Os module: unaligned entry).
// Identity (proven): std::_Rb_tree<CCharString, pair<const CCharString, unsigned long>, _Select1st<...>, less<CCharString>, allocator<...> >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const value_type&, _Rb_tree_node_base*); evidence: its _M_create_node 0x0042B587 mallocs a 0x18-byte node and copy-constructs the value through pair<const CCharString, unsigned long>::pair(const pair&) 0x004297DB (agent952, proven: map<CCharString, unsigned long>, the landed operator[] 0x0042D131 of CCodeSectionManager); callers: insert_unique 0x0042BD4A (0x0042BDCD), insert(hint) 0x0042C940 (0x0042C981) and insert_equal 0x009D34E0 (0x009D3530; CBankFile::FilenameIndices, Ego_r.pdb multimap<CCharString, unsigned long>: the same _Rb_tree type); the same tree's _M_lower_bound is 0x0042AD34 (family J042)
// Label: the manifest's "CMemoryAllocatorVariableSize_InsertNode" is a descriptive placeholder (family J044: five byte-similar _M_insert bodies over CCharString keys, each its own
//   COMDAT; retail has no identical-code folding, so each address is one tree type).
// Callees: std::less<CCharString>::operator() 0x0042AAB3 (??R?$less@VCCharString@@@std@@QBE_NABVCCharString@@0@Z), _M_create_node 0x0042B587 (both arms),
//   std::_Rb_global<bool>::_Rebalance 0x0042971D (__cdecl, node and &root).
// Shape: STLport 4.6 _M_insert (as the landed 0x005BA8CF): `y == header || (w == 0 && (x != 0 || comp(KoV(v), key(y))))`, _M_create_node
//   out of line in both arms, `and [z+8],0 ; and [z+0Ch],0 ; mov [z+4],y`; _M_insert private, _M_create_node protected (STLport 4.6 _tree.h).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
class CCharString {                                        // 4 bytes (Ego_r)
public:
    void* PStringData;                                     // +0x0
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;   // out of line: 0x0042AAB3 for CCharString
};
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
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
};
template <class _Tp> struct _Nonconst_traits { };
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
    _Link_type _M_create_node(const value_type& __x);       // 0x0042B587
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
typedef pair<const CCharString, unsigned long > _MapValue;
typedef _Rb_tree<CCharString, _MapValue, _Select1st<_MapValue>, less<CCharString>, allocator<_MapValue> > _MapTree;
template _MapTree::iterator _MapTree::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const _MapValue&, _Rb_tree_node_base*);
} // namespace std