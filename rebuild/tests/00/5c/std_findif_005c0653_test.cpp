// behaviour test for 005c0653: STLport random-access __find_if (4x unrolled + 3/2/1 remainder switch) over CCategory (44 bytes)
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
}
template <class RandomAccessIter, class Predicate>
RandomAccessIter __fastcall FindIfModel(RandomAccessIter first, RandomAccessIter last, Predicate pred, const std::random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
struct CCategory { unsigned char PAssociatedThing[8]; unsigned char ItemsInCategory[12]; int PDef_Object; int PInventory; int GlobalDefIndex; int SelectedItemIndex; int HighlightedItemIndex; bool Selected; bool DisplayItems; unsigned char _pad[2]; };
struct CIsCategoryWithDefIndex { int DefIndex; bool operator()(const CCategory& category) const { return category.GlobalDefIndex == DefIndex; } };
int main()
{
    CCategory items[12];
    for (int i = 0; i < 12; ++i) {
        unsigned char* b = (unsigned char*)&items[i];
        for (unsigned j = 0; j < sizeof(items[i]); ++j) b[j] = 0xEE;
        items[i].GlobalDefIndex = 100 + i;
    }
    if (sizeof(CCategory) != 44) { std::printf("005c0653_TEST FAIL\n"); return 1; }
    std::random_access_iterator_tag tag;
    int bad = 0;
    for (int n = 0; n <= 12; ++n) {
        for (int k = 0; k < 13; ++k) {
            CIsCategoryWithDefIndex p; p.DefIndex = 100 + k;
            CCategory* r = FindIfModel(items, items + n, p, tag);
            CCategory* want = (k < n) ? items + k : items + n;
            if (r != want) ++bad;
        }
    }
    items[5].GlobalDefIndex = 102;   /* duplicate key: the first match wins */
    { CIsCategoryWithDefIndex p; p.DefIndex = 102; if (FindIfModel(items, items + 12, p, tag) != items + 2) ++bad; }
    { CIsCategoryWithDefIndex p; p.DefIndex = -1; if (FindIfModel(items, items + 12, p, tag) != items + 12) ++bad; }
    if (bad) { std::printf("005c0653_TEST FAIL\n"); return 1; }
    std::printf("005c0653_TEST PASS\n");
    return 0;
}