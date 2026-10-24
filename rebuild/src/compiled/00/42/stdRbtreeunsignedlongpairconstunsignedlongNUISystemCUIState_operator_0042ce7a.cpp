// std::_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, ...>::operator=(const _Rb_tree& x) -- copy assignment of the map<unsigned long, NUISystem::CUIState> tree: clear, then clone the other tree's nodes and fix leftmost/rightmost/count
// Leaf: ??4?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042CE7A, length 105 bytes, `ret 4`.
// The catalog's "CTCCarryable_CopyAssign" is a propagated label. Callers: 0x0042D198 and 0x0053CC51.
// Callees: _Rb_tree::clear() @ 0x0042ABE1 (catalog "CTCCarryable::OnKill", propagated: erase the nodes, reset the header),
//          _Rb_tree::_M_copy @ 0x0042CB90 (row 0x0042CB90), _Rb_tree_node_base::_S_minimum / _S_maximum @ 0x0041C7E0 /
//          0x0041C800 (landed; out of line in this TU, `__cdecl`, the two argument pops deferred to one `pop ecx` each).
// Shape: STLport 4.x `_Rb_tree::operator=`: `if (this != &x)` (`cmp esi,ebx`), clear(), `_M_node_count = 0` (size-TU
// `and dword ptr [esi+4],0`), the empty comparator copy (no code), then either the empty-header reset (root 0,
// leftmost = rightmost = header) or root = _M_copy(x.root, header), leftmost = _S_minimum(root), rightmost =
// _S_maximum(root), count = x.count. The root/leftmost/rightmost accessors return references, so the header is re-read
// into edi before each call. Size TU (`optimize("s", on)`, no frame pointer here).
#pragma optimize("s", on)
namespace NUISystem {
class CUIState {                                         // 0x24 bytes (Ego_r.pdb 36)
public:
    unsigned long StateChangeFlag;
    long          StateChangeType;
    float         Position[2];
    float         Zoom[2];
    unsigned long Colour;
    float         UpdateTime;
    bool          LinearChange;
    CUIState(const CUIState& s);                         // 0x00429E29 (out of line)
};
}
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less {};
template <class T> class allocator {};
template <class V> struct _Select1st {};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00 (_Rb_tree_Color_type)
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
    static _Rb_tree_node_base* _S_minimum(_Rb_tree_node_base* x);   // 0x0041C7E0 (out of line, __cdecl)
    static _Rb_tree_node_base* _S_maximum(_Rb_tree_node_base* x);   // 0x0041C800 (out of line, __cdecl)
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    static _Link_type& _S_left(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_right); }
    static _Link_type& _S_parent(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_parent); }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    static _Link_type _S_minimum(_Link_type x) { return (_Link_type)_Rb_tree_node_base::_S_minimum(x); }
    static _Link_type _S_maximum(_Link_type x) { return (_Link_type)_Rb_tree_node_base::_S_maximum(x); }
    _Link_type _M_copy(_Link_type x, _Link_type p);      // 0x0042CB90
    void clear();                                        // 0x0042ABE1
    _Rb_tree& operator=(const _Rb_tree& x);
};
template <class K, class V, class KoV, class C, class A>
_Rb_tree<K, V, KoV, C, A>& _Rb_tree<K, V, KoV, C, A>::operator=(const _Rb_tree<K, V, KoV, C, A>& x)
{
    if (this != &x)
    {
        clear();
        _M_node_count = 0;
        if (x._M_root() == 0)
        {
            _M_root() = 0;
            _M_leftmost() = _M_header;
            _M_rightmost() = _M_header;
        }
        else
        {
            _M_root() = _M_copy((_Link_type)x._M_root(), (_Link_type)_M_header);
            _M_leftmost() = _S_minimum((_Link_type)_M_root());
            _M_rightmost() = _S_maximum((_Link_type)_M_root());
            _M_node_count = x._M_node_count;
        }
    }
    return *this;
}
}

typedef std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > > TUIStateTree;
template TUIStateTree& TUIStateTree::operator=(const TUIStateTree& x);