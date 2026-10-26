// behaviour test for 0x00C2C2B0 CUnprovenKeyedList_00c2c250::IsKeyLess(const CUnprovenKey_00c2c250* key, CUnprovenItem_00c2c250* item) const: ordering of the probe key and the item's key (at +0x8), through the key's own operator<
#include <cstdio>
struct KeyT { int v; };
static int gEq = 0, gLt = 0;
bool operator==(const KeyT& a, const KeyT& b) { ++gEq; return a.v == b.v; }
bool operator<(const KeyT& a, const KeyT& b) { ++gLt; return a.v < b.v; }
struct Item { char pad[0x8]; KeyT Key; };
struct List {
    int IsKeyLess(const KeyT* key, Item* item) const { return *key < item->Key; }   // the function under test
};
int main()
{
    int fail = 0;
    List list;
    Item i3, i5, j3;
    i3.Key.v = 3; i5.Key.v = 5; j3.Key.v = 3;
    KeyT k3; k3.v = 3; KeyT k5; k5.v = 5;
    if (list.IsKeyLess(&k3, &i3) != 0 || list.IsKeyLess(&k3, &i5) != 1 || list.IsKeyLess(&k5, &i3) != 0) fail |= 1;
    if (gLt != 3 || gEq != 0) fail |= 2;
    if (fail) { std::printf("00c2c2b0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c2c2b0_TEST PASS\n");
    return 0;
}