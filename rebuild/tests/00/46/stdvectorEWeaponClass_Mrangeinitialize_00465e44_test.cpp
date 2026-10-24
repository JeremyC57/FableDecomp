// Behaviour test for 0x00465E44 std::vector<EWeaponClass>::_M_range_initialize(first, last, forward_iterator_tag)
#include <cstdio>
#include <cstdlib>
#include <new>
struct Node { Node* next; int value; };            // stands in for the red-black node walk (_M_increment)
static Node* Increment(Node* n) { return n->next; }
struct IntVector {
    int* start; int* finish; int* eos;
    void RangeInitialize(Node* first, Node* last)
    {
        unsigned int n = 0;
        for (Node* it = first; it != last; it = Increment(it)) ++n;
        start = n ? static_cast<int*>(std::malloc(n * sizeof(int))) : 0;
        eos = start + n;
        int* cur = start;
        for (; first != last; first = Increment(first), ++cur)
            if (cur) new (cur) int(first->value);
        finish = cur;
    }
};
int main()
{
    Node header; Node a, b, c;
    a.value = 3; b.value = 5; c.value = 9;
    a.next = &b; b.next = &c; c.next = &header;
    IntVector v; v.RangeInitialize(&a, &header);
    if (v.finish - v.start != 3 || v.eos != v.finish || v.start[0] != 3 || v.start[1] != 5 || v.start[2] != 9) { std::printf("00465e44_TEST FAIL\n"); return 1; }
    std::free(v.start);
    IntVector e; e.RangeInitialize(&header, &header);
    if (e.start != 0 || e.finish != 0) { std::printf("00465e44_TEST FAIL\n"); return 1; }
    std::printf("00465e44_TEST PASS\n");
    return 0;
}