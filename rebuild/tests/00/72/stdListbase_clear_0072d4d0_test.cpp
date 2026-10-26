// behaviour test for 0x0072D4D0 std::_List_base::clear() [list<<CTCSpecialAbilities::CUndeadFollower>>]: every element is destroyed once and its node freed, the walk starts at the sentinel's next and stops at the sentinel, and the sentinel ends self-linked
#include <cstdio>
#include <cstdlib>
static int gDestroyed = 0, gFreed = 0;
struct Elem { int Tag; ~Elem() { ++gDestroyed; } };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { Elem data; };
struct List {
    NodeBase* sentinel;
    void Clear()                                       // the function under test (STLport 4.6 _List_base::clear)
    {
        Node* cur = (Node*)sentinel->next;
        while (cur != sentinel) {
            Node* tmp = cur;
            cur = (Node*)cur->next;
            tmp->data.~Elem();
            ++gFreed; std::free(tmp);
        }
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    }
};
int main()
{
    int fail = 0;
    NodeBase s; s.next = &s; s.prev = &s;
    List l; l.sentinel = &s;
    l.Clear();
    if (gDestroyed != 0 || gFreed != 0 || s.next != &s || s.prev != &s) fail |= 1;
    NodeBase* last = &s;
    for (int i = 0; i < 3; ++i) {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->data.Tag = i; n->prev = last; n->next = &s; last->next = n; s.prev = n; last = n;
    }
    l.Clear();
    if (gDestroyed != 3 || gFreed != 3 || s.next != &s || s.prev != &s) fail |= 2;
    if (fail) { std::printf("0072d4d0_TEST FAIL %d\n", fail); return 1; }
    std::printf("0072d4d0_TEST PASS\n");
    return 0;
}