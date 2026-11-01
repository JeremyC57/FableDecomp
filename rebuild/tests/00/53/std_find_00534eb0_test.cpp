// Behaviour test for std::__find<CCountedPointer<CComponent>*, CCountedPointer<CComponent> > (0x00534EB0): the unrolled
// random-access find returns the first element with the same Data pointer, or last; every length 0..11 and position.
#include <cstdio>
class CComponent { public: int id; };
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
    T* Get() const { return Data; }
};
template <class T, class U> inline bool operator==(const CCountedPointer<T>& a, const CCountedPointer<U>& b) { return a.Get() == b.Get(); }
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter find_ra(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val, const random_access_iterator_tag&)
{
    int __trip_count = (int)(__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
int main()
{
    CComponent comps[12];
    CCountedPointer<CComponent> arr[12];
    for (int i = 0; i < 12; ++i) { comps[i].id = i; arr[i].Data = &comps[i]; arr[i].Info = (void*)(arr + i); }
    for (int n = 0; n <= 11; ++n) {
        for (int k = 0; k < 12; ++k) {
            CCountedPointer<CComponent> val;
            val.Data = &comps[k];
            val.Info = 0;                                   // only Data takes part in the compare
            CCountedPointer<CComponent>* r = find_ra(arr, arr + n, val, random_access_iterator_tag());
            CCountedPointer<CComponent>* want = (k < n) ? arr + k : arr + n;
            if (r != want) { std::printf("00534eb0_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("00534eb0_TEST PASS\n");
    return 0;
}