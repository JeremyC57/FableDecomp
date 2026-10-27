// behaviour test for 0x007CBBC0: insert links the copy before the position and returns it
#include <cstdio>
#include <cstdlib>
struct N { N* next; N* prev; void* item; long s; };
static N* Insert(N* pos, void* item, long s) { N* t = (N*)std::malloc(sizeof(N)); t->item = item; t->s = s; N* p = pos->prev; t->next = pos; t->prev = p; p->next = t; pos->prev = t; return t; }
int main()
{
    int o1, o2; N head; head.next = head.prev = &head;
    N* a = Insert(&head, &o1, 1); N* b = Insert(a, &o2, 2);
    int ok = head.next == b && b->next == a && a->next == &head && head.prev == a && b->s == 2 && a->item == &o1;
    std::free(a); std::free(b);
    if (!ok) { std::printf("007cbbc0_TEST FAIL\n"); return 1; }
    std::printf("007cbbc0_TEST PASS\n");
    return 0;
}