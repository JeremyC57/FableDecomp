#include <cstdio>
struct Item { long ItemType, NumberOfItems, a, b, c; };
struct Def { bool SelectEmptySlots, Wrap; };
struct Inv { long last; int calls; void OnItemHighlighted(long t) { last = t; ++calls; } };
struct Cat {
    Item* first; Item* lastp; Def* def; Inv* inv; long Highlighted; long uniqueCount;
    unsigned long size() const { return lastp - first; }
    long GetUnique() { return uniqueCount; }
    long GetNextOccupied(long i) { for (unsigned long j = i + 1; j < size(); ++j) if (first[j].NumberOfItems) return j; return i; }
    void HighlightNext() {
        long previous = Highlighted;
        if (def->SelectEmptySlots) {
            if (Highlighted + 1 < GetUnique()) {
                ++Highlighted;
                if ((unsigned long)Highlighted >= size()) { if (def->Wrap) Highlighted = 0; else Highlighted = size() - 1; }
            }
        } else Highlighted = GetNextOccupied(Highlighted);
        if (Highlighted != previous) inv->OnItemHighlighted(first[Highlighted].ItemType);
    }
};
int main()
{
    Item it[4] = { {10, 1}, {11, 0}, {12, 2}, {13, 0} };
    Def d = { false, false }; Inv inv = { 0, 0 };
    Cat c = { it, it + 4, &d, &inv, 0, 10 };
    c.HighlightNext();
    bool ok = c.Highlighted == 2 && inv.last == 12 && inv.calls == 1;
    c.HighlightNext();                                 // no later occupied: unchanged, no call
    ok = ok && c.Highlighted == 2 && inv.calls == 1;
    d.SelectEmptySlots = true; c.Highlighted = 3; d.Wrap = true;
    c.HighlightNext();                                 // past end, wraps to 0
    ok = ok && c.Highlighted == 0 && inv.last == 10 && inv.calls == 2;
    c.Highlighted = 3; d.Wrap = false; c.HighlightNext();
    ok = ok && c.Highlighted == 3 && inv.calls == 2;
    c.Highlighted = 1; c.HighlightNext();
    ok = ok && c.Highlighted == 2 && inv.calls == 3;
    if (!ok) { std::printf("005bd054_TEST FAIL\n"); return 1; }
    std::printf("005bd054_TEST PASS\n");
    return 0;
}