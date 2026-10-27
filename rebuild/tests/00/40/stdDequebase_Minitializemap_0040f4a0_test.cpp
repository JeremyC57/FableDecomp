// behaviour test for 0x0040F4A0: _M_initialize_map sizes the map to max(8, nodes + 2) and centres the nodes
#include <cstdio>
#include <cstdlib>
static const unsigned int BUF = 128 / 4;
struct It { char* cur; char* first; char* last; char** node; };
struct D { It start, finish; char** map; unsigned int map_size;
    void Init(unsigned int n) {
        unsigned int nodes = n / BUF + 1;
        map_size = nodes + 2 > 8 ? nodes + 2 : 8;
        map = (char**)std::malloc(map_size * sizeof(char*));
        char** ns = map + (map_size - nodes) / 2; char** nf = ns + nodes;
        for (char** c = ns; c < nf; ++c) *c = (char*)std::malloc(128);
        start.node = ns; start.first = *ns; start.last = start.first + BUF * 4;
        finish.node = nf - 1; finish.first = *(nf - 1); finish.last = finish.first + BUF * 4;
        start.cur = start.first; finish.cur = finish.first + (n % BUF) * 4; } };
int main()
{
    D d; d.Init(0);
    int ok = d.map_size == 8 && d.start.node == d.map + 3 && d.finish.node == d.start.node && d.finish.cur == d.start.cur;
    D e; e.Init(BUF * 9 + 1);
    ok = ok && e.map_size == 12 && e.start.node == e.map + 1 && e.finish.node == e.map + 10 && e.finish.cur == e.finish.first + 4;
    if (!ok) { std::printf("0040f4a0_TEST FAIL\n"); return 1; }
    std::printf("0040f4a0_TEST PASS\n");
    return 0;
}