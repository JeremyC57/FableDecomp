// behaviour test for 0x00C24D80 CUnprovenKeyedList_00c24d80<CUnprovenItem_00c24d80, unsigned long, &CUnprovenItem_00c24d80::GetKey>::IsKeyLess(const unsigned long*, CUnprovenItem_00c24d80*) const: true exactly when the probe key is below the item's key, compared as unsigned 32-bit values
#include <cstdio>
struct Item {
    unsigned int Key;
    virtual ~Item() { }
    virtual const unsigned int& GetKey() const { return Key; }
};
template <class T, class K, const K& (T::*GetKeyFn)() const> struct List {
    int IsKeyLess(const K* key, T* item) const { return *key < (item->*GetKeyFn)(); }   // the function under test
};
int main()
{
    int fail = 0;
    List<Item, unsigned int, &Item::GetKey> list;
    Item item; item.Key = 10;
    unsigned int k5 = 5, k10 = 10, k11 = 11, kbig = 0xFFFFFFF0u;
    if (!list.IsKeyLess(&k5, &item)) fail |= 1;
    if (list.IsKeyLess(&k10, &item)) fail |= 2;
    if (list.IsKeyLess(&k11, &item)) fail |= 4;
    if (list.IsKeyLess(&kbig, &item)) fail |= 8;
    item.Key = 0xFFFFFFFFu;
    if (!list.IsKeyLess(&kbig, &item)) fail |= 16;
    if (fail) { std::printf("00c24d80_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c24d80_TEST PASS\n");
    return 0;
}