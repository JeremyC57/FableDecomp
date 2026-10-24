// std::_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st, less<CCharString> >::insert_unique(const value_type& v) -- STLport: descend to the insertion leaf, step back one node when it went left, and insert v only if that predecessor's key is less than v's; returns (iterator, inserted)
// Leaf: insert_unique
// Retail: Fable.exe (TLC, VC7.1) @ 0x004758E3, 202 bytes, `ret 8` (thiscall: the hidden pair<iterator, bool> result, v).
// Identity: the catalog's "RBTree_InsertOrUpdate" is behavioural. Same map<CCharString, long> tree as _M_insert @ 0x004759AD.
// Callees: CBasicString<char> compare @ 0x00411570, _Rb_global<bool>::_M_decrement(node) @ 0x004294E2 (__cdecl,
//   iterator::operator--), _Rb_tree::_M_insert(x, y, v, w = 0) @ 0x004759AD (its returned iterator homed in the dead
//   `v` argument slot).
// Model: STLport 4.6 `insert_unique` verbatim; the loop's `comp` is the bool materialised in al (`setl al`), less<CCharString>
//   and CCharString's operator< inlined, the result pair built in the hidden return slot.
// Shape: size TU (#pragma optimize("s", on)), ebp frame (this and y spilled); the compare's body is visible to the TU.
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
struct CCharStringData {                            // the shared string block (PDB 17 bytes: Data, then NoRefs at +0x0D)
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

template <class T1, class T2> struct _Pair {        // pair<iterator, bool> (the value pair above has no constructor)
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
    _Pair(const T1& a, const T2& b) : first(a), second(b) { }
};

struct _Rb_global_inst {
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* x);   // 0x004294E2
};

template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator<Value>& it) : _M_node(it._M_node) { }
    bool operator==(const _Rb_tree_iterator<Value>& y) const { return _M_node == y._M_node; }
    _Rb_tree_iterator<Value>& operator--() { _M_node = _Rb_global_inst::_M_decrement(_M_node); return *this; }
};

template <class Key, class Value, class Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value>* _Link_type;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;                  // +0x00
    unsigned int        _M_node_count;              // +0x04
    Compare             _M_key_compare;             // +0x08
    __forceinline _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    static __forceinline const Key& _S_key(_Rb_tree_node_base* x) { return static_cast<_Link_type>(x)->_M_value_field.first; }
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator _M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const Value& v, _Rb_tree_node_base* w = 0);   // 0x004759AD
    _Pair<iterator, bool> insert_unique(const Value& v);
};

template <class Key, class Value, class Compare>
_Pair<typename _Rb_tree<Key, Value, Compare>::iterator, bool> _Rb_tree<Key, Value, Compare>::insert_unique(const Value& v)
{
    _Rb_tree_node_base* y = _M_header;
    _Rb_tree_node_base* x = _M_root();
    bool comp = true;
    while (x != 0) {
        y = x;
        comp = _M_key_compare(v.first, _S_key(x));
        x = comp ? x->_M_left : x->_M_right;
    }
    iterator j = iterator(y);
    if (comp)
        if (j == begin())
            return _Pair<iterator, bool>(_M_insert(/* x */ y, y, v), true);
        else
            --j;
    if (_M_key_compare(_S_key(j._M_node), v.first))
        return _Pair<iterator, bool>(_M_insert(x, y, v), true);
    return _Pair<iterator, bool>(j, false);
}

typedef pair<const CCharString, long> _MapValue;
template _Pair<_Rb_tree<CCharString, _MapValue, less<CCharString> >::iterator, bool>
    _Rb_tree<CCharString, _MapValue, less<CCharString> >::insert_unique(const _MapValue& v);
}