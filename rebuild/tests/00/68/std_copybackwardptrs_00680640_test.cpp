// behaviour test for 0x00680640 std::__copy_backward_ptrs (std::pair<EReactionSpeechType, long>*): n = last - first elements copied back to front into the range ending at result (safe for an overlapping right shift), both words of every element moved, nothing outside [result - n, result) written, result - n returned; an empty range is a no-op
#include <cstdio>
struct TElem {                                  // stand-in for std::pair<EReactionSpeechType, long>: two words, compiler-generated block assignment
    int A;
    int B;
};
namespace stlp_test {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _RandomAccessIter, class _BidirectionalIter>
_BidirectionalIter __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                        _BidirectionalIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_aux(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                               _BidirectionalIter2 __result, const __true_type&)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward_aux(__first, __last, __result, __true_type());
}
}
static TElem* Run(TElem* first, TElem* last, TElem* result)
{
    return stlp_test::__copy_backward_ptrs(first, last, result, stlp_test::__false_type());
}
static void Fill(TElem* v, int n)
{
    for (int i = 0; i < n; ++i) { v[i].A = 100 + i; v[i].B = -(100 + i); }
}
static int Check()
{
    TElem v[10];
    Fill(v, 10);
    TElem* r = Run(v + 2, v + 5, v + 8);                   // disjoint: [2,5) -> [5,8)
    if (r != v + 5) return 1;
    for (int i = 0; i < 10; ++i) {
        int want = (i >= 5 && i < 8) ? 100 + i - 3 : 100 + i;
        if (v[i].A != want || v[i].B != -want) return 2;
    }
    Fill(v, 10);
    r = Run(v + 1, v + 6, v + 8);                          // overlapping right shift by 2: [1,6) -> [3,8)
    if (r != v + 3) return 3;
    int want2[10] = { 100, 101, 102, 101, 102, 103, 104, 105, 108, 109 };
    for (int i = 0; i < 10; ++i)
        if (v[i].A != want2[i] || v[i].B != -want2[i]) return 4;
    Fill(v, 10);
    r = Run(v + 4, v + 4, v + 9);                          // empty range
    if (r != v + 9) return 5;
    for (int i = 0; i < 10; ++i)
        if (v[i].A != 100 + i) return 6;
    r = Run(v + 9, v + 10, v + 1);                         // single element to the front
    if (r != v + 0 || v[0].A != 109 || v[0].B != -109 || v[1].A != 101) return 7;
    return 0;
}
int main()
{
    int r = Check();
    if (r != 0) { std::printf("00680640_TEST FAIL %d\n", r); return 1; }
    std::printf("00680640_TEST PASS\n");
    return 0;
}