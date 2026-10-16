#include <cstdio>
// Behaviour model of CTCRegionFollower::CullNullFollowers (0x006AECF0): erase every list node whose pointer is null.
struct Node { Node* next; Node* prev; void* item; };
static int g_freed = 0;
static void Cull(Node* head)
{
    Node* it = head->next;
    while (it != head) {
        void* copy = it->item;
        if (copy == 0) {
            Node* next = it->next; Node* prev = it->prev;
            prev->next = next; next->prev = prev; ++g_freed;
            it = next;
        } else
            it = it->next;
    }
}
int main()
{
    int x, y;
    Node head, a, b, c, d;
    head.next = &a; a.next = &b; b.next = &c; c.next = &d; d.next = &head;
    head.prev = &d; d.prev = &c; c.prev = &b; b.prev = &a; a.prev = &head;
    a.item = &x; b.item = 0; c.item = 0; d.item = &y; head.item = 0;
    Cull(&head);
    if (g_freed != 2 || head.next != &a || a.next != &d || d.prev != &a || d.next != &head) { std::printf("006aecf0_TEST FAIL\n"); return 1; }
    std::printf("006aecf0_TEST PASS\n");
    return 0;
}