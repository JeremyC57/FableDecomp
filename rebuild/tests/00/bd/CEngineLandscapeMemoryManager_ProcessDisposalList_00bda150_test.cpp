// Test for CEngineLandscapeMemoryManager::ProcessDisposalList 0x00BDA150: the list after the current one ((index + 1) % 3)
// is drained front to back (every element deleted, list left empty); with delayed destruction off the free pass runs first.
#include <cstdio>
#include <list>
struct Res { int id; bool deleted; Res(int i) : id(i), deleted(false) {} };
struct Manager {
    bool enableDelayed; long current; std::list<Res*> lists[3]; int freed; int deletedOrder[8]; int n;
    Manager() : enableDelayed(true), current(0), freed(0), n(0) {}
    void FreeDisposalList() { freed++; }
    void DeleteTexture(Res* r) { r->deleted = true; deletedOrder[n++] = r->id; }
    void ProcessDisposalList() {
        if (!enableDelayed) FreeDisposalList();
        long index = (current + 1) % 3;
        while (lists[index].size() != 0) { DeleteTexture(lists[index].front()); lists[index].pop_front(); }
    }
};
int main() {
    Manager m; Res a(1), b(2), c(3), d(4);
    m.current = 2;                      // (2 + 1) % 3 == 0 -> list 0 is drained
    m.lists[0].push_back(&a); m.lists[0].push_back(&b); m.lists[1].push_back(&c); m.lists[2].push_back(&d);
    m.ProcessDisposalList();
    bool ok = m.freed == 0 && a.deleted && b.deleted && !c.deleted && !d.deleted && m.lists[0].empty() &&
              m.lists[1].size() == 1 && m.n == 2 && m.deletedOrder[0] == 1 && m.deletedOrder[1] == 2;
    m.enableDelayed = false; m.current = 0; m.ProcessDisposalList();   // list 1 drained, free pass first
    ok = ok && m.freed == 1 && c.deleted && !d.deleted && m.lists[1].empty();
    if (ok) printf("00bda150_TEST PASS\n"); else printf("00bda150_TEST FAIL\n");
    return ok ? 0 : 1;
}