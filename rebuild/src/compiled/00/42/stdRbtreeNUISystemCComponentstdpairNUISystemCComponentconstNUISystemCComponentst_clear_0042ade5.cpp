// std::_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, NUISystem::CComponent*>, _Select1st<...>, less<NUISystem::CComponent*>, allocator<...> >::clear()
// -- STLport: when the tree holds nodes, erase them from the root and reset the header to the empty state
// Leaf: ?clear@?$_Rb_tree@PAVCComponent@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042ADE5, 41 bytes, plain `ret` (__thiscall).
// Identity: the tree behind CTCInventoryTrade::ItemBuyLists / ItemSellLists (struct_layouts_egor: map<NUISystem::CComponent*,
// NUISystem::CComponent*>, retail +0x200 / +0x20C). Every caller passes one of those two maps: the landed
// CTCInventoryTrade::UpdateBuyActionsList 0x0059DE04 / UpdateSellActionsList 0x0059DE76 (FableWin.pdb names; their sources
// already call this map::clear()), ConstructActualSellInventoryPC 0x005A4CF8 (+0x20C), ConstructActualSpecialInventoryPC
// 0x005A8C7C (+0x200), OpenInventory 0x005A9574 / 0x005A957F, and the inlined erase(begin(), end()) of 0x0042B854.
// The old "CTCCarryable::OnKill" label (and its generated `T::OnKill` model) was propagated.
// Body: STLport `clear()`: `_M_erase(_M_root())` (0x0042A20A, __thiscall with the root pushed), then leftmost = header,
// root = 0, rightmost = header, node count = 0 (the header re-read for every store; zero stores as `and ...,0`).
#pragma optimize("s", on)
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
    void _M_erase(_Link_type __x);                      // 0x0042A20A for this tree
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

typedef pair<NUISystem::CComponent* const, NUISystem::CComponent*> TItemListValue;
template void _Rb_tree<NUISystem::CComponent*, TItemListValue, _Select1st<TItemListValue>, less<NUISystem::CComponent*>,
                       allocator<TItemListValue> >::clear();
}
