// Behaviour test for 0x006DB340 list<pair<EDamageAttribute,long> >::_M_fill_insert: n copies of x are linked in
// before pos (n == 0 inserts nothing), keeping the ring consistent.
#include <cstdio>
#include <cstdlib>

struct Pair { long first; long second; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { Pair data; };

static void FillInsert(NodeBase* pos, unsigned int n, const Pair& x)
{
    for (; n > 0; --n) {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        tmp->data = x;
        NodeBase* prev = pos->prev;
        tmp->next = pos; tmp->prev = prev; prev->next = tmp; pos->prev = tmp;
    }
}

int main()
{
    NodeBase head; head.next = head.prev = &head;
    Pair a = { -1, 0 };
    FillInsert(&head, 3, a);
    Pair b = { 7, 9 };
    FillInsert(head.next, 1, b);                       // before the first element
    FillInsert(&head, 0, b);
    int count = 0; long firstKey = ((Node*)head.next)->data.first;
    for (NodeBase* p = head.next; p != &head; p = p->next) { if (p->next->prev != p) { std::printf("006db340_TEST FAIL\n"); return 1; } ++count; }
    if (count != 4 || firstKey != 7 || ((Node*)head.prev)->data.first != -1) { std::printf("006db340_TEST FAIL\n"); return 1; }
    while (head.next != &head) { NodeBase* n = head.next; head.next = n->next; std::free(n); }
    std::printf("006db340_TEST PASS\n");
    return 0;
}