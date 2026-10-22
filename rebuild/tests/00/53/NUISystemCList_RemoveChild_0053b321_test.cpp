// Behaviour test for NUISystem::CList::RemoveChild (0x0053B321): removing a scrolling child shifts the rows after it,
// clamps the selection (first stays 0, last moves to the new last), drops its per-row entries and recomputes offsets;
// a non-scrolling child is erased from its own array; an emptied list sends action 1 and pops itself from the registry
// head; the arrows get the new counts.
#include <cstdio>

struct Arr {
    int v[8]; long n;
    long Find(int x) const { for (long i = 0; i < n; ++i) if (v[i] == x) return i; return n; }
    void EraseAt(long p) { for (long i = p + 1; i < n; ++i) v[i - 1] = v[i]; --n; }
    void Push(int x) { v[n++] = x; }
};

struct List {
    Arr scrolling, nonScrolling, states, visibility, registry;
    long selected; long upCount; long downCount; long scrollLimit; int actions; int recomputes;
    long shiftFrom, shiftTo; int self;
};

static void RemoveChild(List& l, int child)
{
    long pos = l.scrolling.Find(child);
    if (pos != l.scrolling.n) {
        long size = l.scrolling.n;
        l.shiftFrom = pos; l.shiftTo = size - 1;
        if (l.selected == 0) l.selected = 0;
        else if (l.selected == size - 1) l.selected = size - 2;
        l.scrolling.EraseAt(pos);
        l.states.EraseAt(pos);
        l.visibility.EraseAt(pos);
        ++l.recomputes;
    } else {
        long found = l.nonScrolling.Find(child);
        if (found != l.nonScrolling.n)
            l.nonScrolling.EraseAt(found);
    }
    if (l.scrolling.n == 0) {
        ++l.actions;
        if (l.registry.n != 0 && l.registry.v[0] == l.self)
            l.registry.EraseAt(0);
    }
    l.upCount = l.scrolling.n;
    l.downCount = l.scrolling.n - l.scrollLimit - 1;
}

int main()
{
    List l;
    l.scrolling.n = l.nonScrolling.n = l.states.n = l.visibility.n = l.registry.n = 0;
    for (int i = 0; i < 4; ++i) { l.scrolling.Push(10 + i); l.states.Push(i); l.visibility.Push(1); }
    l.nonScrolling.Push(99);
    l.registry.Push(7); l.registry.Push(8);
    l.selected = 3; l.upCount = l.downCount = 0; l.scrollLimit = 1; l.actions = 0; l.recomputes = 0; l.self = 7;
    RemoveChild(l, 13);                           // last child, selected last -> selection moves up
    bool ok = l.scrolling.n == 3 && l.selected == 2 && l.shiftFrom == 3 && l.shiftTo == 3 && l.states.n == 3
        && l.upCount == 3 && l.downCount == 1 && l.recomputes == 1;
    RemoveChild(l, 99);                           // non-scrolling child
    ok = ok && l.nonScrolling.n == 0 && l.recomputes == 1;
    RemoveChild(l, 10);
    ok = ok && l.selected == 1 && l.scrolling.v[0] == 11;
    RemoveChild(l, 11); RemoveChild(l, 12);
    ok = ok && l.scrolling.n == 0 && l.selected == 0 && l.actions == 1 && l.registry.n == 1 && l.registry.v[0] == 8
        && l.upCount == 0 && l.downCount == -2;
    std::printf(ok ? "0053b321_TEST PASS\n" : "0053b321_TEST FAIL\n");
    return ok ? 0 : 1;
}