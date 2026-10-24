// NUISystem::CChangingStateComponent::GetState(unsigned long index) -- the component's state record with this index, or null
// Leaf: GetState
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052CEB0, 44 bytes (`ret 4`).
// Callee: map<unsigned long, CUIState>::find(const unsigned long&) @ 0x0042B402 (manifest
// "CRegion::SetMiniMapRegionExitTextOffsetX" is propagated; the body is the thiscall find front end returning the iterator
// through a hidden pointer, `ret 8`).
// Label: the catalog "NUISystem::CSprite::vfunc_135" is the first vtable (0x0122F5D4 slot 135) listing this body;
// Ego_r.pdb has ?GetState@CChangingStateComponent@NUISystem@@UAEPAVCUIState@2@K@Z and its const twin
// ?GetState@..@UBEPBVCUIState@2@K@Z. VC7.1 lays the two overloads out in reverse declaration order: slot 134 (0x0052CEE0,
// the const find 0x0052D6B0) is the const one, slot 135 this non-const one (HasState, 0x0041C5C0, calls slot 135; the
// non-const GetCurrentState 0x0052CF10 uses the same find as this row).
// Layout: CComponent::States (STLport map<unsigned long, CUIState>, 12 bytes: header, count, comparator) at retail +0x24;
// a tree node keeps the key at +0x10 and the CUIState at +0x14.
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
    CUIState* GetState(unsigned long index);
};

CUIState* CChangingStateComponent::GetState(unsigned long index)
{
    CStateMap::iterator it = States.find(index);
    if (it != States.end())
        return &it->second;
    return 0;
}
}