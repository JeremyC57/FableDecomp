#include <cstdio>
#include <cstdlib>
struct Node { Node* next; Node* prev; unsigned long v; };
struct List {
    Node* head;
    List() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    void push(unsigned long v) { Node* n = (Node*)std::malloc(sizeof(Node)); n->v = v; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    void clear() { Node* c = head->next; while (c != head) { Node* t = c; c = c->next; std::free(t); } head->next = head; head->prev = head; }
};
struct Comp {
    Comp* children[4]; unsigned int nChildren;
    unsigned long StatesBeingDone, StatesDone; List StatesToDo; unsigned long CurrentState, TargetState, RequestedState, PreviousState;
    bool immediateOk; int changed, reached; bool lastImmediate;
    Comp() : nChildren(0), StatesBeingDone(3), StatesDone(4), CurrentState(1), TargetState(2), RequestedState(0), PreviousState(0), immediateOk(false), changed(0), reached(0), lastImmediate(false) { }
    void OnStateChanged(unsigned long) { ++changed; }
    bool IsStateReachedImmediately(unsigned long) { return immediateOk; }
    void OnStateReached() { ++reached; }
    void ForceState(unsigned long state, bool immediate)
    {
        lastImmediate = immediate;
        RequestedState = state; StatesDone = 0; StatesBeingDone = 0; StatesToDo.clear();
        PreviousState = CurrentState; CurrentState = TargetState;
        OnStateChanged(state);
        if (IsStateReachedImmediately(state)) { CurrentState = TargetState = RequestedState; OnStateReached(); }
        for (unsigned int i = 0; i < nChildren; ++i) children[i]->ForceState(RequestedState, immediate);
    }
};
int main()
{
    Comp root, child; root.children[0] = &child; root.nChildren = 1; child.immediateOk = true;
    root.StatesToDo.push(5); root.StatesToDo.push(6);
    root.ForceState(9, true);
    if (root.RequestedState != 9 || root.StatesDone || root.StatesBeingDone || root.StatesToDo.head->next != root.StatesToDo.head
        || root.PreviousState != 1 || root.CurrentState != 2 || root.reached != 0 || root.changed != 1) { std::printf("0052caf0_TEST FAIL\n"); return 1; }
    if (child.RequestedState != 9 || child.CurrentState != 9 || child.TargetState != 9 || child.reached != 1 || !child.lastImmediate) { std::printf("0052caf0_TEST FAIL\n"); return 1; }
    std::printf("0052caf0_TEST PASS\n");
    return 0;
}