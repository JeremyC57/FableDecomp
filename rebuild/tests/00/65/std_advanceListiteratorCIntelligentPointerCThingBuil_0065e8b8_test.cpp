// Behaviour test for 0x0065E8B8 std::__advance over a list iterator (model copy)
#include <cstdio>
struct Node { Node* next; Node* prev; int v; };
struct It { Node* node; };
void advance_model(It& i, int n)
{
    if (n > 0)
        while (n--) i.node = i.node->next;
    else
        while (n++) i.node = i.node->prev;
}
int main()
{
    Node n[5];
    for (int i = 0; i < 5; ++i) { n[i].v = i; n[i].next = &n[(i + 1) % 5]; n[i].prev = &n[(i + 4) % 5]; }
    It it; it.node = &n[0];
    advance_model(it, 3);
    if (it.node->v != 3) { std::printf("0065e8b8_TEST FAIL\n"); return 1; }
    advance_model(it, -2);
    if (it.node->v != 1) { std::printf("0065e8b8_TEST FAIL\n"); return 1; }
    advance_model(it, 0);
    if (it.node->v != 1) { std::printf("0065e8b8_TEST FAIL\n"); return 1; }
    advance_model(it, -3);
    if (it.node->v != 3) { std::printf("0065e8b8_TEST FAIL\n"); return 1; }
    std::printf("0065e8b8_TEST PASS\n");
    return 0;
}