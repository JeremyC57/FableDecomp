// NUISystem::CAction::CAction() -- default-constructs a UI action: type left unset, empty parameter list
// Leaf: ??0CAction@NUISystem@@QAE@XZ
// Retail: Fable.exe @ 0x0042BED4, 22 bytes, returns this. Catalog label CBankFile::GetBankHandle is wrong: it constructs the
// list at +4 through std::list ctor 0x0042ABAF with a default allocator temporary and returns this. 30 callers, e.g.
// NUISystem::CManager message handler 0x0041E6D3, construct an 8-byte local then set +0 = 2 (`mov [ebp-0x248],2`) and
// copy it by value; that matches Ego_r.pdb NUISystem::CAction: ActionType +0, UIActionParams list<CActionParam*> +4.
// Layout: retail CAction is 8 bytes (list is one node pointer; ctor 0x0042ABAF stores only [this]); Ego_r.pdb is 32
// (Dinkumware list plus ThingActionParams and Valid).
namespace std {
template <class T> class allocator {
public:
    allocator() {}
};
template <class T, class A = allocator<T> > class list {
public:
    explicit list(const A& a = A());                     // 0x0042ABAF (out of line)
    void* _M_node;
};
}
namespace NUISystem {
class CActionParam;
enum EActionType { ACTION_NONE };
class CAction {
public:
    EActionType                ActionType;               // +0x00
    std::list<CActionParam*>   UIActionParams;           // +0x04
    CAction();
};
CAction::CAction()
{
}
}