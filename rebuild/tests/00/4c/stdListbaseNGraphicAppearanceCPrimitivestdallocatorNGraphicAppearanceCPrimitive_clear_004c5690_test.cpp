// Behaviour test for 0x004C5690 list<CPrimitive>::clear(): every node's handle tells a uniquely-held primitive it lost its
// last handle, drops its reference (destroying the primitive at zero), the node is freed and the sentinel is relinked empty.
#include <cstdio>
#include <cstdlib>

struct Prim { long RefCount; int lastHandle; int destroyed; };
struct Handle { Prim* Data; };
struct Primitive { long Type; Handle h; long BankIndex; };
struct Node { Node* next; Node* prev; Primitive v; };
static int g_freed = 0;

static void DestroyHandle(Handle& h)
{
    if (h.Data && h.Data->RefCount == 1) ++h.Data->lastHandle;
    if (h.Data) { if (--h.Data->RefCount == 0) ++h.Data->destroyed; h.Data = 0; }
    h.Data = 0;
}

static void Clear(Node* sentinel)
{
    Node* cur = sentinel->next;
    while (cur != sentinel) { Node* tmp = cur; cur = cur->next; DestroyHandle(tmp->v.h); std::free(tmp); ++g_freed; }
    sentinel->next = sentinel; sentinel->prev = sentinel;
}

static Node* Push(Node* s, Prim* p)
{
    Node* n = static_cast<Node*>(std::malloc(sizeof(Node)));
    n->v.h.Data = p; n->next = s; n->prev = s->prev; s->prev->next = n; s->prev = n;
    return n;
}

int main()
{
    Node s = { &s, &s };
    Prim unique = { 1, 0, 0 }, shared = { 3, 0, 0 };
    Push(&s, &unique); Push(&s, &shared); Push(&s, 0);
    Clear(&s);
    if (s.next != &s || s.prev != &s || g_freed != 3 || unique.lastHandle != 1 || unique.destroyed != 1 || unique.RefCount != 0 ||
        shared.lastHandle != 0 || shared.destroyed != 0 || shared.RefCount != 2) {
        std::printf("004c5690_TEST FAIL\n"); return 1;
    }
    Clear(&s);                                          // empty list: nothing freed, sentinel stays linked to itself
    if (g_freed != 3 || s.next != &s) { std::printf("004c5690_TEST FAIL\n"); return 1; }
    std::printf("004c5690_TEST PASS\n");
    return 0;
}