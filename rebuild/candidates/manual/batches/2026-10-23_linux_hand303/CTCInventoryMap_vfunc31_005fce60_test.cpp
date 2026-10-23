// Behaviour test for CTCInventoryMap::vfunc_31 @ 0x005FCE60: hover/selection reset always; UI teardown only while the
// hero-info screen is inactive; the dirty flag, list clear and the two pending flags always follow
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct Node { Node* next; Node* prev; };
struct Map {
    long v214, v188; int screenActive; int sound; int hud; long frame; long prevScreen; long screenSet;
    int comps[4]; int* start; int* finish; Node head; int dirty; int listsCleared; int f58, f248;
};
static void ClearNodes(Node* h)
{
    Node* cur = h->next;
    while (cur != h) { Node* tmp = cur; cur = cur->next; std::free(tmp); }
    h->next = h; h->prev = h;
}
static void Close(Map& m, long worldFrame)
{
    m.v214 = 0; m.v188 = 0;
    if (!m.screenActive) {
        m.sound = 3; m.hud = 1; m.frame = worldFrame; m.screenSet = m.prevScreen;
        for (int* p = m.start; p != m.finish; ++p) *p = 0;
        unsigned int n = (char*)m.finish - (char*)m.finish;
        if (n) std::memmove(m.start, m.finish, n);
        m.finish = m.start;
        ClearNodes(&m.head);
    }
    m.dirty = 1; m.listsCleared = 1; m.f58 = 0; m.f248 = 0;
}
static void Init(Map& m, int active)
{
    std::memset(&m, 0, sizeof(m));
    m.v214 = 5; m.v188 = 6; m.screenActive = active; m.prevScreen = 4; m.screenSet = -1; m.f58 = 1; m.f248 = 1;
    m.start = m.comps; m.finish = m.comps + 3;
    m.head.next = &m.head; m.head.prev = &m.head;
    for (int i = 0; i < 2; ++i) {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->next = m.head.next; n->prev = &m.head; m.head.next->prev = n; m.head.next = n;
    }
}
int main()
{
    int ok = 1;
    Map a; Init(a, 0); Close(a, 42);
    if (a.v214 || a.v188 || a.sound != 3 || a.frame != 42 || a.screenSet != 4 || a.finish != a.start || a.head.next != &a.head) ok = 0;
    if (!a.dirty || !a.listsCleared || a.f58 || a.f248) ok = 0;
    Map b; Init(b, 1); Close(b, 42);
    if (b.v214 || b.sound || b.frame || b.screenSet != -1 || b.finish != b.comps + 3 || b.head.next == &b.head) ok = 0;
    if (!b.dirty || b.f58 || b.f248) ok = 0;
    ClearNodes(&b.head);
    if (!ok) { std::printf("005fce60_TEST FAIL\n"); return 1; }
    std::printf("005fce60_TEST PASS\n");
    return 0;
}
