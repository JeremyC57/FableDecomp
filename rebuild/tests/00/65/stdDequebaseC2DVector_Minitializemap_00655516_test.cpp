// Behaviour test for 0x00655516 std::_Deque_base<C2DVector>::_M_initialize_map (model copy, 8-byte elements)
#include <cstdio>
#include <cstdlib>
#include <stddef.h>
struct Vec2 { float X, Y; };
inline size_t buf_size(size_t n) { return n < 128 ? (128 / n) : 1; }
template <class T> inline const T& tmax(const T& a, const T& b) { return a < b ? b : a; }
struct Iter {
    Vec2* cur; Vec2* first; Vec2* last; Vec2** node;
    void set_node(Vec2** n) { last = (first = *(node = n)) + ptrdiff_t(buf_size(sizeof(Vec2))); }
};
struct DequeBase {
    Iter start; Iter finish; Vec2** map; size_t map_size; int created;
    void create_nodes(Vec2** s, Vec2** f) { for (Vec2** c = s; c < f; ++c) { *c = (Vec2*)malloc(128); ++created; } }
    void initialize_map(size_t n)
    {
        size_t num_nodes = n / buf_size(sizeof(Vec2)) + 1;
        map_size = tmax((size_t)8, num_nodes + 2);
        map = map_size != 0 ? (Vec2**)malloc(map_size * sizeof(Vec2*)) : 0;
        Vec2** nstart = map + (map_size - num_nodes) / 2;
        Vec2** nfinish = nstart + num_nodes;
        create_nodes(nstart, nfinish);
        start.set_node(nstart);
        finish.set_node(nfinish - 1);
        start.cur = start.first;
        finish.cur = finish.first + n % buf_size(sizeof(Vec2));
    }
};
int main()
{
    if (buf_size(sizeof(Vec2)) != 16) { std::printf("00655516_TEST FAIL\n"); return 1; }
    DequeBase d; d.created = 0;
    d.initialize_map(5);            // 5 / 16 + 1 = 1 node, map 8
    if (d.map_size != 8 || d.created != 1 || d.start.node != d.map + 3 || d.finish.node != d.map + 3 ||
        d.start.cur != d.start.first || d.finish.cur != d.finish.first + 5 || d.start.last != d.start.first + 16) { std::printf("00655516_TEST FAIL\n"); return 1; }
    DequeBase e; e.created = 0;
    e.initialize_map(200);          // 200 / 16 + 1 = 13 nodes, map 15
    if (e.map_size != 15 || e.created != 13 || e.start.node != e.map + 1 || e.finish.node != e.map + 13 ||
        e.finish.cur != e.finish.first + (200 % 16)) { std::printf("00655516_TEST FAIL\n"); return 1; }
    std::printf("00655516_TEST PASS\n");
    return 0;
}