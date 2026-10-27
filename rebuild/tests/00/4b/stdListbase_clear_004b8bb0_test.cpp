// behaviour test for 0x004B8BB0: clear destroys every element once and leaves an empty circular sentinel
#include <cstdio>
#include <cstdlib>
static int g_dtors = 0;
struct S { ~S() { ++g_dtors; } };
struct E { S a; long b; S c; };
struct N { N* next; N* prev; E data; };
static void Clear(N* head) { N* cur = head->next; while (cur != head) { N* tmp = cur; cur = cur->next; tmp->data.~E(); std::free(tmp); } head->next = head; head->prev = head; }
int main()
{
    N head; head.next = head.prev = &head;
    for (int i = 0; i < 3; ++i) { N* n = (N*)std::malloc(sizeof(N)); n->next = &head; n->prev = head.prev; head.prev->next = n; head.prev = n; }
    Clear(&head);
    if (g_dtors != 6 || head.next != &head || head.prev != &head) { std::printf("004b8bb0_TEST FAIL\n"); return 1; }
    std::printf("004b8bb0_TEST PASS\n");
    return 0;
}