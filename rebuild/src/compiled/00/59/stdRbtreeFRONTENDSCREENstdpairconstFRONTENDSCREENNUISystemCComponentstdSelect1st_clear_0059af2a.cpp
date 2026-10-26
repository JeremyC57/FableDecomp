// std::_Rb_tree<FRONTEND_SCREEN, pair<const FRONTEND_SCREEN, NUISystem::CComponent*>, _Select1st<...>, less<FRONTEND_SCREEN>, allocator<...> >::clear()
// -- STLport: when the tree holds nodes, erase them from the root and reset the header to the empty state
// Leaf: ?clear@?$_Rb_tree@W4FRONTEND_SCREEN@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059AF2A, 41 bytes, plain `ret` (__thiscall).
// Identity: the tree behind NUISystem::CFrontEndManager::m_mapScreens (struct_layouts_egor: map<FRONTEND_SCREEN,
// NUISystem::CComponent*>, +0x54). Callers: the landed CFrontEndManager::ClearComponents 0x0059525D (FableWin.pdb name; its
// tail `jmp` here is m_mapScreens.clear()), the map's destructor 0x0059B098 (landed under the collision placeholder
// std::list<T_0059b098>::~list: clear, then free the header) and 0x0059B034. The old "RbTree_Clear" / "CTCCarryable::OnKill"
// labels and the generated AutoTinyOwnedList model were propagated / descriptive.
// Body: STLport `clear()`: `_M_erase(_M_root())` (0x0059AD64, __thiscall with the root pushed), then leftmost = header,
// root = 0, rightmost = header, node count = 0 (the header re-read for every store; zero stores as `and ...,0`).
#pragma optimize("s", on)
enum FRONTEND_SCREEN { FRONTEND_SCREEN_NONE };
namespace NUISystem { class CComponent; }

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
private:
    void _M_erase(_Link_type __x);                      // 0x0059AD64 for this tree
public:
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

typedef pair<const FRONTEND_SCREEN, NUISystem::CComponent*> TScreenValue;
template void _Rb_tree<FRONTEND_SCREEN, TScreenValue, _Select1st<TScreenValue>, less<FRONTEND_SCREEN>,
                       allocator<TScreenValue> >::clear();
}
