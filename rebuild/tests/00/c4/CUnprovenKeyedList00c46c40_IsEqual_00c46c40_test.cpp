// behaviour test for 0x00C46C40 CUnprovenKeyedList_00c46c40::IsEqual(CUnprovenItem_00c46c40* a, CUnprovenItem_00c46c40* b) const: equality of the two items' keys (the key member at +0x4), through the key's own operator==
#include <cstdio>
struct KeyT { int v; };
static int gEq = 0, gLt = 0;
bool operator==(const KeyT& a, const KeyT& b) { ++gEq; return a.v == b.v; }
bool operator<(const KeyT& a, const KeyT& b) { ++gLt; return a.v < b.v; }
struct Item { char pad[0x4]; KeyT Key; };
struct List {
    int IsEqual(Item* a, Item* b) const { return a->Key == b->Key; }   // the function under test
};
int main()
{
    int fail = 0;
    List list;
    Item i3, i5, j3;
    i3.Key.v = 3; i5.Key.v = 5; j3.Key.v = 3;
    KeyT k3; k3.v = 3; KeyT k5; k5.v = 5;
    if (list.IsEqual(&i3, &j3) != 1 || list.IsEqual(&i3, &i5) != 0 || list.IsEqual(&i5, &i3) != 0) fail |= 1;
    if (gEq != 3 || gLt != 0) fail |= 2;
    if (fail) { std::printf("00c46c40_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c46c40_TEST PASS\n");
    return 0;
}