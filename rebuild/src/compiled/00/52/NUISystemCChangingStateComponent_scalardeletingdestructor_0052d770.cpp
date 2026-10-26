// NUISystem::CChangingStateComponent::`scalar deleting destructor'(unsigned int) -- stores the class's three vtables, destroys the StatesToDo list, runs ~CComponent, frees for flag bit 0
// Leaf: ??_GCChangingStateComponent
// Retail: Fable.exe 0x0052D770, 63 bytes; NUISystem::CChangingStateComponent vtable 0x01245DE4 slot 0
// (rtti-vtables.tsv). The manifest label "CThingTrackNode::`vector_deleting_destructor'" is propagated.
// Vtables: 0x01245DE4 (+0x00, primary), 0x01245DBC (+0x04, nine slots), 0x01245DB4 (+0x18, one slot);
// CComponent has the same three (0x0124608C/64/5C).
// Callees: the StatesToDo list destructor 0x0052D5C0 (out of line; labelled "SortTreeRecursively"),
// NUISystem::CComponent::~CComponent 0x00532D90, operator delete 0x00BFE9BC.
// Retail layout: StatesToDo at +0x13c (Ego_r.pdb +0x164: the retail CComponent is 0x134 bytes, its
// containers narrower). Local model: the three CComponent bases are descriptive.
// Shape: a user-declared inline virtual destructor (it stores all three vtables, unlike a generated one),
// the member destroyed before the out-of-line base destructor; `new` with an inline constructor pulls the
// vtable and the `??_G` into the TU.
namespace NUISystem {

struct CComponentRoot {                                  // +0x00 primary base (vptr only)
    virtual ~CComponentRoot();
};

struct CComponentObserver {                              // +0x04 (0x14 bytes)
    virtual void o000(); virtual void o001(); virtual void o002(); virtual void o003(); virtual void o004();
    virtual void o005(); virtual void o006(); virtual void o007(); virtual void o008();
    unsigned char _data[0x10];
};

struct CComponentListener {                              // +0x18
    virtual void l000();
};

class CComponent : public CComponentRoot, public CComponentObserver, public CComponentListener { public:
    unsigned char _body[0x134 - 0x1c];
    CComponent();
    virtual ~CComponent();                               // 0x00532D90
};

struct CStateList {                                      // list<EChangingStateComponentState>
    void* m_pHead;
    void* m_size;
    CStateList();
    ~CStateList();                                       // 0x0052D5C0
};

class CChangingStateComponent : public CComponent { public:
    long       StatesBeingDone;                          // +0x134
    long       StatesDone;                               // +0x138
    CStateList StatesToDo;                               // +0x13c
    float      ParentUpdateTime;
    unsigned long CurrentState;
    unsigned long TargetState;
    unsigned long RequestedState;
    float      UpdateTime;
    bool       PreviousUpdateChanged;
    unsigned long PreviousState;
    CChangingStateComponent() {}
    virtual ~CChangingStateComponent() {}
};

} // namespace NUISystem

NUISystem::CChangingStateComponent* NewChangingStateComponent()
{
    return new NUISystem::CChangingStateComponent;
}