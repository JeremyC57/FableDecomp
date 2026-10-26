// behaviour test for 0x0065DF94 list<CIntelligentPointer<CThingTrackNode > >::remove: every node equal to the value is erased, the others keep their order
#include <cstdio>
struct Node { Node* next; Node* prev; int value; };
static int gErased = 0;
struct List {
    Node head;
    Node* erase(Node* n) { ++gErased; Node* nx = n->next; n->prev->next = nx; nx->prev = n->prev; return nx; }
    void remove(int val)                                       // the function under test
    {
        Node* first = head.next; Node* last = &head;
        while (first != last) { Node* next = first->next; if (val == first->value) erase(first); first = next; }
    }
};
int main()
{
    int fail = 0, i;
    Node n[5]; int v[5] = { 0, 3, 0, 0, 4 };
    List l; l.head.next = &n[0]; l.head.prev = &n[4];
    for (i = 0; i < 5; ++i) { n[i].value = v[i]; n[i].next = i < 4 ? &n[i + 1] : &l.head; n[i].prev = i > 0 ? &n[i - 1] : &l.head; }
    l.remove(0);
    if (gErased != 3) fail |= 1;
    if (l.head.next != &n[1] || n[1].next != &n[4] || n[4].next != &l.head || n[4].prev != &n[1]) fail |= 2;
    l.remove(0);
    if (gErased != 3) fail |= 4;
    if (fail) { std::printf("0065df94_TEST FAIL %d\n", fail); return 1; }
    std::printf("0065df94_TEST PASS\n");
    return 0;
}