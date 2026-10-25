// std::_Rb_tree<CDefString, pair<const CDefString,long>, _Select1st<...>, CCompareDefString<long>, allocator<...> >::clear()
// -- STLport: when the tree holds nodes, erase them from the root and reset the header to the empty state
// Leaf: ?clear@?$_Rb_tree@VCDefString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DBB20, 41 bytes, plain `ret` (__thiscall).
// Identity: the tree behind map<CDefString, long, CCompareDefString<long> > (Ego_r.pdb CFireEffectCreationSet
// ContainmentMap). Callers: GFSerialiseMapBinaryIn<long,CDefString,CCompareDefString<long> > 0x004E5A76 (whose built
// symbol is the PDB-exact name; it clears the map before reading), the map's destructor 0x004DD470 (destroyed by
// CPersistContext::Transfer<CFireEffectCreationSet> 0x004E59E1) and the CFireEffectCreationSet assignment 0x004E5878.
// The old "CTCCarryable::OnKill" label was propagated.
// Body: STLport `clear()`: `_M_erase(_M_root())` (0x004DA3BA, __thiscall with the root pushed), then leftmost = header,
// root = 0, rightmost = header, node count = 0 (the header re-read for every store; zero stores as `and ...,0`).
#pragma optimize("s", on)
class CDefString;
template <class T> class CCompareDefString;

namespace std {
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Pair> struct _Select1st;

struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0C
};
template <class _Value> struct _Rb_tree_node;         // _Rb_tree_node_base + the value at +0x10

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_node_base*    _M_header;                   // +0x00 (the header node)
    unsigned int           _M_node_count;               // +0x04
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    void _M_erase(_Link_type __x);                      // 0x004DA3BA for this tree
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = _M_header;
            _M_root() = 0;
            _M_rightmost() = _M_header;
            _M_node_count = 0;
        }
    }
};

typedef pair<const CDefString, long> TContainmentValue;
template void _Rb_tree<CDefString, TContainmentValue, _Select1st<TContainmentValue>, CCompareDefString<long>,
                       allocator<TContainmentValue> >::clear();
}
