// behaviour test for 0x0076D5D0: __find returns the first node holding the value, or last
#include <cstdio>
struct Node { Node* next; Node* prev; void* item; };
static Node* Find(Node* first, Node* last, void* const& val) { while (first != last && !(first->item == val)) first = first->next; return first; }
int main()
{
    int a, b, c; Node n[4];
    for (int i = 0; i < 4; ++i) { n[i].next = &n[(i + 1) % 4]; n[i].prev = &n[(i + 3) % 4]; }
    n[0].item = 0; n[1].item = &a; n[2].item = &b; n[3].item = &a;
    void* pa = &a; void* pb = &b; void* pc = &c;
    if (Find(&n[1], &n[0], pa) != &n[1] || Find(&n[1], &n[0], pb) != &n[2] || Find(&n[1], &n[0], pc) != &n[0] || Find(&n[2], &n[2], pb) != &n[2])
    { std::printf("0076d5d0_TEST FAIL\n"); return 1; }
    std::printf("0076d5d0_TEST PASS\n");
    return 0;
}