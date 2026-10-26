// std::_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st, less<CCharString> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport: hang a new node for v under y (left when y is the header, x is set or v's key is less than y's, unless w forces right), fix the leftmost/rightmost links, rebalance, count it
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$less@VCCharString@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@J@std@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@J@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004759AD, 164 bytes, `ret 14h` (thiscall: the hidden iterator result, x, y, v, w).
// Identity: the catalog's "RBTree_InsertOrUpdateScript" is behavioural. Same map<CCharString, long> tree as _M_find
//   @ 0x00475861; called by insert_unique @ 0x004758E3.
// Callees: _Rb_tree::_M_create_node @ 0x0046ECE7 (one call per branch), CBasicString<char> compare @ 0x00411570,
//   _Rb_global<bool>::_Rebalance(x, root) @ 0x0042971D (__cdecl, root by reference).
// Model: STLport 4.6 `_M_insert` verbatim (the y == header / w == 0 / x != 0 / key-compare chain), less<CCharString> and
//   CCharString's operator< inlined; the iterator (user copy constructor) is returned through the hidden pointer.
// Shape: size TU (#pragma optimize("s", on)); the compare's body is visible to the TU as in retail.
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

struct _Rb_global_inst {
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
};

template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator<Value>& it) : _M_node(it._M_node) { }
};

template <class Key, class Value, class Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value>* _Link_type;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;                  // +0x00
    unsigned int        _M_node_count;              // +0x04
    Compare             _M_key_compare;             // +0x08
    __forceinline _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    __forceinline _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    __forceinline _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    static __forceinline const Key& _S_key(_Rb_tree_node_base* x) { return static_cast<_Link_type>(x)->_M_value_field.first; }
protected:
    _Link_type _M_create_node(const Value& x);     // 0x0046ECE7
private:
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const Value& v, _Rb_tree_node_base* w_);
};

template <class Key, class Value, class Compare>
typename _Rb_tree<Key, Value, Compare>::iterator
_Rb_tree<Key, Value, Compare>::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const Value& v, _Rb_tree_node_base* w_)
{
    _Link_type w = (_Link_type)w_;
    _Link_type x = (_Link_type)x_;
    _Link_type y = (_Link_type)y_;
    _Link_type z;
    if (y == _M_header ||
        (w == 0 &&                                  // if w != 0, the remainder fails to false
         (x != 0 ||                                 // if x != 0, the remainder succeeds to true
          _M_key_compare(v.first, _S_key(y))))) {
        z = _M_create_node(v);
        y->_M_left = z;                             // also makes _M_leftmost() = z when y == header
        if (y == _M_header) {
            _M_root() = z;
            _M_rightmost() = z;
        }
        else if (y == _M_leftmost())
            _M_leftmost() = z;                      // maintain _M_leftmost() pointing to the min node
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_rightmost())
            _M_rightmost() = z;                     // maintain _M_rightmost() pointing to the max node
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_global_inst::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}

typedef pair<const CCharString, long> _MapValue;
template _Rb_tree<CCharString, _MapValue, less<CCharString> >::iterator
    _Rb_tree<CCharString, _MapValue, less<CCharString> >::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_,
                                                                    const _MapValue& v, _Rb_tree_node_base* w_);
}