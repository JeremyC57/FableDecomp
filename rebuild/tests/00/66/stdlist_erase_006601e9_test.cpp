// behaviour test for 0x006601E9 list<CCountedPointer<CVillageTaskCarryCrate> >::erase(iterator): the node is unlinked from both neighbours, its element released exactly once, and the iterator to the following node returned
#include <cstdio>
struct Node { Node* next; Node* prev; int item; };
static int released = 0, lastReleased = -1;
static Node* Erase(Node* position)                       // the function under test (STLport 4.6 shape)
{
    Node* next = position->next;
    Node* prev = position->prev;
    prev->next = next;
    next->prev = prev;
    ++released; lastReleased = position->item;           // ~CCountedPointer
    return next;                                         // (free of the node omitted: the nodes live on the stack)
}
int main()
{
    int fail = 0;
    Node n[4];                                           // n[3] is the sentinel
    for (int i = 0; i < 4; ++i) { n[i].next = &n[(i + 1) % 4]; n[i].prev = &n[(i + 3) % 4]; n[i].item = 10 + i; }
    if (Erase(&n[1]) != &n[2]) fail |= 1;
    if (n[0].next != &n[2] || n[2].prev != &n[0]) fail |= 2;
    if (released != 1 || lastReleased != 11) fail |= 4;
    if (Erase(&n[2]) != &n[3]) fail |= 8;                // the last element: returns end()
    if (n[0].next != &n[3] || n[3].prev != &n[0]) fail |= 16;
    if (Erase(&n[0]) != &n[3] || n[3].next != &n[3] || n[3].prev != &n[3]) fail |= 32;   // now empty
    if (released != 3) fail |= 64;
    if (fail) { std::printf("006601e9_TEST FAIL %d\n", fail); return 1; }
    std::printf("006601e9_TEST PASS\n");
    return 0;
}