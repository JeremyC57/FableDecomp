// behaviour test for 005ba2a3: STLport random-access __find_if (4x unrolled + 3/2/1 remainder switch) over SKeyedRecord24 (24 bytes)
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
struct SKeyedRecord24 { int Key; int Value[5]; };
struct CIsRecordKeyEqualTo { int Key; bool operator()(const SKeyedRecord24& record) const { return record.Key == Key; } };
int main()
{
    SKeyedRecord24 items[12];
    for (int i = 0; i < 12; ++i) {
        unsigned char* b = (unsigned char*)&items[i];
        for (unsigned j = 0; j < sizeof(items[i]); ++j) b[j] = 0xEE;
        items[i].Key = 100 + i;
    }
    if (sizeof(SKeyedRecord24) != 24) { std::printf("005ba2a3_TEST FAIL\n"); return 1; }
    std::random_access_iterator_tag tag;
    int bad = 0;
    for (int n = 0; n <= 12; ++n) {
        for (int k = 0; k < 13; ++k) {
            CIsRecordKeyEqualTo p; p.Key = 100 + k;
            SKeyedRecord24* r = FindIfModel(items, items + n, p, tag);
            SKeyedRecord24* want = (k < n) ? items + k : items + n;
            if (r != want) ++bad;
        }
    }
    items[5].Key = 102;   /* duplicate key: the first match wins */
    { CIsRecordKeyEqualTo p; p.Key = 102; if (FindIfModel(items, items + 12, p, tag) != items + 2) ++bad; }
    { CIsRecordKeyEqualTo p; p.Key = -1; if (FindIfModel(items, items + 12, p, tag) != items + 12) ++bad; }
    if (bad) { std::printf("005ba2a3_TEST FAIL\n"); return 1; }
    std::printf("005ba2a3_TEST PASS\n");
    return 0;
}