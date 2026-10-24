// Behaviour test for 0x006569DA deque<pair<CCharString,long>>::_M_push_back_aux_v (model copy, 8-byte elements, 16 per block)
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct Elem { int text; long time; Elem(int t, long v) : text(t), time(v) { ++g_live; } Elem(const Elem& o) : text(o.text), time(o.time) { ++g_live; } ~Elem() { --g_live; } };
struct Iter {
    Elem* cur; Elem* first; Elem* last; Elem** node;
    void set_node(Elem** n) { last = (first = *(node = n)) + 16; }
};
struct Deque {
    Iter start; Iter finish; Elem** map; unsigned int map_size; int reserved;
    void reserve_map_at_back() { ++reserved; }
    void push_back_aux(const Elem& t)
    {
        Elem t_copy = t;
        reserve_map_at_back();
        *(finish.node + 1) = (Elem*)malloc(16 * sizeof(Elem));
        if (finish.cur) new (finish.cur) Elem(t_copy);
        finish.set_node(finish.node + 1);
        finish.cur = finish.first;
    }
};
int main()
{
    Elem* map[4] = { 0, 0, 0, 0 };
    map[1] = (Elem*)malloc(16 * sizeof(Elem));
    Deque d; d.map = map; d.map_size = 4; d.reserved = 0;
    d.finish.set_node(map + 1); d.finish.cur = d.finish.last - 1;      // one slot left: the aux path
    d.start = d.finish;
    d.push_back_aux(Elem(5, 600));
    if (d.reserved != 1 || map[2] == 0 || d.finish.node != map + 2 || d.finish.cur != map[2] || d.finish.last != map[2] + 16 ||
        map[1][15].text != 5 || map[1][15].time != 600 || g_live != 1) { std::printf("006569da_TEST FAIL\n"); return 1; }
    std::printf("006569da_TEST PASS\n");
    return 0;
}