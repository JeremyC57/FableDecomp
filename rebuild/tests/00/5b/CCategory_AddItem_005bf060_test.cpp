#include <cstdio>
struct Item { long ItemType, NumberOfItems, max; bool IsFull() { return NumberOfItems >= max; } void Add(long t, long n, long) { ItemType = t; NumberOfItems += n; } };
struct Inv { long sel; int calls; void OnItemSelected(long t) { sel = t; ++calls; } };
struct Cat {
    Item* items; int n; Inv* inv; long Selected, Highlighted;
    long GetSlotWithItemsOfType(long t) { for (int i = 0; i < n; ++i) if (items[i].NumberOfItems && items[i].ItemType == t) return i; return -1; }
    long GetFirstEmptySlot() { for (int i = 0; i < n; ++i) if (!items[i].NumberOfItems) return i; return -1; }
    bool AddItem(long t, bool select, long data, long count) {
        long slot = GetSlotWithItemsOfType(t);
        if (slot != -1) { if (items[slot].IsFull()) return false; }
        else { slot = GetFirstEmptySlot(); if (slot == -1) return false; }
        if (items[slot].IsFull()) return false;
        items[slot].Add(t, count, data);
        if (items[Highlighted].NumberOfItems == 0) Highlighted = slot;
        if (select) { Selected = slot; inv->OnItemSelected(t); }
        return true;
    }
};
int main()
{
    Item it[2] = { {0, 0, 5}, {0, 0, 5} };
    Inv inv = { 0, 0 };
    Cat c = { it, 2, &inv, -1, 1 };
    bool ok = c.AddItem(7, true, 0, 2) && it[0].ItemType == 7 && it[0].NumberOfItems == 2 && c.Highlighted == 0 && c.Selected == 0 && inv.sel == 7;
    ok = ok && c.AddItem(7, false, 0, 3) && it[0].NumberOfItems == 5 && inv.calls == 1;
    ok = ok && !c.AddItem(7, false, 0, 1);              // full
    ok = ok && c.AddItem(8, false, 0, 1) && it[1].ItemType == 8;
    ok = ok && !c.AddItem(9, false, 0, 1);              // no empty slot
    if (!ok) { std::printf("005bf060_TEST FAIL\n"); return 1; }
    std::printf("005bf060_TEST PASS\n");
    return 0;
}