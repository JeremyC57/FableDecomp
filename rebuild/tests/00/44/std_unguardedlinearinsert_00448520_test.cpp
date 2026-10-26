// behaviour test for 0x00448520 std::__unguarded_linear_insert over pair<EGameAction, CCountedPointer<const CInputTypeBase> >: entries whose key is above val's move up one slot (sharing their counted blocks), val lands in the hole, an equal key stops the walk, and every count block ends with the right reference count
#include <cstdio>
struct Info { int RefCount; };
struct Entry { int Key; Info* PInfo; };
static void Assign(Entry& to, const Entry& from)
{
    to.Key = from.Key;
    if (to.PInfo != from.PInfo) { if (to.PInfo) --to.PInfo->RefCount; to.PInfo = from.PInfo; if (to.PInfo) ++to.PInfo->RefCount; }
}
static void LinearInsert(Entry* last, Entry val)            // the function under test (val arrives with its own reference)
{
    Entry* next = last - 1;
    while (val.Key < next->Key) { Assign(*last, *next); last = next; --next; }
    Assign(*last, val);
    if (val.PInfo) --val.PInfo->RefCount;                 // ~val
}
int main()
{
    int fail = 0;
    Info i1 = { 1 }, i3 = { 1 }, i5 = { 1 }, i2 = { 1 };
    Entry a[4] = { { 1, &i1 }, { 3, &i3 }, { 5, &i5 }, { 2, &i2 } };
    Entry v = a[3]; ++i2.RefCount;                         // the caller's by-value copy
    LinearInsert(a + 3, v);
    if (a[0].Key != 1 || a[1].Key != 2 || a[2].Key != 3 || a[3].Key != 5) fail |= 1;
    if (a[1].PInfo != &i2 || a[2].PInfo != &i3 || a[3].PInfo != &i5) fail |= 2;
    if (i1.RefCount != 1 || i2.RefCount != 1 || i3.RefCount != 1 || i5.RefCount != 1) fail |= 4;
    Entry b[3] = { { 4, &i1 }, { 7, &i3 }, { 7, &i5 } };
    Entry w = b[2]; ++i5.RefCount;
    LinearInsert(b + 2, w);                                // equal key: stays put
    if (b[1].Key != 7 || b[1].PInfo != &i3 || b[2].PInfo != &i5 || i5.RefCount != 1 || i3.RefCount != 1) fail |= 8;
    if (fail) { std::printf("00448520_TEST FAIL %d\n", fail); return 1; }
    std::printf("00448520_TEST PASS\n");
    return 0;
}