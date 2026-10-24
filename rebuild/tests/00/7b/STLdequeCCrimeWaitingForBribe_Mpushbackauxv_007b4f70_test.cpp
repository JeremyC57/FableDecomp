// Behaviour test for 0x007B4F70 deque<CCrimeWaitingForBribe>::_M_push_back_aux_v: reserve a map slot, allocate the next
// node, construct at the old cursor and move the finish iterator to the start of the new node.
#include <cstdio>
#include <cstdlib>
struct Rec { int a, b; };
static int g_realloc = 0;
struct It {
    Rec* cur; Rec* first; Rec* last; Rec** node;
    void set_node(Rec** n) { node = n; first = *n; last = first + 16; }
};
struct Deque {
    It start, finish; Rec** map; unsigned int map_size;
    void reallocate_map(unsigned int add, bool front) { if (add == 1 && !front) ++g_realloc; }
    void reserve_map_at_back() { if (2u > map_size - (unsigned int)(finish.node - map)) reallocate_map(1, false); }
    void push_back_aux(const Rec& t)
    {
        Rec copy = t;
        reserve_map_at_back();
        *(finish.node + 1) = (Rec*)std::malloc(0x80);
        *finish.cur = copy;
        finish.set_node(finish.node + 1);
        finish.cur = finish.first;
    }
};
int main()
{
    Rec* slots[4]; Rec first[16];
    slots[0] = first; slots[1] = slots[2] = slots[3] = 0;
    Deque d; d.map = slots; d.map_size = 4;
    d.finish.node = &slots[0]; d.finish.first = first; d.finish.last = first + 16; d.finish.cur = first + 15;
    Rec r = { 9, 10 };
    d.push_back_aux(r);
    if (first[15].a != 9 || d.finish.node != &slots[1] || slots[1] == 0 || d.finish.cur != slots[1] || d.finish.last != slots[1] + 16 || g_realloc != 0) { std::printf("007b4f70_TEST FAIL\n"); return 1; }
    // node 2 of 4: 4 - 2 = 2 < 2 is false -> no reallocation; at node 3: 4 - 3 = 1 -> reallocate
    d.finish.node = &slots[2]; d.finish.cur = first; d.push_back_aux(r);
    if (g_realloc != 0) { std::printf("007b4f70_TEST FAIL\n"); return 1; }
    std::free(slots[1]); std::free(slots[3]);
    std::printf("007b4f70_TEST PASS\n");
    return 0;
}