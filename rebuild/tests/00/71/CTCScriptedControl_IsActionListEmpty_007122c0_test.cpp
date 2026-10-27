// Behaviour test for CTCScriptedControl::IsActionListEmpty (0x007122C0): the action list is empty when the sentinel links to itself.
#include <cstdio>
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct List { NodeBase* node; bool empty() const { return node->next == node; } };
class CTCScriptedControl { public: List PActionList; bool IsActionListEmpty() const { return PActionList.empty(); } };
int main()
{
    NodeBase head, a;
    head.next = &head; head.prev = &head;
    CTCScriptedControl c; c.PActionList.node = &head;
    if (!c.IsActionListEmpty()) { std::printf("007122c0_TEST FAIL\n"); return 1; }
    head.next = &a; a.next = &head; a.prev = &head; head.prev = &a;
    if (c.IsActionListEmpty()) { std::printf("007122c0_TEST FAIL\n"); return 1; }
    std::printf("007122c0_TEST PASS\n");
    return 0;
}