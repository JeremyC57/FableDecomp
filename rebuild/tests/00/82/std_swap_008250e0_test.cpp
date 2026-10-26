// behaviour test for 0x008250E0 std::swap<std::pair<long, CCountedPointer<CContainmentVolume> > >: the two entries exchange key and counted pointer, every reference count ends where it began, and nothing is deleted
#include <cstdio>
struct Info { int RefCount; int Deleted; };
struct Counted {
    void* Data; Info* I;
    Counted(void* d, Info* i) : Data(d), I(i) { if (I) ++I->RefCount; }
    Counted(const Counted& o) : Data(o.Data), I(o.I) { if (I) ++I->RefCount; }
    ~Counted() { Release(); }
    void Release() { if (I) { if (--I->RefCount == 0) ++I->Deleted; } }
    Counted& operator=(const Counted& o) { if (I != o.I) { Release(); Data = o.Data; I = o.I; if (I) ++I->RefCount; } return *this; }
};
struct Entry { unsigned int first; Counted second; Entry(unsigned int k, const Counted& c) : first(k), second(c) { } };
static void Swap(Entry& a, Entry& b)                       // the function under test
{
    Entry tmp = a;
    a = b;
    b = tmp;
}
int main()
{
    int fail = 0;
    int x = 1, y = 2;
    Info ia = { 0, 0 }, ib = { 0, 0 };
    {
        Counted ca(&x, &ia), cb(&y, &ib);
        Entry a(7u, ca), b(0x80000002u, cb);
        Swap(a, b);
        if (a.first != 0x80000002u || b.first != 7u) fail |= 1;
        if (a.second.Data != &y || b.second.Data != &x || a.second.I != &ib || b.second.I != &ia) fail |= 2;
        if (ia.RefCount != 2 || ib.RefCount != 2 || ia.Deleted || ib.Deleted) fail |= 4;
        Entry c(3u, ca);                                   // both sides share one block
        Swap(b, c);
        if (b.first != 3u || c.first != 7u || ia.RefCount != 3 || ia.Deleted) fail |= 8;
        Entry n(9u, Counted(0, 0));                        // an empty pointer on one side
        Swap(a, n);
        if (a.first != 9u || a.second.I != 0 || n.second.I != &ib || ib.RefCount != 2 || ib.Deleted) fail |= 16;
    }
    if (ia.RefCount != 0 || ib.RefCount != 0 || ia.Deleted != 1 || ib.Deleted != 1) fail |= 32;
    if (fail) { std::printf("008250e0_TEST FAIL %d\n", fail); return 1; }
    std::printf("008250e0_TEST PASS\n");
    return 0;
}