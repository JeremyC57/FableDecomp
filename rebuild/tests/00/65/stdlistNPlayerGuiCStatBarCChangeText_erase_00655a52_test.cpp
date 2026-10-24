// Behaviour test for 0x00655A52 std::list<CStatBar::CChangeText>::erase (model copy)
#include <cstdio>
#include <cstdlib>
static int g_destroyed = 0;
struct Text { void* p; ~Text() { ++g_destroyed; } };
struct ChangeText { float a, b, c, d; Text T; long Value; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { ChangeText data; };
struct Iter { NodeBase* node; Iter(NodeBase* n) : node(n) { } Iter(const Iter& o) : node(o.node) { } };
struct List {
    NodeBase* head;
    Iter erase(Iter pos)
    {
        NodeBase* next = pos.node->next;
        NodeBase* prev = pos.node->prev;
        Node* n = (Node*)pos.node;
        prev->next = next;
        next->prev = prev;
        n->data.~ChangeText();
        if (n != 0) free(n);
        return Iter(next);
    }
};
int main()
{
    NodeBase sentinel;
    Node* a = (Node*)malloc(sizeof(Node)); Node* b = (Node*)malloc(sizeof(Node));
    a->data.Value = 1; b->data.Value = 2;
    sentinel.next = a; a->prev = &sentinel; a->next = b; b->prev = a; b->next = &sentinel; sentinel.prev = b;
    List l; l.head = &sentinel;
    Iter it = l.erase(Iter(a));
    if (it.node != b || sentinel.next != b || b->prev != &sentinel || g_destroyed != 1) { std::printf("00655a52_TEST FAIL\n"); return 1; }
    it = l.erase(it);
    if (it.node != &sentinel || sentinel.next != &sentinel || sentinel.prev != &sentinel || g_destroyed != 2) { std::printf("00655a52_TEST FAIL\n"); return 1; }
    std::printf("00655a52_TEST PASS\n");
    return 0;
}