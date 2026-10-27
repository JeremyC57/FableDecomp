// behaviour test for 0x006C91E0: push_back links copies before the sentinel in insertion order
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0;
struct E { int v; E(int x) : v(x) { } E(const E& o) : v(o.v) { ++g_copies; } };
struct N { N* next; N* prev; E data; };
static void PushBack(N* head, const E& x) { N* t = (N*)std::malloc(sizeof(N)); new (&t->data) E(x); N* p = head->prev; t->next = head; t->prev = p; p->next = t; head->prev = t; }
int main()
{
    N* head = (N*)std::malloc(sizeof(N)); head->next = head->prev = head;
    for (int i = 0; i < 3; ++i) PushBack(head, E(i + 1));
    int ok = g_copies == 3 && head->next->data.v == 1 && head->prev->data.v == 3 && head->next->next->next->next == head;
    if (!ok) { std::printf("006c91e0_TEST FAIL\n"); return 1; }
    std::printf("006c91e0_TEST PASS\n");
    return 0;
}