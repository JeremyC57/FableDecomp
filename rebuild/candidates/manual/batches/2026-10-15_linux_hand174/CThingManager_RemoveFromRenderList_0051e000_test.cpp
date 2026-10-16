#include <cstdio>
static int g_added = 0, g_removed = 0;
struct Phys { void OnAdded() { ++g_added; } void OnRemoved() { ++g_removed; } };
struct Thing { unsigned char idx; Phys* PhysicsTC; Thing* prevR; Thing* nextR; Thing* prevU; Thing* nextU; };
struct LL { Thing* head; Thing* tail; };
struct Mgr { LL Render; LL* Update;
    void RemoveRender(Thing* t) {
        if (t->nextR || t->prevR || Render.head == t) {
            if (Render.head == t) Render.head = t->nextR;
            if (Render.tail == t) Render.tail = t->prevR;
            if (t->nextR) t->nextR->prevR = t->prevR;
            if (t->prevR) t->prevR->nextR = t->nextR;
            t->nextR = 0; t->prevR = 0; } }
    void AddUpdate(Thing* t) {
        LL* l = &Update[t->idx];
        if (t->nextU || t->prevU || l->head == t) return;
        if (t->PhysicsTC) t->PhysicsTC->OnAdded();
        if (l->tail) l->tail->nextU = t; else l->head = t;
        t->prevU = l->tail; t->nextU = 0; l->tail = t; }
    void RemoveUpdate(Thing* t) {
        LL* l = &Update[t->idx];
        if (t->nextU || t->prevU || l->head == t) {
            if (t->PhysicsTC) t->PhysicsTC->OnRemoved();
            if (l->head == t) l->head = t->nextU;
            if (l->tail == t) l->tail = t->prevU;
            if (t->nextU) t->nextU->prevU = t->prevU;
            if (t->prevU) t->prevU->nextU = t->nextU;
            t->nextU = 0; t->prevU = 0; } } };
int main()
{
    Phys p;
    Thing a = { 1, &p, 0, 0, 0, 0 }, b = { 1, 0, 0, 0, 0, 0 }, c = { 1, &p, 0, 0, 0, 0 };
    LL lists[2] = { { 0, 0 }, { 0, 0 } };
    Mgr m = { { &a, &c }, lists };
    a.nextR = &b; b.prevR = &a; b.nextR = &c; c.prevR = &b;
    m.RemoveRender(&b);
    bool ok = a.nextR == &c && c.prevR == &a && b.nextR == 0 && b.prevR == 0;
    m.RemoveRender(&a); ok = ok && m.Render.head == &c && c.prevR == 0;
    m.AddUpdate(&a); m.AddUpdate(&b); m.AddUpdate(&c); m.AddUpdate(&b);
    ok = ok && lists[1].head == &a && lists[1].tail == &c && a.nextU == &b && b.nextU == &c && c.prevU == &b && g_added == 2;
    m.RemoveUpdate(&b); ok = ok && a.nextU == &c && c.prevU == &a && g_removed == 0;
    m.RemoveUpdate(&c); ok = ok && lists[1].tail == &a && g_removed == 1;
    m.RemoveUpdate(&c); ok = ok && g_removed == 1;
    if (!ok) { std::printf("0051e000_TEST FAIL\n"); return 1; }
    std::printf("0051e000_TEST PASS\n");
    return 0;
}
