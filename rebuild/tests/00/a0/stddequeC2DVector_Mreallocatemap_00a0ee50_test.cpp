// Behaviour test for 0x00A0EE50 deque<C2DVector>::_M_reallocate_map (STLport node-map grow / recentre).
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef float* Node;
struct Iter { float* cur; float* first; float* last; Node* node;
    void set_node(Node* n) { node = n; first = *n; last = first + 32; } };
struct Deque {
    Iter start, finish; Node* map; unsigned int map_size;
    void reallocate_map(unsigned int nodes_to_add, bool add_at_front)
    {
        unsigned int old_num = (unsigned int)(finish.node - start.node) + 1;
        unsigned int new_num = old_num + nodes_to_add;
        Node* new_nstart;
        if (map_size > 2 * new_num) {
            new_nstart = map + (map_size - new_num) / 2 + (add_at_front ? nodes_to_add : 0);
            std::memmove(new_nstart, start.node, old_num * sizeof(Node));
        } else {
            unsigned int new_map_size = map_size + (map_size < nodes_to_add ? nodes_to_add : map_size) + 2;
            Node* new_map = (Node*)std::malloc(new_map_size * sizeof(Node));
            new_nstart = new_map + (new_map_size - new_num) / 2 + (add_at_front ? nodes_to_add : 0);
            std::memmove(new_nstart, start.node, old_num * sizeof(Node));
            std::free(map);
            map = new_map; map_size = new_map_size;
        }
        start.set_node(new_nstart);
        finish.set_node(new_nstart + old_num - 1);
    }
};

int main()
{
    int ok = 1;
    float bufA[32], bufB[32];
    Deque d;
    d.map_size = 3;
    d.map = (Node*)std::malloc(3 * sizeof(Node));
    d.map[0] = 0; d.map[1] = bufA; d.map[2] = bufB;
    d.start.set_node(d.map + 1); d.finish.set_node(d.map + 2);
    d.reallocate_map(1, false);                          // 3 <= 2 * 3: new map 3 + max(3, 1) + 2 = 8
    if (d.map_size != 8 || d.start.node != d.map + 2 || d.finish.node != d.map + 3 || *d.start.node != bufA ||
        *d.finish.node != bufB || d.start.first != bufA || d.finish.last != bufB + 32) ok = 0;
    d.reallocate_map(1, true);                           // 8 > 2 * 3: recentre with one free slot in front
    if (d.map_size != 8 || d.start.node != d.map + 3 || *d.start.node != bufA || *d.finish.node != bufB) ok = 0;
    std::free(d.map);
    if (!ok) { std::printf("00a0ee50_TEST FAIL\n"); return 1; }
    std::printf("00a0ee50_TEST PASS\n");
    return 0;
}