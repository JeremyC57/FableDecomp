// NUISystem::CChangingStateComponent::UpdateStateChange() -- advance the state machine: when the current batch of change states is all done, start the next queued batch (process its own change, push the requested state to the owned children); otherwise mark the own/child parts done as they finish; once nothing is queued and everything is done, commit the target state
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052CD20, 400 bytes. NUISystem::CSprite vtable 0x0122F5D4 slot 136.
// Callees: free 0x00BFEA14 (the STLport list node, inline pop_front); virtual slots 47 (+0xbc, ChangeState on
//          the children), 52 (+0xd0, GetParent), 58 (+0xe8, ProcessChangeState 0x0052C920), 65 (+0x104, GetType),
//          138 (+0x228, InternalChanged 0x0052C780) and 139 (+0x22c, AllChildrenChangedState 0x0052CBF0; the
//          manifest label CList::AllScrollingChildrenDeleting is propagated).
//
// Shape: StatesToDo is the bare 4-byte STLport list sentinel pointer; empty() is `[head] == head`, front() and
// the inline pop_front (unlink, then free the node) re-read the sentinel. The batch test is
// `(StatesDone & StatesBeingDone) == StatesBeingDone`. A type-8 child skips ChangeState for requested states
// 1, 3 and 4. The commit block keeps CurrentState/TargetState in registers for the stores.
// Layout: local model -- the retail CChangingStateComponent tail of the landed rows (0x0052C730, 0x0052C780,
// 0x0052CAB0): CComponent is 0x134 bytes in retail, StatesBeingDone +0x134 .. PreviousState +0x158.
namespace NUISystem {

extern "C" void __cdecl free(void* block);   // 0x00BFEA14

struct SStateListNode {                     // STLport _List_node<EChangingStateComponentState>
    SStateListNode* _M_next;
    SStateListNode* _M_prev;
    long            _M_data;
};
struct CStateList {                         // STLport list<EChangingStateComponentState> (4 bytes)
    SStateListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    long& front() { return _M_node->_M_next->_M_data; }
    void pop_front()
    {
        SStateListNode* node = _M_node->_M_next;
        SStateListNode* next = node->_M_next;
        SStateListNode* prev = node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(node);
    }
};
class CComponent;
struct CCountedComponentPointer {           // CCountedPointer<NUISystem::CComponent>
    CComponent* Object;
    void*       Info;
    CComponent* operator->() const { return Object; }
};
struct CComponentPointerVector {            // STLport vector<CCountedPointer<CComponent>> (12 bytes)
    CCountedComponentPointer* _M_start;
    CCountedComponentPointer* _M_finish;
    CCountedComponentPointer* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    CCountedComponentPointer& operator[](unsigned int i) { return _M_start[i]; }
};
class CComponent {
public:
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void ChangeState(unsigned long state, float time);      // slot 47 (+0xbc)
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void VSlot51();
    virtual CComponent* GetParent();                                // slot 52 (+0xd0)
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void ProcessChangeState();                              // slot 58 (+0xe8), 0x0052C920
    virtual void VSlot59();
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void VSlot62();
    virtual void VSlot63();
    virtual void VSlot64();
    virtual long GetType();                                         // slot 65 (+0x104)
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual void VSlot82();
    virtual void VSlot83();
    virtual void VSlot84();
    virtual void VSlot85();
    virtual void VSlot86();
    virtual void VSlot87();
    virtual void VSlot88();
    virtual void VSlot89();
    virtual void VSlot90();
    virtual void VSlot91();
    virtual void VSlot92();
    virtual void VSlot93();
    virtual void VSlot94();
    virtual void VSlot95();
    virtual void VSlot96();
    virtual void VSlot97();
    virtual void VSlot98();
    virtual void VSlot99();
    virtual void VSlot100();
    virtual void VSlot101();
    virtual void VSlot102();
    virtual void VSlot103();
    virtual void VSlot104();
    virtual void VSlot105();
    virtual void VSlot106();
    virtual void VSlot107();
    virtual void VSlot108();
    virtual void VSlot109();
    virtual void VSlot110();
    virtual void VSlot111();
    virtual void VSlot112();
    virtual void VSlot113();
    virtual void VSlot114();
    virtual void VSlot115();
    virtual void VSlot116();
    virtual void VSlot117();
    virtual void VSlot118();
    virtual void VSlot119();
    virtual void VSlot120();
    virtual void VSlot121();
    virtual void VSlot122();
    virtual void VSlot123();
    virtual void VSlot124();
    virtual void VSlot125();
    virtual void VSlot126();
    virtual void VSlot127();
    virtual void VSlot128();
    virtual void VSlot129();
    virtual void VSlot130();
    virtual void VSlot131();
    virtual void VSlot132();
    virtual void VSlot133();
    virtual void VSlot134();
    virtual void VSlot135();
    virtual void UpdateStateChange();                               // slot 136 (+0x220)
    virtual bool ChangedStateLastUpdate();                          // slot 137 (+0x224)
    virtual bool InternalChanged();                                 // slot 138 (+0x228), 0x0052C780
    virtual bool AllChildrenChangedState();                         // slot 139 (+0x22c), 0x0052CBF0
    unsigned char _pad04[0xac];             // +0x04
    CComponentPointerVector Children;       // +0xb0
    unsigned char _padbc[0x78];             // +0xbc .. +0x134
};
class CChangingStateComponent : public CComponent {
public:
    long          StatesBeingDone;          // +0x134 (EChangingStateComponentState bits)
    long          StatesDone;               // +0x138
    CStateList    StatesToDo;               // +0x13c
    float         ParentUpdateTime;         // +0x140
    unsigned long CurrentState;             // +0x144
    unsigned long TargetState;              // +0x148
    unsigned long RequestedState;           // +0x14c
    float         UpdateTime;               // +0x150
    bool          PreviousUpdateChanged;    // +0x154
    unsigned long PreviousState;            // +0x158
    virtual void UpdateStateChange();
};

void CChangingStateComponent::UpdateStateChange()
{
    if (!StatesToDo.empty() && (StatesDone & StatesBeingDone) == StatesBeingDone) {
        StatesBeingDone = StatesToDo.front();
        StatesToDo.pop_front();
        if (StatesBeingDone & 2)
            ProcessChangeState();
        if (StatesBeingDone & 1) {
            for (unsigned int i = 0; i < Children.size(); ++i) {
                if (Children[i]->GetParent() == this) {
                    if (Children[i]->GetType() != 8
                        || (RequestedState != 1 && RequestedState != 3 && RequestedState != 4))
                        Children[i]->ChangeState(RequestedState, UpdateTime);
                }
            }
        }
    } else {
        if ((StatesBeingDone & 2) && InternalChanged())
            StatesDone |= 2;
        if ((StatesBeingDone & 1) && AllChildrenChangedState())
            StatesDone |= 1;
    }
    if (StatesToDo.empty() && (StatesDone & StatesBeingDone) == StatesBeingDone && CurrentState != TargetState) {
        StatesDone = 0;
        StatesBeingDone = 0;
        PreviousState = CurrentState;
        CurrentState = TargetState;
    }
}

}