// std::_Rb_tree<CDateAndTime, CDateWideStringPair, less<CDateAndTime>>::_M_lower_bound(const CDateAndTime& k) const -- first node whose date is not less than k (the header when none)
// Leaf: _M_lower_bound
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059339A, 46 bytes, `ret 4` (thiscall; returns the node pointer in eax).
// Tree: map<CDateAndTime, CWideString> (Ego_r.pdb / FableWin.pdb `??A?$map@VCDateAndTime@@VCWideString@@...`); the key
// sits at node + 0x10 and the CWideString value at node + 0x2C. Callee: CDateAndTime::operator<(const CDateAndTime&)
// const @ 0x009E9B80 (through the inline less<CDateAndTime>).
// Callers: map<CDateAndTime, CWideString>::operator[] @ 0x00594A08 (this chunk) and the tree lower_bound/insert
// paths at 0x00593BC6 and 0x005940C5.
// Label: the catalog "BinarySearchTree_FindNode_DateAndTime" is descriptive: the body is STLport 4.x
// `_Rb_tree::_M_lower_bound` verbatim: y = header, x = root; `if (!comp(key(x), k)) y = x, x = left; else x = right`.
// Shape: header in edi (the result), the walk in esi, the loop entered at its `test esi,esi` bottom test.
// Size TU: `#pragma optimize("s", on)`, frameless.
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

template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Base_ptr _M_header;                             // +0x00 (_STLP_alloc_proxy::_M_data)
    size_t    _M_node_count;                         // +0x04
    _Compare  _M_key_compare;                        // +0x08

    _Base_ptr& _M_root() const { return _M_header->_M_parent; }
    static _Base_ptr& _S_left(_Base_ptr __x) { return __x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr __x) { return __x->_M_right; }
    static const _Key& _S_key(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field.first; }

    _Base_ptr _M_lower_bound(const _Key& __k) const
    {
        _Base_ptr __y = _M_header;                   // last node which is not less than __k
        _Base_ptr __x = _M_root();                   // current node
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        return __y;
    }
};

template class _Rb_tree<CDateAndTime, pair<const CDateAndTime, CWideString>, less<CDateAndTime> >;
}