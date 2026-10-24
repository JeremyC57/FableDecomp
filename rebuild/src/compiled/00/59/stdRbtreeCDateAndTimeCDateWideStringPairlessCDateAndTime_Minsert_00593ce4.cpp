// std::_Rb_tree<CDateAndTime, CDateWideStringPair, less<CDateAndTime>>::_M_insert(x, y, const value_type& v, w) -- link a new (date, wide string) node under y, fix the header links, rebalance and count it
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00593CE4, 146 bytes, `ret 14h` (thiscall; the iterator result through the hidden
// pointer at [ebp+8], then x, y, v, w).
// Tree: map<CDateAndTime, CWideString> (Ego_r.pdb / FableWin.pdb `??A?$map@VCDateAndTime@@VCWideString@@...`), 0x30-byte
// nodes (key at +0x10, CWideString at +0x2C). Callees: _M_create_node(v) @ 0x005938B1 (malloc 0x30 + the pair copy
// ctor 0x005927F4), CDateAndTime::operator< const @ 0x009E9B80 (the inline less<CDateAndTime>), the STLport
// `_Rb_global<bool>::_Rebalance(node, root&)` @ 0x0042971D (`__cdecl`, landed).
// Only caller: insert_unique(const value_type&) @ 0x00593D76 (this chunk).
// Label: the catalog "CMemoryAllocatorVariableSize_InsertNode" is propagated: the body is STLport 4.x
// `_Rb_tree::_M_insert` verbatim (y == header, or w == 0 and (x != 0 or comp(KeyOfValue(v), key(y))) links left).
// Shape: this in edi, y in ebx, the new node in esi; the two `_M_create_node` calls stay separate (one per arm), the
// left arm's header update reloads the header and shares the right arm's `rightmost = z` store.
// The source keeps STLport's `parent, left, right` store order; VC7.1 schedules them as left, right, parent.
// Frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

class CDateAndTime {
public:
    unsigned char _body[0x1c];                       // 0x1C bytes: the CWideString value follows at node + 0x2C
    bool operator<(const CDateAndTime& other) const; // 0x009E9B80
};
class CWideString {
public:
    void* PStringData;
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
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
    _Value _M_value_field;                           // +0x10
};
template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
};

template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    _Base_ptr _M_header;                             // +0x00 (_STLP_alloc_proxy::_M_data)
    size_t    _M_node_count;                         // +0x04
    _Compare  _M_key_compare;                        // +0x08

    _Link_type _M_create_node(const _Value& __x);    // 0x005938B1
    _Base_ptr& _M_root() const { return _M_header->_M_parent; }
    _Base_ptr& _M_leftmost() const { return _M_header->_M_left; }
    _Base_ptr& _M_rightmost() const { return _M_header->_M_right; }
    static _Base_ptr& _S_left(_Base_ptr __x) { return __x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr __x) { return __x->_M_right; }
    static _Base_ptr& _S_parent(_Base_ptr __x) { return __x->_M_parent; }
    static const _Key& _S_key(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field.first; }

    iterator _M_insert(_Base_ptr __x_, _Base_ptr __y_, const _Value& __v, _Base_ptr __w_);
};

template <class _Key, class _Value, class _Compare>
typename _Rb_tree<_Key, _Value, _Compare>::iterator
_Rb_tree<_Key, _Value, _Compare>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const _Value& __v, _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == _M_header || (__w == 0 && (__x != 0 || _M_key_compare(__v.first, _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;                          // also makes _M_leftmost() = __z when __y == _M_header
        if (__y == _M_header) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;                     // maintain _M_leftmost() pointing to min node
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;                    // maintain _M_rightmost() pointing to max node
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global<bool>::_Rebalance(__z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}

template class _Rb_tree<CDateAndTime, pair<const CDateAndTime, CWideString>, less<CDateAndTime> >;
}