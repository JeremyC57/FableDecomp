// NUISystem::CChangingStateComponent::GetCurrentState() -- the state record of the current state index, or null
// Leaf: GetCurrentState
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052CF10, 44 bytes (plain `ret`).
// Callee: map<unsigned long, CUIState>::find(const unsigned long&) @ 0x0042B402 (manifest
// "CRegion::SetMiniMapRegionExitTextOffsetX" is propagated; the body is the thiscall find front end returning the iterator
// through a hidden pointer, `ret 8`).
// Label: the catalog "NUISystem::CSprite::vfunc_50" is the first vtable (0x0122F5D4 slot 50) listing this body;
// Ego_r.pdb names ?GetCurrentState@CChangingStateComponent@NUISystem@@UAEPAVCUIState@2@XZ. It is GetState(CurrentState)
// with the map lookup inlined (the same non-const find as GetState slot 135, 0x0052CEB0).
// Layout: CComponent::States (STLport map<unsigned long, CUIState>, 12 bytes: header, count, comparator) at retail +0x24;
// a tree node keeps the key at +0x10 and the CUIState at +0x14.
// CurrentState is the retail CChangingStateComponent +0x144 (landed 0x0052C8B0 layout); its address is the key reference.
// Shape: the map's address kept in esi across the call, the iterator returned into the dead `push ecx` slot, `end()`
// (the header) read after the call, `&it->second` = node + 0x14; the found
// arm written first (`if (it != end()) return &it->second; return 0;`) keeps retail's block order.
namespace NUISystem {
class CUIState {
public:
    unsigned long m_word0;
};

struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct SStateNode : _Rb_tree_node_base {          // pair<const unsigned long, CUIState>
    unsigned long first;                          // +0x10
    CUIState      second;                         // +0x14
};
class CStateMap {                                 // map<unsigned long, CUIState>
public:
    struct iterator {
        SStateNode* _M_node;
        iterator() {}
        iterator(SStateNode* node) : _M_node(node) {}
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
        SStateNode* operator->() const { return _M_node; }
    };
    SStateNode*   _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    iterator find(const unsigned long& key);      // 0x0042B402
    iterator end() { return iterator(_M_header); }
};

class CChangingStateComponent {
public:
    virtual void v000();
    unsigned char _pad04[0x24 - 4];
    CStateMap States;                             // +0x24
    unsigned char _pad30[0x144 - 0x30];
    unsigned long CurrentState;                   // +0x144
    CUIState* GetCurrentState();
};

CUIState* CChangingStateComponent::GetCurrentState()
{
    CStateMap::iterator it = States.find(CurrentState);
    if (it != States.end())
        return &it->second;
    return 0;
}
}