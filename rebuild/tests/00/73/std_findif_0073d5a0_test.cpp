// Behaviour test for std::__find_if<list<CCountedPointer<CModeBase>>::iterator, CModeBase::CIsType> (0x0073D5A0): first mode whose virtual GetType equals the predicate type.
#include <cstdio>
class CModeBase { public:
    unsigned int T;
    virtual ~CModeBase() {}
    virtual unsigned int GetType() const { return T; }
};
struct CCountedPointer { CModeBase* Data; void* Info; };
struct CIsType { unsigned int Type; bool operator()(const CCountedPointer& m) const { return m.Data->GetType() == Type; } };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CCountedPointer data; };
static NodeBase* find_if_type(NodeBase* first, NodeBase* last, CIsType pred)
{
    while (first != last && !pred(((Node*)first)->data)) first = first->next;
    return first;
}
int main()
{
    CModeBase m[3]; m[0].T = 4; m[1].T = 9; m[2].T = 9;
    Node head, n[3];
    head.next = &n[0]; n[0].next = &n[1]; n[1].next = &n[2]; n[2].next = &head;
    for (int i = 0; i < 3; ++i) n[i].data.Data = &m[i];
    CIsType p9; p9.Type = 9; CIsType p5; p5.Type = 5;
    if (find_if_type(head.next, &head, p9) != &n[1]) { std::printf("0073d5a0_TEST FAIL\n"); return 1; }
    if (find_if_type(head.next, &head, p5) != &head) { std::printf("0073d5a0_TEST FAIL\n"); return 1; }
    std::printf("0073d5a0_TEST PASS\n");
    return 0;
}