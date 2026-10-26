// behaviour test for 0x009D7DB0 _List_base<CCountedPointer<CBankFileAsyncData> >::clear(): every node is freed, each pointee is deleted
// only when its count reaches zero, and the sentinel ends up linked to itself
#include <cstdio>
#include <cstdlib>
struct Info { int refs; int deleted; };
struct Node { Node* next; Node* prev; int* data; Info* info; };
static int gFreed = 0;
static void Clear(Node** self)                               // the function under test
{
    Node* cur = (*self)->next;
    while (cur != *self) {
        Node* tmp = cur; cur = cur->next;
        if (tmp->info) { if (--tmp->info->refs == 0) tmp->info->deleted = 1; }
        tmp->data = 0; tmp->info = 0;
        std::free(tmp); ++gFreed;
    }
    (*self)->next = *self; (*self)->prev = *self;
}
int main()
{
    int fail = 0;
    Node head; head.next = head.prev = &head;
    Node* self = &head;
    Clear(&self);
    if (gFreed != 0 || head.next != &head || head.prev != &head) fail |= 1;
    Info i1 = { 1, 0 }, i2 = { 2, 0 };
    int v1 = 1, v2 = 2;
    Node* a = (Node*)std::malloc(sizeof(Node)); a->data = &v1; a->info = &i1;
    Node* b = (Node*)std::malloc(sizeof(Node)); b->data = &v2; b->info = &i2;
    Node* c = (Node*)std::malloc(sizeof(Node)); c->data = 0; c->info = 0;
    head.next = a; a->prev = &head; a->next = b; b->prev = a; b->next = c; c->prev = b; c->next = &head; head.prev = c;
    Clear(&self);
    if (gFreed != 3 || head.next != &head || head.prev != &head) fail |= 2;
    if (i1.deleted != 1 || i1.refs != 0 || i2.deleted != 0 || i2.refs != 1) fail |= 4;
    if (sizeof(Info) != (unsigned int)8) fail |= 8;
    if (fail) { std::printf("009d7db0_TEST FAIL %d\n", fail); return 1; }
    std::printf("009d7db0_TEST PASS\n");
    return 0;
}