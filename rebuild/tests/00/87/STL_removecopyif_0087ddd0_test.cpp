#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of the STLport remove_copy_if @ 0x0087DDD0 over a unary_negate predicate: copies down the entries the wrapped
// predicate accepts and returns the new end.
struct CAnimationEntry { float Rotation; };
class CRotationBelow {
public:
    typedef const CAnimationEntry* argument_type;
    float RotationAngle;
    bool operator()(const CAnimationEntry* entry) const { return entry->Rotation < RotationAngle; }
};
template <class _Predicate>
class unary_negate {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& __x) : _M_pred(__x) { }
    bool operator()(const typename _Predicate::argument_type& __x) const { return !_M_pred(__x); }
};
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall RemoveCopyIf(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (!__pred(*__first)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}
int main()
{
    bool ok = true;
    CAnimationEntry e[6] = { {0.1f}, {0.8f}, {0.3f}, {0.9f}, {0.2f}, {0.6f} };
    const CAnimationEntry* v[6];
    for (int i = 0; i < 6; ++i) v[i] = &e[i];
    CRotationBelow below;
    below.RotationAngle = 0.5f;
    const CAnimationEntry** end = RemoveCopyIf(v, v + 6, v, unary_negate<CRotationBelow>(below));
    if (end != v + 3 || v[0] != &e[0] || v[1] != &e[2] || v[2] != &e[4]) ok = false;
    if (RemoveCopyIf(v, v, v, unary_negate<CRotationBelow>(below)) != v) ok = false;
    if (!ok) { std::printf("0087ddd0_TEST FAIL\n"); return 1; }
    std::printf("0087ddd0_TEST PASS\n");
    return 0;
}