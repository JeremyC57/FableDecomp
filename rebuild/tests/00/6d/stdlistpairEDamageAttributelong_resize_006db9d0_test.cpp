// Behaviour test for 0x006DB9D0 list<pair<EDamageAttribute,long> >::resize: a longer list is cut to new_size (the
// surplus nodes unlinked and freed), a shorter one is padded at the end with copies of x, an equal one is unchanged.
#include <cstdio>
#include <cstdlib>

struct Pair { long first; long second; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { Pair data; };

struct List {
    NodeBase* head;
    void FillInsert(NodeBase* pos, unsigned int n, const Pair& x)
    {
        for (; n > 0; --n) {
            Node* tmp = (Node*)std::malloc(sizeof(Node)); tmp->data = x;
            NodeBase* prev = pos->prev; tmp->next = pos; tmp->prev = prev; prev->next = tmp; pos->prev = tmp;
        }
    }
    void resize(unsigned int newSize, const Pair& x)
    {
        NodeBase* i = head->next; unsigned int len = 0;
        for (; i != head && len < newSize; i = i->next, ++len);
        if (len == newSize) {
            while (i != head) { NodeBase* n = i; i = i->next; n->prev->next = n->next; n->next->prev = n->prev; std::free(n); }
        } else
            FillInsert(head, newSize - len, x);
    }
    unsigned int size() const { unsigned int n = 0; for (NodeBase* p = head->next; p != head; p = p->next) ++n; return n; }
};

int main()
{
    NodeBase sentinel; sentinel.next = sentinel.prev = &sentinel;
    List l; l.head = &sentinel;
    Pair unset = { -1, 0 };
    l.resize(10, unset);
    if (l.size() != 10 || ((Node*)sentinel.prev)->data.first != -1) { std::printf("006db9d0_TEST FAIL\n"); return 1; }
    ((Node*)sentinel.next)->data.second = 42;
    l.resize(3, unset);
    if (l.size() != 3 || ((Node*)sentinel.next)->data.second != 42) { std::printf("006db9d0_TEST FAIL\n"); return 1; }
    l.resize(3, unset);
    if (l.size() != 3) { std::printf("006db9d0_TEST FAIL\n"); return 1; }
    l.resize(0, unset);
    if (l.size() != 0 || sentinel.next != &sentinel || sentinel.prev != &sentinel) { std::printf("006db9d0_TEST FAIL\n"); return 1; }
    std::printf("006db9d0_TEST PASS\n");
    return 0;
}