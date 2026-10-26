// behaviour test for 0x0065E6B8 std::__find over list<CIntelligentPointer<CThingAICreature> > with a const CThing* key: the first node whose pointee equals the key is returned, a missing key returns last, and a dead pointer (null pointee) never matches a live key
#include <cstdio>
struct Obj { int Id; };
struct Node { Node* next; Node* prev; Obj* pointee; };
static Node* Find(Node* first, Node* last, Obj* const& val)     // the function under test (STLport 4.6 shape)
{
    while (first != last && !(first->pointee == val))
        first = first->next;
    return first;
}
int main()
{
    int fail = 0;
    Obj a, b, c, d; a.Id = 1; b.Id = 2; c.Id = 3; d.Id = 4;
    Node n[5];                                   // n[4] is the sentinel
    Obj* items[4] = { &a, 0, &b, &c };
    for (int i = 0; i < 5; ++i) { n[i].next = &n[(i + 1) % 5]; n[i].prev = &n[(i + 4) % 5]; n[i].pointee = i < 4 ? items[i] : 0; }
    Obj* key = &b;
    if (Find(&n[0], &n[4], key) != &n[2]) fail |= 1;
    key = &a;
    if (Find(&n[0], &n[4], key) != &n[0]) fail |= 2;
    key = &d;
    if (Find(&n[0], &n[4], key) != &n[4]) fail |= 4;
    key = &c;
    if (Find(&n[2], &n[3], key) != &n[3]) fail |= 8;          // last itself is never examined
    if (fail) { std::printf("0065e6b8_TEST FAIL %d\n", fail); return 1; }
    std::printf("0065e6b8_TEST PASS\n");
    return 0;
}