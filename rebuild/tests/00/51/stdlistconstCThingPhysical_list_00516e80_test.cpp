// behaviour test for 0x00516E80: a new list's sentinel points at itself both ways
#include <cstdio>
#include <cstdlib>
struct Node { Node* next; Node* prev; unsigned char value[4]; };
struct L { Node* node; L() : node(0) { node = (Node*)std::malloc(sizeof(Node)); node->next = node; node->prev = node; } };
int main()
{
    L l;
    if (!l.node || l.node->next != l.node || l.node->prev != l.node) { std::printf("00516e80_TEST FAIL\n"); return 1; }
    std::free(l.node);
    std::printf("00516e80_TEST PASS\n");
    return 0;
}