// std::_Rb_tree<CCharString, pair<const CCharString, long>, ...>::_M_create_node(const value_type&) -- allocate a tree node and copy-construct the value into it
// Leaf: ?_M_create_node@
// Retail: Fable.exe @ 0x00431773, 46 bytes, `ret 4`, returns the node; `this` is unused (the node allocator is malloc).
// Catalog label NScript::CExpression_DigScript::Alloc is wrong: the body is STLport's _M_create_node -- malloc(0x18)
// (0x00BFEA0E), then a placement-new null test on the value at +0x10 and the implicit pair copy: CCharString's copy
// constructor (0x0099EC30) on first, a dword copy of second. Callers 0x00431713 / 0x0043172C / 0x00432BBC / 0x00432BF8
// (the tree's insert paths). No try/unwind frame: STLport's _STLP_TRY is empty without exception handling.
// Layout: STLport _Rb_tree_node (color +0, parent +4, left +8, right +0xc, value +0x10); CCharString is one pointer;
// the 4-byte mapped type does not reach the bytes beyond its copy, so long stands for it.
// Opt: size TU (unaligned entry, `pop ecx` after the cdecl malloc). Shape: _Construct and the pair copy constructor are
// __forceinline -- the size TU otherwise keeps either out of line (retail has both inlined).
#pragma optimize("s", on)
#include <stdlib.h>
#include <new>
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& s);                   // 0x0099EC30
    bool operator<(const CCharString& other) const;
};
namespace std {
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
    __forceinline pair(const pair& p) : first(p.first), second(p.second) {}
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> class allocator {};
template <class V> struct _Select1st {
    const typename V::first_type& operator()(const V& x) const { return x.first; }
};
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<V>*   _Link_type;
    _Base_ptr _M_header;                                 // +0x00
    size_t    _M_node_count;                             // +0x04
    C         _M_key_compare;
    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<V>));
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
};
}
typedef std::pair<const CCharString, long> CCharStringLongValue;
template class std::_Rb_tree<CCharString, CCharStringLongValue, std::_Select1st<CCharStringLongValue>, std::less<CCharString>, std::allocator<CCharStringLongValue> >;