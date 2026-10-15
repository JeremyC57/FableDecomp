#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_destroyed = 0, g_freed = 0; static long g_now = 100;
struct CMessageEvent { virtual ~CMessageEvent() { ++g_destroyed; } long a; long type; unsigned char pad[0x28]; long TimeStamp; unsigned long Lifespan; unsigned char pad2[0x1c]; };
struct SNode { SNode* next; SNode* prev; CMessageEvent value; };
static void FreeNode(void*) { ++g_freed; }
struct CList { SNode* head;
    SNode* Erase(SNode* n) { SNode* nx = n->next; SNode* pv = n->prev; pv->next = nx; nx->prev = pv; n->value.~CMessageEvent(); FreeNode(n); return nx; } };
static long Now() { return g_now; }
struct CMgr { void* world; CList msgs;
    void Update() { SNode* n = msgs.head->next; while (n != msgs.head) { CMessageEvent* e = &n->value; if (e->Lifespan != 0 && e->TimeStamp + e->Lifespan < (unsigned long)Now()) n = msgs.Erase(n); else n = n->next; } } };
int main()
{
    SNode h, a, b, c; h.next = &a; a.prev = &h; a.next = &b; b.prev = &a; b.next = &c; c.prev = &b; c.next = &h; h.prev = &c;
    a.value.TimeStamp = 10; a.value.Lifespan = 0;      /* permanent */
    b.value.TimeStamp = 10; b.value.Lifespan = 50;     /* expired at 100 */
    c.value.TimeStamp = 90; c.value.Lifespan = 50;     /* alive */
    CMgr m; m.msgs.head = &h; m.Update();
    if (g_destroyed != 1 || g_freed != 1 || h.next != &a || a.next != &c || c.prev != &a || c.next != &h) { std::printf("006874b0_TEST FAIL\n"); return 1; }
    g_now = 141; m.Update(); if (g_destroyed != 2 || a.next != &h || h.prev != &a) { std::printf("006874b0_TEST FAIL\n"); return 1; }
    m.Update(); if (g_destroyed != 2) { std::printf("006874b0_TEST FAIL\n"); return 1; }
    std::printf("006874b0_TEST PASS\n");
    return 0;
}