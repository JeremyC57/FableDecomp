// Behaviour test for NUISystem::CChangingStateComponent::~CChangingStateComponent (0x0052CCF0): the StatesToDo list is
// destroyed (all nodes freed) before the CComponent base destructor runs.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
struct Node { Node* next; Node* prev; int value; };
struct StateList {
    Node* head;
    StateList() { head = new Node; head->next = head; head->prev = head; }
    void push_back(int v) { Node* n = new Node; n->value = v; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    ~StateList() {
        Node* cur = head->next; int freed = 0;
        while (cur != head) { Node* t = cur; cur = cur->next; delete t; ++freed; }
        delete head; g_log[g_n++] = 100 + freed;
    }
};
struct Component { virtual ~Component() { g_log[g_n++] = 1; } };
struct ChangingStateComponent : Component {
    long StatesBeingDone, StatesDone;
    StateList StatesToDo;
    virtual ~ChangingStateComponent() { }
};
int main()
{
    {
        ChangingStateComponent c; c.StatesToDo.push_back(3); c.StatesToDo.push_back(4);
    }
    if (g_n != 2 || g_log[0] != 102 || g_log[1] != 1) { std::printf("0052ccf0_TEST FAIL\n"); return 1; }
    std::printf("0052ccf0_TEST PASS\n");
    return 0;
}