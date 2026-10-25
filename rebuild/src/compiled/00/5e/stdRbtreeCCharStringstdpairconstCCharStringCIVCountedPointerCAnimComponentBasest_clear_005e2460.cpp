// std::_Rb_tree<CCharString, pair<const CCharString, CIVCountedPointer<CAnimComponentBase> >, _Select1st<...>, less<CCharString>, allocator<...> >::clear()
// -- STLport: when the tree holds nodes, erase them from the root and reset the header to the empty state
// Leaf: ?clear@?$_Rb_tree@VCCharString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E2460, 51 bytes, plain `ret` (__thiscall).
// Identity: the only call in the body is _M_erase 0x005E1F30, landed as the _M_erase of the
// map<CCharString, CIVCountedPointer<CAnimComponentBase> > tree (node value { CCharString +0x10, counted pointer +0x14 });
// retail has no identical-code folding, so the clear calling that _M_erase belongs to the same tree. Ego_r.pdb / FableWin.pdb
// both carry `clear` for this map (Dinkumware `_Tree<_Tmap_traits<CCharString, CIVCountedPointer<CAnimComponentBase>, ...> >`).
// No direct callers in .text (an out-of-line COMDAT kept by the link). The old "RbTree_Clear_005e2460" /
// "CTCCarryable::OnKill" labels were propagated / descriptive (FINDINGS chunk 380).
// Body: STLport `clear()`: `_M_erase(_M_root())` (__thiscall with the root pushed), then leftmost = header, root = 0,
// rightmost = header, node count = 0 (the header re-read for every store).
class CCharString;
class CAnimComponentBase;
template <class T> class CIVCountedPointer;

namespace std {
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Pair> struct _Select1st;
template <class _Tp> struct less;

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
    void _M_erase(_Link_type __x);                      // 0x005E1F30 for this tree
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

typedef pair<const CCharString, CIVCountedPointer<CAnimComponentBase> > TAnimComponentValue;
template void _Rb_tree<CCharString, TAnimComponentValue, _Select1st<TAnimComponentValue>, less<CCharString>,
                       allocator<TAnimComponentValue> >::clear();
}
