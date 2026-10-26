// std::_Rb_tree<CDateAndTime, CDateWideStringPair, less<CDateAndTime>>::insert_unique(const value_type& v) -- insert a (date, wide string) entry unless the date is already present; returns (position, inserted)
// Leaf: ?insert_unique@?$_Rb_tree@VCDateAndTime@@U?$pair@$$CBVCDateAndTime@@VCWideString@@@std@@U?$less@VCDateAndTime@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCDateAndTime@@VCWideString@@@std@@@std@@_N@2@ABU?$pair@$$CBVCDateAndTime@@VCWideString@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00593D76, 146 bytes, `ret 8` (thiscall; the pair<iterator, bool> result through
// the hidden pointer at [ebp+8], then v).
// Tree: map<CDateAndTime, CWideString> (Ego_r.pdb / FableWin.pdb `??A?$map@VCDateAndTime@@VCWideString@@...`), key at
// node + 0x10. Callees: CDateAndTime::operator< const @ 0x009E9B80 (the inline less<CDateAndTime>), _M_insert(x, y, v,
// w = 0) @ 0x00593CE4 (this chunk), the STLport `_Rb_global<bool>::_M_decrement` @ 0x004294E2 (`__cdecl`; the
// iterator's `--`).
// Label: the catalog "BinarySearchTree_FindOrInsert_DateAndTime" is descriptive: the body is STLport 4.x
// `_Rb_tree::insert_unique(const value_type&)` verbatim, with the `_M_insert(/* __x*/ __y, __y, __v)` call of the
// begin() arm (`push edi ; push v ; push esi ; push esi`: the zero x doubles as the default w).
// Shape: this in ebx, y in esi, x in edi, the comparison flag in al (`mov al,1` before the walk); the iterator `j`
// lives in [ebp-4]; the `_M_insert` result temporary reuses the dead `v` slot ([ebp+0Ch]).
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
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
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
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* __x);   // 0x004294E2
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator& operator--() { _M_node = _Rb_global<bool>::_M_decrement(_M_node); return *this; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    _Base_ptr _M_header;                             // +0x00 (_STLP_alloc_proxy::_M_data)
    size_t    _M_node_count;                         // +0x04
    _Compare  _M_key_compare;                        // +0x08

    _Base_ptr& _M_root() const { return _M_header->_M_parent; }
    _Base_ptr& _M_leftmost() const { return _M_header->_M_left; }
    static _Base_ptr& _S_left(_Base_ptr __x) { return __x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr __x) { return __x->_M_right; }
    static const _Key& _S_key(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field.first; }
    iterator begin() { return iterator(_M_leftmost()); }

private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const _Value& __v, _Base_ptr __w = 0);   // 0x00593CE4
public:
    pair<iterator, bool> insert_unique(const _Value& __v);
};

template <class _Key, class _Value, class _Compare>
pair<typename _Rb_tree<_Key, _Value, _Compare>::iterator, bool>
_Rb_tree<_Key, _Value, _Compare>::insert_unique(const _Value& __v)
{
    _Base_ptr __y = _M_header;
    _Base_ptr __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(__v.first, _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), __v.first))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}

template class _Rb_tree<CDateAndTime, pair<const CDateAndTime, CWideString>, less<CDateAndTime> >;
}