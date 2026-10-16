#include <cstdio>
struct CWound { int life; bool Dead; int killed; void Update() { if (--life <= 0) Dead = true; } void Kill() { ++killed; } };
struct Node { Node* next; Node* prev; CWound w; };
static int g_freed = 0;
static Node* Erase(Node* pos) { Node* n = pos->next; Node* p = pos->prev; p->next = n; n->prev = p; ++g_freed; return n; }
static void FrameUpdate(Node* head, bool hasDecal)
{
    if (!hasDecal) return;
    for (Node* it = head->next; it != head; ) {
        it->w.Update();
        if (it->w.Dead) { it->w.Kill(); it = Erase(it); } else it = it->next;
    }
}
int main()
{
    Node h, a, b, c; h.next = &a; a.next = &b; b.next = &c; c.next = &h; h.prev = &c; c.prev = &b; b.prev = &a; a.prev = &h;
    a.w.life = 1; b.w.life = 3; c.w.life = 1; a.w.Dead = b.w.Dead = c.w.Dead = false; a.w.killed = b.w.killed = c.w.killed = 0;
    FrameUpdate(&h, false);
    if (g_freed) { std::printf("00757760_TEST FAIL\n"); return 1; }
    FrameUpdate(&h, true);
    if (g_freed != 2 || h.next != &b || b.next != &h || h.prev != &b || a.w.killed != 1 || b.w.life != 2) { std::printf("00757760_TEST FAIL\n"); return 1; }
    std::printf("00757760_TEST PASS\n");
    return 0;
}
