// behaviour test for 0x00449440 CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase>, CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> > >::Insert(const EGameAction&, const CCountedPointer<const CInputTypeBase>&): the {key, pointer} entry is appended at the end (in place, or through the grow path), the map is marked dirty, and the stored entry holds exactly one extra reference once the temporary is released
#include <cstdio>
struct Info { int RefCount; int Deleted; };
struct Counted {
    void* Data; Info* I;
    Counted() : Data(0), I(0) { }
    Counted(void* d, Info* i) : Data(d), I(i) { if (I) ++I->RefCount; }
    Counted(const Counted& o) : Data(o.Data), I(o.I) { if (I) ++I->RefCount; }
    ~Counted() { if (I) { if (--I->RefCount == 0) ++I->Deleted; } }
    Counted& operator=(const Counted& o)
    {
        if (I != o.I) { if (I) { if (--I->RefCount == 0) ++I->Deleted; } Data = o.Data; I = o.I; if (I) ++I->RefCount; }
        return *this;
    }
};
struct Entry { int first; Counted second; Entry() : first(0) { } Entry(int k, const Counted& c) : first(k), second(c) { } };
struct Map {
    Entry slots[4]; unsigned int count; unsigned int cap; bool Dirty; int grows;
    Map() : count(0), cap(2), Dirty(false), grows(0) { }
    void Insert(const int& key, const Counted& value)      // the function under test
    {
        Entry tmp(key, value);
        if (count == cap) { ++grows; cap = cap * 2; }       // _M_insert_overflow: grow, then place at the end
        slots[count] = tmp;
        ++count;
        Dirty = true;
    }
};
int main()
{
    int fail = 0;
    int x = 1, y = 2, z = 3;
    Info ia = { 0, 0 }, ib = { 0, 0 };
    {
        Counted ca(&x, &ia), cb(&y, &ib), none;
        Map m;
        m.Insert(30, ca);
        if (m.count != 1 || !m.Dirty || m.slots[0].first != 30 || m.slots[0].second.Data != &x || ia.RefCount != 2) fail |= 1;
        m.Dirty = false;
        m.Insert(10, cb);
        m.Insert(20, none);
        if (m.count != 3 || !m.Dirty || m.grows != 1) fail |= 2;
        if (m.slots[1].first != 10 || m.slots[2].first != 20 || m.slots[2].second.I != 0 || ib.RefCount != 2) fail |= 4;
        (void)z;
    }
    if (ia.RefCount != 0 || ib.RefCount != 0 || ia.Deleted != 1 || ib.Deleted != 1) fail |= 8;
    if (fail) { std::printf("00449440_TEST FAIL %d\n", fail); return 1; }
    std::printf("00449440_TEST PASS\n");
    return 0;
}