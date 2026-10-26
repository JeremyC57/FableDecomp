// behaviour test for 0x009C6C60 list<CCountedPointer<const CFileInstaller::CFileInfo> >::erase(iterator): the node is unlinked, the pointee is deleted
// only when its count reaches zero, and the iterator to the following node is returned
#include <cstdio>
#include <cstdlib>
struct Info { int refs; int deleted; };
struct Node { Node* next; Node* prev; int* data; Info* info; };
static int gFreed = 0;
static Node* Erase(Node* pos)                                // the function under test
{
    Node* next = pos->next; Node* prev = pos->prev;
    prev->next = next; next->prev = prev;
    if (pos->info) { if (--pos->info->refs == 0) pos->info->deleted = 1; }
    pos->data = 0; pos->info = 0;
    std::free(pos); ++gFreed;
    return next;
}
int main()
{
    int fail = 0;
    Node head; head.next = head.prev = &head;
    Info i1 = { 1, 0 }, i2 = { 2, 0 };
    int v1 = 1, v2 = 2;
    Node* a = (Node*)std::malloc(sizeof(Node)); a->data = &v1; a->info = &i1;
    Node* b = (Node*)std::malloc(sizeof(Node)); b->data = &v2; b->info = &i2;
    head.next = a; a->prev = &head; a->next = b; b->prev = a; b->next = &head; head.prev = b;
    if (Erase(a) != b || head.next != b || b->prev != &head || i1.deleted != 1 || gFreed != 1) fail |= 1;
    if (Erase(b) != &head || head.next != &head || head.prev != &head || i2.deleted != 0 || i2.refs != 1) fail |= 2;
    if (sizeof(Info) != (unsigned int)8) fail |= 4;
    if (fail) { std::printf("009c6c60_TEST FAIL %d\n", fail); return 1; }
    std::printf("009c6c60_TEST PASS\n");
    return 0;
}