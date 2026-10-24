#include <cstdio>
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct CStateList {
    _List_node_base* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
struct CChangingStateComponent {
    unsigned long StatesBeingDone, StatesDone;
    CStateList StatesToDo;
    bool ChangedState() { return StatesToDo.empty() && (StatesDone & StatesBeingDone) == StatesBeingDone; }
};
int main()
{
    _List_node_base head, node;
    head._M_next = &head; head._M_prev = &head;
    CChangingStateComponent c;
    c.StatesToDo._M_node = &head;
    c.StatesBeingDone = 0x5; c.StatesDone = 0x7;
    if (!c.ChangedState()) { std::printf("0052c8f0_TEST FAIL\n"); return 1; }
    c.StatesDone = 0x4;
    if (c.ChangedState()) { std::printf("0052c8f0_TEST FAIL\n"); return 1; }
    c.StatesDone = 0x5;
    head._M_next = &node; node._M_next = &head;
    if (c.ChangedState()) { std::printf("0052c8f0_TEST FAIL\n"); return 1; }
    std::printf("0052c8f0_TEST PASS\n");
    return 0;
}