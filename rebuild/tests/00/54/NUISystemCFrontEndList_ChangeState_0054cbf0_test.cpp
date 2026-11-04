// Behaviour test for NUISystem::CFrontEndList::ChangeState (0x0054CBF0): a model of the per-state observer bookkeeping.
// ACTIVE observes the list (UP, DOWN); INACTIVE / UNSELECTED stop observing the list and every scrolling child (each child
// also gets OnObserverRemove); SELECTED observes the list, every scrolling child (MOUSE_MOVEMENT) and the selected child
// (DELETE, SELECT); LIST_ACTIVE recomputes the offsets once when flagged, sets every non-scrolling child to CUSTOM + the
// selection's index among Children when it has that state (else ACTIVE), and observes like SELECTED after the base state;
// LIST_INACTIVE drops the observers and forwards INACTIVE when the current state is INACTIVE, else LIST_INACTIVE; any other
// state goes straight to the base. Every path ends in exactly one base ChangeState with the expected argument.
#include <cstdio>
#include <vector>
#include <set>
enum { S_INACTIVE = 0, S_ACTIVE = 1, S_DEAD = 2, S_SELECTED = 3, S_UNSELECTED = 4, S_LIST_ACTIVE = 5, S_LIST_INACTIVE = 6, S_CUSTOM = 8 };
enum { E_UP = 0, E_DOWN = 1, E_SELECT = 4, E_DELETE = 22, E_MOUSE = 25 };
struct Node {
    std::set<int> events; bool observed; int removedCalls; int state; std::set<unsigned long> states;
    Node() : observed(false), removedCalls(0), state(-1) { }
    bool HasState(unsigned long s) { return states.count(s) != 0; }
};
struct FrontEndList : Node {
    std::vector<Node*> Children, Scrolling, NonScrolling;
    long Selected; bool Recompute; int recomputes; unsigned long CurrentState; int baseState; int baseCalls;
    FrontEndList() : Selected(0), Recompute(false), recomputes(0), CurrentState(1), baseState(-1), baseCalls(0) { }
    void Base(unsigned long s) { baseState = (int)s; ++baseCalls; }
    void DropAll() { observed = false; for (unsigned i = 0; i < Scrolling.size(); ++i) { Scrolling[i]->observed = false; ++Scrolling[i]->removedCalls; } }
    void ObserveChildren() {
        if (Scrolling.size() > 0) {
            for (unsigned i = 0; i < Scrolling.size(); ++i) { Scrolling[i]->observed = true; Scrolling[i]->events.insert(E_MOUSE); }
            Scrolling[Selected]->events.insert(E_DELETE); Scrolling[Selected]->events.insert(E_SELECT);
        }
    }
    void ChangeState(unsigned long state) {
        switch (state) {
        case S_ACTIVE: observed = true; events.insert(E_UP); events.insert(E_DOWN); Base(S_ACTIVE); break;
        case S_INACTIVE: DropAll(); Base(S_INACTIVE); break;
        case S_UNSELECTED: DropAll(); Base(S_UNSELECTED); break;
        case S_LIST_ACTIVE:
            if (Recompute) { ++recomputes; Recompute = false; }
            if (Scrolling.size() > 0) {
                bool found = false;
                for (unsigned long j = 0; j < Children.size(); ++j)
                    if (Children[j] == Scrolling[Selected]) {
                        for (unsigned i = 0; i < NonScrolling.size(); ++i)
                            NonScrolling[i]->state = NonScrolling[i]->HasState(j + S_CUSTOM) ? (int)(j + S_CUSTOM) : S_ACTIVE;
                        found = true; break;
                    }
                if (!found) for (unsigned i = 0; i < NonScrolling.size(); ++i) NonScrolling[i]->state = S_ACTIVE;
            }
            Base(S_LIST_ACTIVE); ObserveChildren(); break;
        case S_LIST_INACTIVE: DropAll(); Base(CurrentState == S_INACTIVE ? S_INACTIVE : S_LIST_INACTIVE); break;
        case S_SELECTED: observed = true; events.insert(E_UP); events.insert(E_DOWN); ObserveChildren(); Base(S_SELECTED); break;
        default: Base(state); break;
        }
    }
};
static int Fail(int n) { std::printf("0054cbf0_TEST FAIL %d\n", n); return 1; }
int main()
{
    Node c0, c1, c2, n0, n1;
    FrontEndList l;
    l.Children.push_back(&c0); l.Children.push_back(&c1); l.Children.push_back(&c2); l.Children.push_back(&n0); l.Children.push_back(&n1);
    l.Scrolling.push_back(&c0); l.Scrolling.push_back(&c1); l.Scrolling.push_back(&c2);
    l.NonScrolling.push_back(&n0); l.NonScrolling.push_back(&n1);
    l.ChangeState(S_ACTIVE);
    if (!l.observed || !l.events.count(E_UP) || !l.events.count(E_DOWN) || l.baseState != S_ACTIVE) return Fail(1);
    l.Selected = 1; l.ChangeState(S_SELECTED);
    if (!c0.observed || !c2.observed || !c1.events.count(E_DELETE) || c0.events.count(E_SELECT) || l.baseState != S_SELECTED) return Fail(2);
    l.ChangeState(S_UNSELECTED);
    if (l.observed || c1.observed || c1.removedCalls != 1 || l.baseState != S_UNSELECTED) return Fail(3);
    n0.states.insert(S_CUSTOM + 1);                            // selected child c1 is Children[1]
    l.Recompute = true; l.ChangeState(S_LIST_ACTIVE);
    if (l.recomputes != 1 || l.Recompute || n0.state != S_CUSTOM + 1 || n1.state != S_ACTIVE || !c1.observed || l.baseState != S_LIST_ACTIVE) return Fail(4);
    l.ChangeState(S_LIST_ACTIVE);
    if (l.recomputes != 1) return Fail(5);
    Node stranger; l.Scrolling[1] = &stranger;                  // selection not among Children: everything ACTIVE
    n0.state = -1; l.ChangeState(S_LIST_ACTIVE);
    if (n0.state != S_ACTIVE || n1.state != S_ACTIVE) return Fail(6);
    l.Scrolling[1] = &c1;
    l.CurrentState = S_INACTIVE; l.ChangeState(S_LIST_INACTIVE);
    if (l.baseState != S_INACTIVE || c0.observed) return Fail(7);
    l.CurrentState = S_ACTIVE; l.ChangeState(S_LIST_INACTIVE);
    if (l.baseState != S_LIST_INACTIVE) return Fail(8);
    l.ChangeState(S_INACTIVE);
    if (l.baseState != S_INACTIVE || c0.removedCalls < 3) return Fail(9);
    int before = l.baseCalls; l.ChangeState(S_DEAD); l.ChangeState(7);
    if (l.baseState != 7 || l.baseCalls != before + 2) return Fail(10);
    std::printf("0054cbf0_TEST PASS\n");
    return 0;
}