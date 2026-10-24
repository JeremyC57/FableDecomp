// Behaviour test for 0x0059AC9F std::_Deque_base<CComponent*>::_M_initialize_map (model copy, 4-byte elements)
#include <cstdio>
#include <cstdlib>
#include <stddef.h>
inline size_t buf_size(size_t n) { return n < 128 ? (128 / n) : 1; }
template <class T> inline const T& tmax(const T& a, const T& b) { return a < b ? b : a; }
struct Iter {
    unsigned int* cur; unsigned int* first; unsigned int* last; unsigned int** node;
    void set_node(unsigned int** n) { last = (first = *(node = n)) + ptrdiff_t(buf_size(sizeof(unsigned int))); }
};
struct DequeBase {
    Iter start; Iter finish; unsigned int** map; size_t map_size; int created;
    void create_nodes(unsigned int** s, unsigned int** f) { for (unsigned int** c = s; c < f; ++c) { *c = (unsigned int*)malloc(128); ++created; } }
    void initialize_map(size_t n)
    {
        size_t num_nodes = n / buf_size(sizeof(unsigned int)) + 1;
        map_size = tmax((size_t)8, num_nodes + 2);
        map = map_size != 0 ? (unsigned int**)malloc(map_size * sizeof(unsigned int*)) : 0;
        unsigned int** nstart = map + (map_size - num_nodes) / 2;
        unsigned int** nfinish = nstart + num_nodes;
        create_nodes(nstart, nfinish);
        start.set_node(nstart);
        finish.set_node(nfinish - 1);
        start.cur = start.first;
        finish.cur = finish.first + n % buf_size(sizeof(unsigned int));
    }
};
int main()
{
    DequeBase d; d.created = 0;
    d.initialize_map(0);
    if (d.map_size != 8 || d.created != 1 || d.start.node != d.map + 3 || d.finish.node != d.map + 3 ||
        d.start.cur != d.start.first || d.finish.cur != d.finish.first || d.start.last != d.start.first + 32) { std::printf("0059ac9f_TEST FAIL\n"); return 1; }
    DequeBase e; e.created = 0;
    e.initialize_map(300);          // 300 / 32 + 1 = 10 nodes, map 12
    if (e.map_size != 12 || e.created != 10 || e.start.node != e.map + 1 || e.finish.node != e.map + 10 ||
        e.finish.cur != e.finish.first + (300 % 32)) { std::printf("0059ac9f_TEST FAIL\n"); return 1; }
    std::printf("0059ac9f_TEST PASS\n");
    return 0;
}