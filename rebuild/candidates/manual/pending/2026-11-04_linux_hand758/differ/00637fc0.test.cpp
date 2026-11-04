// Behaviour test for NThingMap::CThingMapManager::PlaceThing (0x00637FC0), g++ model: unless exempt, push the four corner nodes on the front of their (non-null) cells, flag InMapwho, count the thing.
#include <cstdio>
struct Cell; struct Node { Node* next; Node* prev; Cell* cell; };
struct Cell { Node* first; };
static void Add(Cell* cell, Node& n) { if (!cell) return; Node* f = cell->first; n.prev = 0; n.next = f; if (f) f->prev = &n; cell->first = &n; n.cell = cell; }
struct Mapwho { Node tl, tr, bl, br; bool inMapwho; };
static void PlaceThing(Mapwho& m, bool exempt, Cell* tl, Cell* tr, Cell* br, Cell* bl, long& noThings)
{
    if (exempt) return;
    Add(tl, m.tl); Add(tr, m.tr); Add(br, m.br); Add(bl, m.bl);
    m.inMapwho = true; ++noThings;
}
int main()
{
    Cell a = { 0 }, b = { 0 };
    Node other = { 0, 0, &a }; a.first = &other;
    Mapwho m = { { 0, 0, 0 }, { 0, 0, 0 }, { (Node*)1, (Node*)1, 0 }, { 0, 0, 0 }, false };
    long n = 0;
    PlaceThing(m, false, &a, &b, 0, 0, n);
    int ok = a.first == &m.tl && m.tl.next == &other && other.prev == &m.tl && m.tl.cell == &a && b.first == &m.tr && m.tr.next == 0
          && m.bl.cell == 0 && m.inMapwho && n == 1;
    Mapwho e = m; e.inMapwho = false; PlaceThing(e, true, &a, &b, 0, 0, n); ok = ok && !e.inMapwho && n == 1;
    std::printf(ok ? "00637fc0_TEST PASS\n" : "00637fc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
