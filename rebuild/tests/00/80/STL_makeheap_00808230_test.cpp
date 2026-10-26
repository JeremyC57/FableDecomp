// Behaviour test for 0x00808230 STLport __make_heap: sift every parent down from (len-2)/2 to 0 (deterministic comparator stand-in).
#include <cstdio>
class CThing { public: int id; };
struct CPointer { CThing* Item; };
struct CLess { bool operator()(const CPointer& a, const CPointer& b) const { return a.Item->id < b.Item->id; } };
static void PushHeap(CPointer* first, int hole, int top, CPointer value, CLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeap(CPointer* first, int hole, int len, CPointer value, CLess comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) --child;
        first[hole] = first[child];
        hole = child;
        child = 2 * child + 2;
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    PushHeap(first, hole, top, value, comp);
}
static void MakeHeap(CPointer* first, CPointer* last, CLess comp, CPointer*, int*)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        AdjustHeap(first, parent, len, CPointer(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
int main()
{
    CThing things[10];
    for (int i = 0; i < 10; ++i) things[i].id = (i * 7) % 10;
    CPointer v[10];
    for (int i = 0; i < 10; ++i) v[i].Item = &things[i];
    MakeHeap(v, v + 10, CLess(), (CPointer*)0, (int*)0);
    bool ok = v[0].Item->id == 9;
    for (int i = 1; ok && i < 10; ++i)
        ok = !(v[(i - 1) / 2].Item->id < v[i].Item->id);   // heap property
    CPointer one[1] = { { &things[3] } };
    MakeHeap(one, one + 1, CLess(), (CPointer*)0, (int*)0);  // fewer than two: untouched
    ok = ok && one[0].Item == &things[3];
    if (!ok) { std::printf("00808230_TEST FAIL\n"); return 1; }
    std::printf("00808230_TEST PASS\n");
    return 0;
}