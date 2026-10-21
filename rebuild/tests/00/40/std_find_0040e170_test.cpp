#include <cstdio>
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
template <class RandomAccessIter, class T>
RandomAccessIter __fastcall __find(RandomAccessIter first, RandomAccessIter last, const T& val, const random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == val) return first;
        ++first;
    case 2:
        if (*first == val) return first;
        ++first;
    case 1:
        if (*first == val) return first;
    case 0:
    default:
        return last;
    }
}
template long* __fastcall __find<long*, long>(long*, long*, const long&, const random_access_iterator_tag&);
}

int main()
{
    long a[11] = { 5, 1, 2, 3, 4, 9, 7, 8, 6, 10, 11 };
    std::random_access_iterator_tag tag;
    bool ok = true;
    for (int n = 0; n <= 11; ++n)
        for (long v = 0; v <= 12; ++v) {
            long* expect = a + n;
            for (int i = 0; i < n; ++i) if (a[i] == v) { expect = a + i; break; }
            ok = ok && std::__find(a, a + n, v, tag) == expect;
        }
    std::printf(ok ? "0040e170_TEST PASS\n" : "0040e170_TEST FAIL\n");
    return ok ? 0 : 1;
}