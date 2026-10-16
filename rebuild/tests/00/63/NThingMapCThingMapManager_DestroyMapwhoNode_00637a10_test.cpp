#include <cstdio>
struct Cell;
struct Node { void* mapwho; Node* next; Node* prev; Cell* cell; };
struct Cell { Node* head; };
struct Mapwho { bool exempt; Node TL, TR, BL, BR; bool InMapwho; };
static void RemoveFromCell(Cell* c, Node* n)
{
    if (n->next) n->next->prev = n->prev;
    if (n->prev) n->prev->next = n->next; else if (c->head == n) c->head = n->next;
    n->prev = 0; n->next = 0; n->cell = 0;
}
struct Mgr { long NoThings;
    void Destroy(Mapwho* m) {
        if (m->exempt) return;
        if (m->TL.cell) RemoveFromCell(m->TL.cell, &m->TL);
        if (m->TR.cell) RemoveFromCell(m->TR.cell, &m->TR);
        if (m->BR.cell) RemoveFromCell(m->BR.cell, &m->BR);
        if (m->BL.cell) RemoveFromCell(m->BL.cell, &m->BL);
        m->InMapwho = false; --NoThings; } };
int main()
{
    Cell c1 = { 0 }, c2 = { 0 };
    Mapwho m = { false, { 0, 0, 0, &c1 }, { 0, 0, 0, &c2 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, true };
    c1.head = &m.TL; c2.head = &m.TR;
    Mgr g = { 5 };
    g.Destroy(&m);
    bool ok = c1.head == 0 && c2.head == 0 && m.TL.cell == 0 && !m.InMapwho && g.NoThings == 4;
    Mapwho e = { true, { 0, 0, 0, &c1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, true };
    g.Destroy(&e); ok = ok && e.InMapwho && g.NoThings == 4;
    if (!ok) { std::printf("00637a10_TEST FAIL\n"); return 1; }
    std::printf("00637a10_TEST PASS\n");
    return 0;
}