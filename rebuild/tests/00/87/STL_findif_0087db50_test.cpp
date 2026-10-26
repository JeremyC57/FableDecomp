#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of the STLport random-access __find_if @ 0x0087DB50 (four-way unrolled loop + switch tail) over a
// unary_negate predicate: the first entry the wrapped predicate rejects.
class CAnimationEntry { public: float Rotation; };
class CRotationBelow {                                   // stand-in for CTurningAnimCompare's unary overload
public:
    typedef const CAnimationEntry* argument_type;
    float RotationAngle;
    bool operator()(const CAnimationEntry* entry) const { return entry->Rotation < RotationAngle; }
};
struct random_access_iterator_tag { };
template <class _Predicate>
class unary_negate {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& __x) : _M_pred(__x) { }
    bool operator()(const typename _Predicate::argument_type& __x) const { return !_M_pred(__x); }
};
template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall FindIf(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (int)(__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}
int main()
{
    bool ok = true;
    CAnimationEntry entries[10];
    const CAnimationEntry* ptrs[10];
    CRotationBelow below;
    below.RotationAngle = 0.5f;
    random_access_iterator_tag tag;
    for (int n = 0; n <= 10; ++n) {
        for (int k = 0; k <= n; ++k) {                   // k = index of the first entry at or above 0.5 (n: none)
            for (int i = 0; i < 10; ++i) {
                entries[i].Rotation = (i < k) ? 0.1f : 0.9f;
                ptrs[i] = &entries[i];
            }
            const CAnimationEntry** found = FindIf(ptrs, ptrs + n, unary_negate<CRotationBelow>(below), tag);
            if (found != ptrs + k) ok = false;
        }
    }
    if (!ok) { std::printf("0087db50_TEST FAIL\n"); return 1; }
    std::printf("0087db50_TEST PASS\n");
    return 0;
}