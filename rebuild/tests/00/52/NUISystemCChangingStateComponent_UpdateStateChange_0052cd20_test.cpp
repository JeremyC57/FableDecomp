// Behaviour test for NUISystem::CChangingStateComponent::UpdateStateChange (0x0052CD20): batch start with
// pop_front, child ChangeState filtering, done-bit marking and the final commit of the target state.
#include <cstdio>
#include <cstdlib>

struct SStateListNode { SStateListNode* _M_next; SStateListNode* _M_prev; long _M_data; };
struct CStateList {
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
        std::free(node);
    }
    void push_back(long v)
    {
        SStateListNode* n = (SStateListNode*)std::malloc(sizeof(SStateListNode));
        n->_M_data = v; n->_M_next = _M_node; n->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n;
    }
};
struct CComponent;
struct CCountedComponentPointer { CComponent* Object; void* Info; CComponent* operator->() const { return Object; } };
struct CComponentPointerVector {
    CCountedComponentPointer* _M_start; CCountedComponentPointer* _M_finish;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CCountedComponentPointer& operator[](unsigned int i) { return _M_start[i]; }
};
struct CComponent {
    CComponentPointerVector Children;
    CComponent* Parent; long Type; int ChangeCalls; unsigned long LastState; float LastTime;
    virtual ~CComponent() {}
    virtual void ChangeState(unsigned long state, float time) { ++ChangeCalls; LastState = state; LastTime = time; }
    virtual CComponent* GetParent() { return Parent; }
    virtual long GetType() { return Type; }
    virtual void ProcessChangeState() {}
    virtual bool InternalChanged() { return false; }
    virtual bool AllChildrenChangedState() { return false; }
};
struct CChangingStateComponent : CComponent {
    long StatesBeingDone, StatesDone; CStateList StatesToDo;
    unsigned long CurrentState, TargetState, RequestedState; float UpdateTime; unsigned long PreviousState;
    int Processed; bool OwnDone, ChildrenDone;
    virtual void ProcessChangeState() { ++Processed; }
    virtual bool InternalChanged() { return OwnDone; }
    virtual bool AllChildrenChangedState() { return ChildrenDone; }
    void UpdateStateChange();
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

static void InitC(CComponent& c) { c.Children._M_start = c.Children._M_finish = 0; c.Parent = 0; c.Type = 0; c.ChangeCalls = 0; }

int main()
{
    SStateListNode sentinel; sentinel._M_next = sentinel._M_prev = &sentinel;
    CChangingStateComponent s; InitC(s);
    s.StatesToDo._M_node = &sentinel;
    s.StatesBeingDone = 0; s.StatesDone = 0; s.CurrentState = 1; s.TargetState = 5; s.RequestedState = 3;
    s.UpdateTime = 0.5f; s.PreviousState = 0; s.Processed = 0; s.OwnDone = false; s.ChildrenDone = false;
    CComponent a, b, c; InitC(a); InitC(b); InitC(c);
    a.Parent = &s; b.Parent = &s; b.Type = 8; c.Parent = 0;
    CCountedComponentPointer kids[3] = { { &a, 0 }, { &b, 0 }, { &c, 0 } };
    s.Children._M_start = kids; s.Children._M_finish = kids + 3;
    s.StatesToDo.push_back(3);
    s.UpdateStateChange();                                   // starts batch 3
    bool ok = s.StatesBeingDone == 3 && s.StatesToDo.empty() && s.Processed == 1
        && a.ChangeCalls == 1 && a.LastState == 3 && a.LastTime == 0.5f
        && b.ChangeCalls == 0 && c.ChangeCalls == 0 && s.CurrentState == 1;
    s.OwnDone = true;
    s.UpdateStateChange();                                   // own part done
    ok = ok && s.StatesDone == 2 && s.CurrentState == 1;
    s.ChildrenDone = true;
    s.UpdateStateChange();                                   // all done: commit
    ok = ok && s.StatesDone == 0 && s.StatesBeingDone == 0 && s.CurrentState == 5 && s.PreviousState == 1;
    s.RequestedState = 2; s.StatesToDo.push_back(1);
    s.UpdateStateChange();                                   // type-8 child gets state 2
    ok = ok && b.ChangeCalls == 1 && b.LastState == 2 && a.ChangeCalls == 2 && s.Processed == 1;
    if (!ok) { std::printf("0052cd20_TEST FAIL\n"); return 1; }
    std::printf("0052cd20_TEST PASS\n");
    return 0;
}