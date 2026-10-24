// Behaviour test for 0x00808950 STLport __unguarded_partition over thing pointers (deterministic comparator stand-in for the coin flip).
#include <cstdio>
struct CThing { int id; };
struct CPointer { CThing* Item; };
static int g_calls = 0;
struct CLess { bool operator()(const CPointer& a, const CPointer& b) const { ++g_calls; return a.Item->id < b.Item->id; } };
static void Swap(CPointer& a, CPointer& b) { CPointer t = a; a = b; b = t; }
static CPointer* UnguardedPartition(CPointer* first, CPointer* last, CPointer pivot, CLess comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Swap(*first, *last);
        ++first;
    }
}
int main()
{
    const int ids[9] = { 7, 2, 9, 4, 5, 1, 8, 3, 6 };
    CThing things[9];
    CPointer v[9];
    for (int i = 0; i < 9; ++i) { things[i].id = ids[i]; v[i].Item = &things[i]; }
    CThing pivotThing = { 5 };
    CPointer pivot = { &pivotThing };
    CPointer* cut = UnguardedPartition(v, v + 9, pivot, CLess());
    bool ok = cut > v && cut < v + 9;
    for (CPointer* p = v; ok && p != cut; ++p) ok = p->Item->id <= 5;
    for (CPointer* p = cut; ok && p != v + 9; ++p) ok = p->Item->id >= 5;
    int sum = 0;
    for (int i = 0; i < 9; ++i) sum += v[i].Item->id;
    ok = ok && sum == 45 && g_calls > 0;                   // a permutation of the input
    if (!ok) { std::printf("00808950_TEST FAIL\n"); return 1; }
    std::printf("00808950_TEST PASS\n");
    return 0;
}