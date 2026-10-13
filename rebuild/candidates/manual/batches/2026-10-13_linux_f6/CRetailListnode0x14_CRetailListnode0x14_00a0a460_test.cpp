#include <cstdio>
#include <cstdlib>
struct A {}; struct L { struct node { node* next; node* prev; unsigned char v[0x14 - 8]; }; node* head;
    L(A) : head(0) { head = (node*)std::malloc(sizeof(node)); head->next = head; head->prev = head; } };
int main()
{
    A al; L l(al); if (!l.head || l.head->next != l.head || l.head->prev != l.head) { std::printf("00a0a460_TEST FAIL\n"); return 1; }
    std::free(l.head);

    std::printf("00a0a460_TEST PASS\n");
    return 0;
}
