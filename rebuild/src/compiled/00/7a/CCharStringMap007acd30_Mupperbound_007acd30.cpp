// CCharStringMap_007acd30::_M_upper_bound(const CCharString& k) const -- STLport _Rb_tree::_M_upper_bound of a map keyed by CCharString: walk from the root to the first node whose key sorts after k, the header when none does
// Leaf: _M_upper_bound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ACD30, length 67 bytes, `ret 4`, the node pointer in eax.
// Callers: the same map's erase(key) @ 0x007AD180 (call 0x007AD255), equal_range @ 0x007ADBC0 (call 0x007ADBCA) and
//   erase-range @ 0x007ADE60 (call 0x007ADE6E); the lower-bound sibling is the landed 0x007ACC00.
// Callees: CBasicString<char>::operator< @ 0x00429950 (the string-data compare).
// Layout: STLport 4.x `_Rb_tree` = { _M_header (node pointer) +0, empty comparator, _M_node_count }; nodes =
//   _Rb_tree_node_base { _M_color +0, _M_parent +4, _M_left +8, _M_right +0xC } + value (key at +0x10). The header's
//   parent is the root.
// Shape: `y = header; x = root; while (x) if (comp(k, key(x))) y = x, x = left(x); else x = right(x); return y;` with
//   std::less<CCharString> inline (CCharString::operator<: equal data pointers -> false, null left -> true, null right ->
//   false, else the out-of-line data compare). The key's data pointer is read ONCE before the loop (`mov edi,[eax]`):
//   VC7.1 only drops the per-iteration re-read when it can see that the data compare does not write memory, i.e.
//   when the CBasicString<char>::operator< template body (the landed 0x00429950 shape) is in the TU -- as it is in
//   retail, where every TU instantiates the template and the linker folds the copies. Without the body the key is
//   re-read in the loop (65 v 67).
// Label correction: the catalog's `_Tree<_Tmap_traits<CCharString,...>>::_Ubound` is the Dinkumware name; retail is the
//   STLport tree (node layout above).
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
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

struct _Rb_tree_node_base {
    bool                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};

struct CMappedValue;
typedef std::pair<const CCharString, CMappedValue*> value_type;
typedef _Rb_tree_node<value_type> _Node;

class CCharStringMap_007acd30 {                // STLport _Rb_tree<CCharString, pair<const CCharString, V>, ...>
public:
    _Node*                     _M_header;      // +0x0
    std::less<CCharString>     _M_key_compare;
    unsigned long              _M_node_count;
    static _Node* _S_left(_Rb_tree_node_base* x) { return (_Node*)x->_M_left; }
    static _Node* _S_right(_Rb_tree_node_base* x) { return (_Node*)x->_M_right; }
    static const CCharString& _S_key(_Node* x) { return x->_M_value_field.first; }
    _Node* _M_root() const { return (_Node*)_M_header->_M_parent; }
    _Node* _M_upper_bound(const CCharString& k) const;
};

_Node* CCharStringMap_007acd30::_M_upper_bound(const CCharString& k) const
{
    _Node* y = _M_header;                      // last node which is greater than k
    _Node* x = _M_root();                      // current node
    while (x != 0)
        if (_M_key_compare(k, _S_key(x)))
            y = x, x = _S_left(x);
        else
            x = _S_right(x);
    return y;
}