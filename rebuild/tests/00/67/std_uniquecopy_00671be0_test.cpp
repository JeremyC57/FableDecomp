#include <cstdio>
// Behaviour test for 0x00671BE0: STLport __unique_copy over ENavigatorType (drop adjacent duplicates).
enum ENavigatorType { NT0, NT1, NT2, NT3, NT4, NT_FORCE_DWORD = 0x7fffffff };
struct forward_iterator_tag { forward_iterator_tag() { } };
template <class T> struct equal_to { bool operator()(const T& x, const T& y) const { return x == y; } };
template <class _InputIterator, class _ForwardIter, class _BinaryPredicate>
_ForwardIter __unique_copy(_InputIterator __first, _InputIterator __last, _ForwardIter __result,
                           _BinaryPredicate __binary_pred, const forward_iterator_tag&)
{
    *__result = *__first;
    while (++__first != __last)
        if (!__binary_pred(*__result, *__first))
            *++__result = *__first;
    return ++__result;
}
int main()
{
    ENavigatorType v[9] = { NT0, NT1, NT1, NT1, NT2, NT3, NT3, NT4, NT4 };
    // in place from the first adjacent pair, as unique() does
    ENavigatorType* e = __unique_copy(v + 1, v + 9, v + 1, equal_to<ENavigatorType>(), forward_iterator_tag());
    ENavigatorType want[5] = { NT0, NT1, NT2, NT3, NT4 };
    if (e != v + 5) { std::printf("00671be0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (v[i] != want[i]) { std::printf("00671be0_TEST FAIL\n"); return 1; }
    ENavigatorType one[1] = { NT2 }; ENavigatorType out[1];
    if (__unique_copy(one, one + 1, out, equal_to<ENavigatorType>(), forward_iterator_tag()) != out + 1 || out[0] != NT2) {
        std::printf("00671be0_TEST FAIL\n"); return 1;
    }
    std::printf("00671be0_TEST PASS\n");
    return 0;
}