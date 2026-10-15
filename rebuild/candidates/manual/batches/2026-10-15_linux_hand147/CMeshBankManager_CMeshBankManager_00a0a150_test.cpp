#include <cstdio>
static int g_destroyed = 0;
static void Destroy(void*) { ++g_destroyed; }
struct Block { long refs; void (*destroy)(void*); void* obj; };
struct CP { void* obj; Block* blk;
    CP() : obj(0), blk(0) {}
    CP(void* o) : obj(o), blk(0) { if (o) { blk = new Block; blk->refs = 1; blk->destroy = Destroy; blk->obj = o; } }
    CP(const CP& o) : obj(o.obj), blk(o.blk) { if (blk) ++blk->refs; }
    ~CP() { Block* b = blk; if (b) { if (--b->refs == 0) { b->destroy(b->obj); delete b; } } }
    CP& operator=(const CP& o) { if (blk != o.blk) { if (blk) { --blk->refs; if (blk->refs == 0) { blk->destroy(blk->obj); delete blk; } } obj = o.obj; blk = o.blk; if (blk) ++blk->refs; } return *this; }
    void SetNull() { if (blk) { --blk->refs; if (blk->refs == 0) { blk->destroy(blk->obj); delete blk; } } blk = 0; obj = 0; } };
struct Info { long id; CP bank; };
struct Node { Node* next; Info value; };
static void ReleaseAll(Node* head) { for (Node* n = head->next; n != head; n = n->next) n->value.bank.SetNull(); }
int main()
{
    int o1, o2; Node head, a, b; head.next = &a; a.next = &b; b.next = &head;
    a.value.bank = CP(&o1); b.value.bank = CP(&o2); CP extra = b.value.bank;
    ReleaseAll(&head);
    if (g_destroyed != 1 || a.value.bank.blk != 0 || a.value.bank.obj != 0 || b.value.bank.blk != 0 || extra.blk->refs != 1) { std::printf("00a0a150_TEST FAIL\n"); return 1; }
    std::printf("00a0a150_TEST PASS\n");
    return 0;
}
