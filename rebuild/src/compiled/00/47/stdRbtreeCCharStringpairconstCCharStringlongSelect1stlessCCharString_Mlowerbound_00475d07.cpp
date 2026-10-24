// std::_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st, less<CCharString> >::_M_lower_bound(const CCharString& k) -- STLport: walk down from the root keeping the last node whose key is not less than k (the header when none)
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- port of the landed twin 0x0047547D (_Base_ptr-typed walk, size TU, compare body in the TU)
// Leaf: ?_M_lower_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$less@VCCharString@@@3@@std@@QBEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00475D07, 73 bytes, `ret 4` (thiscall: the key by reference; returns the node).
// Twin: byte-identical (relocations aside) to the landed 0x0047547D in another TU; this instance is reached from equal_range @ 0x00475CDD (erase(key) 0x00475BF6 <- TransferMap<CCharString, long> 0x0047550F). The parked speed-form, _Link_type-typed model swapped y/key registers; the landed _Base_ptr-typed size-TU (#pragma optimize("s", on)) model matches.
// Identity: the catalog's "RBTree_FindNode" is behavioural. The tree is map<CCharString, long> (CPlayerGuiDef::ScriptSprites
//   / MiniMapGraphics per Ego_r.pdb; TransferMap<CCharString, long> @ 0x00475026 calls its erase(key) 0x0047536C, whose
//   equal_range 0x00475453 calls this and _M_upper_bound 0x004754C6). Node key at +0x10.
// Callees: CBasicString<char> compare @ 0x00411570 (__fastcall, the two C strings).
// Model: STLport 4.6 `_M_lower_bound` returning _Base_ptr, `while (x) if (!comp(key(x), k)) y = x, x = left; else
//   x = right;` with less<CCharString> and CCharString's operator< inlined: same block -> false, null left -> true,
//   null right -> false, else the data compare `< 0` (the key's block is read once into ebx).
// Shape: size TU (#pragma optimize("s", on)), no frame pointer; the compare's body is visible so the key's block stays in ebx
//   (with only a declaration VC reloads it after every call: 69v73).
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

template <class Key, class Value, class Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    _Rb_tree_node_base* _M_header;                  // +0x00
    unsigned int        _M_node_count;              // +0x04
    Compare             _M_key_compare;             // +0x08
    __forceinline _Base_ptr _M_root() const { return _M_header->_M_parent; }
    static __forceinline const Key& _S_key(_Base_ptr x) { return static_cast<_Rb_tree_node<Value>*>(x)->_M_value_field.first; }
    _Base_ptr _M_lower_bound(const Key& k) const;
};

template <class Key, class Value, class Compare>
typename _Rb_tree<Key, Value, Compare>::_Base_ptr _Rb_tree<Key, Value, Compare>::_M_lower_bound(const Key& k) const
{
    _Base_ptr y = _M_header;                        // last node which is not less than k
    _Base_ptr x = _M_root();                        // current node
    while (x != 0)
        if (!_M_key_compare(_S_key(x), k))
            y = x, x = x->_M_left;
        else
            x = x->_M_right;
    return y;
}

typedef pair<const CCharString, long> _MapValue;
template _Rb_tree<CCharString, _MapValue, less<CCharString> >::_Base_ptr
    _Rb_tree<CCharString, _MapValue, less<CCharString> >::_M_lower_bound(const CCharString& k) const;
}