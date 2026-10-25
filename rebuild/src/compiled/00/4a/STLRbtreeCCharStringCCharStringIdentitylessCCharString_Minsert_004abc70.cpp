// std::_Rb_tree<CCharString, CCharString, _Identity, less<CCharString> >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const CCharString&, _Rb_tree_node_base*) -- STLport red-black tree node insert for a set<CCharString>: link a new node as y's left child (y is the header, or x is set, or v < key(y), unless a hint w is given) else as its right child, fix the leftmost/rightmost/root links, rebalance, count it, return the iterator
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ABC70, 206 bytes, `ret 14h` (__fastcall: this -> ebp; the hidden
//   iterator result, x, y, &v and the hint w on the stack).
// Callees: 0x00BFEA0E malloc (STLport malloc allocator: `allocate(1)` folds to malloc(0x14)), 0x0099EC30 CCharString copy
//   ctor (the value constructed at node + 0x10, address null-tested), 0x00429950 CBasicString<char>::operator< (the
//   string-data compare behind the inline CCharString::operator<), 0x0042971D _Rb_global<bool>::_Rebalance(x, root&)
//   (static, __cdecl).
// Callers: set<CCharString>::insert_unique 0x004AC380 (calls at 0x004AC3F6 / 0x004AC44F).
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4, empty comparator +8 }; nodes =
//   _Rb_tree_node_base { _M_color +0, _M_parent +4, _M_left +8, _M_right +0xC } + the CCharString value at +0x10
//   (0x14-byte nodes). The header's parent is the root, its left the leftmost, its right the rightmost node.
// Shape: STLport 4.x `_M_insert` verbatim (`if (y == header || (w == 0 && (x != 0 || comp(KeyOfValue()(v),
//   S_key(y))))) { z = create(v); left(y) = z; if (y == header) { root = z; rightmost = z; } else if (y ==
//   leftmost) leftmost = z; } else { z = create(v); right(y) = z; if (y == rightmost) rightmost = z; } parent(z) = y;
//   left(z) = 0; right(z) = 0; _Rebalance(z, header->_M_parent); ++_M_node_count; return iterator(z);`); VC7.1 lays the
//   right arm out first. CCharString::operator< and the CBasicString<char>::operator< body as the landed tree lookups
//   (0x007ACD30): equal data pointers -> false, null left -> true, null right -> false, else the data compare.
// Label correction: catalogued as `StdSet_InsertNode` (right idea); STLport `_Rb_tree::_M_insert` of set<CCharString>.
#include <stdlib.h>                                 // malloc @ 0x00BFEA0E
#include <new>

template <class T> class CBasicString {
public:
    T* m_pData;
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    __declspec(noinline) bool operator<(const CBasicString& other) const { return Compare(other) < 0; }   // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;
    CCharString(const CCharString& other);          // 0x0099EC30
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};

namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
}

struct _Rb_tree_node_base {
    bool                _M_color;                   // +0x0
    _Rb_tree_node_base* _M_parent;                  // +0x4
    _Rb_tree_node_base* _M_left;                    // +0x8
    _Rb_tree_node_base* _M_right;                   // +0xC
};

template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                               // +0x10
};

template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};

typedef _Rb_tree_node<CCharString> _Node;

struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
};

class CCharStringSetTree {                          // STLport _Rb_tree<CCharString, CCharString, _Identity<CCharString>, less<CCharString> >
public:
    _Node*                 _M_header;               // +0x0
    unsigned int           _M_node_count;           // +0x4
    std::less<CCharString> _M_key_compare;          // +0x8

    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* __x) { return __x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* __x) { return __x->_M_right; }
    static _Rb_tree_node_base*& _S_parent(_Rb_tree_node_base* __x) { return __x->_M_parent; }
    static const CCharString& _S_key(_Rb_tree_node_base* __x) { return ((_Node*)__x)->_M_value_field; }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }

    _Node* _M_create_node(const CCharString& __x)
    {
        _Node* __tmp = (_Node*)malloc(sizeof(_Node));
        new (&__tmp->_M_value_field) CCharString(__x);
        return __tmp;
    }

    _Rb_tree_iterator _M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const CCharString& __v,
                                _Rb_tree_node_base* __w_);
};

_Rb_tree_iterator CCharStringSetTree::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const CCharString& __v,
                                                _Rb_tree_node_base* __w_)
{
    _Node* __w = (_Node*)__w_;
    _Node* __x = (_Node*)__x_;
    _Node* __y = (_Node*)__y_;
    _Node* __z;

    if (__y == _M_header ||
        (__w == 0 &&
         (__x != 0 ||
          _M_key_compare(_Identity<CCharString>()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == _M_header) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, _M_header->_M_parent);
    ++_M_node_count;
    return _Rb_tree_iterator(__z);
}