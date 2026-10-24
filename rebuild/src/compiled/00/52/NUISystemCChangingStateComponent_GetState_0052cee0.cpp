// NUISystem::CChangingStateComponent::GetState(unsigned long index) const -- the component's state record with this index, or null (const overload)
// Leaf: GetState
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052CEE0, 44 bytes (`ret 4`).
// Callee: map<unsigned long, CUIState>::find(const unsigned long&) const @ 0x0052D6B0 (landed as
// CRetailTree_ulongkey::find: lower-bound walk, iterator through a hidden pointer, `ret 8`).
// Label: the catalog "NUISystem::CSprite::vfunc_134" is the first vtable (0x0122F5D4 slot 134) listing this body;
// Ego_r.pdb has ?GetState@CChangingStateComponent@NUISystem@@UAEPAVCUIState@2@K@Z and its const twin
// ?GetState@..@UBEPBVCUIState@2@K@Z. VC7.1 lays the two overloads out in reverse declaration order: slot 134 (0x0052CEE0,
// the const find 0x0052D6B0) is the const one, slot 135 this non-const one (HasState, 0x0041C5C0, calls slot 135; the
// non-const GetCurrentState 0x0052CF10 uses the non-const find 0x0042B402 like slot 135). The model declares only the
// const overload (non-virtual here: the TU emits just this body).
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
    typedef iterator const_iterator;
    SStateNode*   _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    const_iterator find(const unsigned long& key) const;   // 0x0052D6B0
    const_iterator end() const { return const_iterator(_M_header); }
};

class CChangingStateComponent {
public:
    virtual void v000();
    unsigned char _pad04[0x24 - 4];
    CStateMap States;                             // +0x24
    const CUIState* GetState(unsigned long index) const;
};

const CUIState* CChangingStateComponent::GetState(unsigned long index) const
{
    CStateMap::const_iterator it = States.find(index);
    if (it != States.end())
        return &it->second;
    return 0;
}
}