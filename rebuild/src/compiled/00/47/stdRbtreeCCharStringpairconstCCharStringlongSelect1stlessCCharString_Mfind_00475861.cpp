// std::_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st, less<CCharString> >::_M_find(const CCharString& k) -- STLport: lower-bound walk for k, then the header unless the node found has exactly key k
// Leaf: _M_find
// Retail: Fable.exe (TLC, VC7.1) @ 0x00475861, 130 bytes, `ret 4` (thiscall: the key by reference; returns the node).
// Identity: the catalog's "RBTree_FindNodeOrUpper" is behavioural. Same map<CCharString, long> tree as _M_lower_bound
//   @ 0x0047547D / _M_insert @ 0x004759AD. Node key at +0x10.
// Callees: CBasicString<char> compare @ 0x00411570 (__fastcall, the two C strings).
// Model: STLport `find` on the node pointer: the lower-bound loop, then `if (y == header || comp(k, key(y))) y =
//   header; return y;` (the y == header arm re-stores the header, as retail) with less<CCharString> and CCharString's
//   operator< inlined (same block -> false, null left -> true, null right -> false, else the data compare `< 0`). The
//   candidate node lives in [ebp-4] (registers are full; /Os then keeps the ebp frame).
// Shape: size TU (#pragma optimize("s", on)); the compare's body is visible so the key's block stays in edi in the loop.
#pragma optimize("s", on)

// CBasicString<char>'s character compare @ 0x00411570 (landed body), visible to this TU as in retail's header so VC knows it
// writes no memory (the key's block stays hoisted in a register across the calls); never inlined, as retail.
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

struct CBasicStringChar {                           // CBasicString<char> (PDB 13 bytes; only the text pointer is read here)
    char* PData;                                    // +0x00
    __forceinline bool operator<(const CBasicStringChar& other) const { return Compare_00411570(PData, other.PData) < 0; }
};
class CCharStringData { public:                            // the shared string block (PDB 17 bytes: Data, then NoRefs at +0x0D)
    CBasicStringChar Data;                          // +0x00
};
class CCharString {
public:
    CCharStringData* PStringData;                   // +0x00
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return PStringData->Data < other.PStringData->Data;
    }
};

namespace std {
template <class T> struct less {
    __forceinline bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};
struct _Rb_tree_node_base {
    bool                _M_color;                   // +0x00
    _Rb_tree_node_base* _M_parent;                  // +0x04
    _Rb_tree_node_base* _M_left;                    // +0x08
    _Rb_tree_node_base* _M_right;                   // +0x0C
};
template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                           // +0x10
};

template <class Key, class Value, class Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    _Rb_tree_node_base* _M_header;                  // +0x00
    unsigned int        _M_node_count;              // +0x04
    Compare             _M_key_compare;             // +0x08
    __forceinline _Base_ptr _M_root() const { return _M_header->_M_parent; }
    static __forceinline const Key& _S_key(_Base_ptr x) { return static_cast<_Rb_tree_node<Value>*>(x)->_M_value_field.first; }
    _Base_ptr _M_find(const Key& k) const;
};

template <class Key, class Value, class Compare>
typename _Rb_tree<Key, Value, Compare>::_Base_ptr _Rb_tree<Key, Value, Compare>::_M_find(const Key& k) const
{
    _Base_ptr y = _M_header;                        // last node which is not less than k
    _Base_ptr x = _M_root();                        // current node
    while (x != 0)
        if (!_M_key_compare(_S_key(x), k))
            y = x, x = x->_M_left;
        else
            x = x->_M_right;
    if (y == _M_header || _M_key_compare(k, _S_key(y)))
        y = _M_header;
    return y;
}

typedef pair<const CCharString, long> _MapValue;
template _Rb_tree<CCharString, _MapValue, less<CCharString> >::_Base_ptr
    _Rb_tree<CCharString, _MapValue, less<CCharString> >::_M_find(const CCharString& k) const;
}