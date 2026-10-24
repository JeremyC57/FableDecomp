// NUISystem::CChangingStateComponent::ChangedState() -- true once no state change is queued and every state being done is done
// Leaf: ChangedState
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052C8F0, 37 bytes (plain `ret`), no callees.
// Label: the catalog "NUISystem::CSprite::vfunc_49" is the first vtable (0x0122F5D4 slot 49) listing this body;
// Ego_r.pdb names ?ChangedState@CChangingStateComponent@NUISystem@@UAE_NXZ (the landed 0x0052C8B0 comment calls
// slot 49 "IsChanged").
// Layout: retail CChangingStateComponent tail (as the landed 0x0052C8B0): StatesBeingDone +0x134 and StatesDone +0x138
// (flag words), StatesToDo +0x13c (STLport list, the 4-byte sentinel pointer).
// Shape: `list.empty()` is `node->next == node` (`cmp [eax],eax`); `a && b` materialises a full-register 0/1.
namespace NUISystem {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct CStateList {                                   // list<EChangingStateComponentState>
    _List_node_base* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
class CChangingStateComponent {
public:
    virtual bool ChangedState();
    unsigned char _pad04[0x134 - 4];
    unsigned long StatesBeingDone;                    // +0x134
    unsigned long StatesDone;                         // +0x138
    CStateList    StatesToDo;                         // +0x13c
};

bool CChangingStateComponent::ChangedState()
{
    return StatesToDo.empty() && (StatesDone & StatesBeingDone) == StatesBeingDone;
}
}