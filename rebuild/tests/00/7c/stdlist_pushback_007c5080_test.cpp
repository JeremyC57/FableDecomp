// behaviour test for 0x007C5080: push_back links each new node before the sentinel, keeping insertion order
#include <cstdio>
#include <cstdlib>
struct N { N* next; N* prev; void* item; };
static void PushBack(N* head, void* item) { N* t = (N*)std::malloc(sizeof(N)); t->item = item; N* p = head->prev; t->next = head; t->prev = p; p->next = t; head->prev = t; }
int main()
{
    int objs[3]; N head; head.next = head.prev = &head;
    for (int i = 0; i < 3; ++i) PushBack(&head, &objs[i]);
    int ok = head.next->item == &objs[0] && head.next->next->item == &objs[1] && head.prev->item == &objs[2] && head.prev->next == &head;
    while (head.next != &head) { N* n = head.next; head.next = n->next; std::free(n); }
    if (!ok) { std::printf("007c5080_TEST FAIL\n"); return 1; }
    std::printf("007c5080_TEST PASS\n");
    return 0;
}