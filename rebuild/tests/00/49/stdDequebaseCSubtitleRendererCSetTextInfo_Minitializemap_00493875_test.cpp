// Behaviour test for 0x00493875 _Deque_base<CSetTextInfo>::_M_initialize_map (model with a node-creation stand-in).
#include <cstdio>
#include <cstdlib>

struct Rec { unsigned char bytes[0x1c]; };
struct Iter { Rec* cur; Rec* first; Rec* last; Rec** node;
    void set_node(Rec** n) { last = (first = *(node = n)) + 4; } };
struct DequeBase {
    Iter start, finish;
    Rec** map;
    unsigned int map_size;
    void create_nodes(Rec** s, Rec** f) { for (Rec** p = s; p < f; ++p) *p = (Rec*)std::malloc(4 * sizeof(Rec)); }
    void initialize_map(unsigned int n)
    {
        unsigned int nodes = n / 4 + 1;
        unsigned int a = 8, b = nodes + 2;
        map_size = a < b ? b : a;
        map = (Rec**)std::malloc(map_size * sizeof(Rec*));
        Rec** ns = map + (map_size - nodes) / 2;
        Rec** nf = ns + nodes;
        create_nodes(ns, nf);
        start.set_node(ns);
        finish.set_node(nf - 1);
        start.cur = start.first;
        finish.cur = finish.first + n % 4;
    }
    void release() { for (Rec** p = start.node; p <= finish.node; ++p) std::free(*p); std::free(map); }
};

int main()
{
    DequeBase d;
    d.initialize_map(0);
    int ok = d.map_size == 8 && d.start.node == d.map + 3 && d.finish.node == d.start.node && d.start.cur == d.finish.cur;
    d.release();
    d.initialize_map(30);                    // 8 nodes -> map of 10, nodes at [1..8], finish cursor 2 into the last node
    ok = ok && d.map_size == 10 && d.start.node == d.map + 1 && d.finish.node == d.map + 8
            && d.finish.cur == d.finish.first + 2 && d.start.last - d.start.first == 4;
    d.release();
    if (!ok) { std::printf("00493875_TEST FAIL\n"); return 1; }
    std::printf("00493875_TEST PASS\n");
    return 0;
}