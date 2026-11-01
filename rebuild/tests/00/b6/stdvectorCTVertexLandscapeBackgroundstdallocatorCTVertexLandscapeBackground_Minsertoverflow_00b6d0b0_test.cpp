#include <cstdio>
#include <cstdlib>
#include <new>
struct SScalarTag { SScalarTag() { } };
struct SRecord24 { int m_a, m_b, m_c, m_d, m_e, m_f; SRecord24() { } };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class InputIter, class ForwardIter>
inline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const SScalarTag&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class ForwardIter, class Size, class T>
inline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const SScalarTag&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
static int g_frees;
struct CRetailVector_SRecord24 {
    SRecord24* first; SRecord24* last; SRecord24* end;
    unsigned int size() const { return (unsigned int)(last - first); }
    SRecord24* allocate(unsigned int n) { return n != 0 ? (SRecord24*)std::malloc(n * sizeof(SRecord24)) : 0; }
    void Deallocate() { if (first) { std::free(first); ++g_frees; } }
    void InsertGrow(SRecord24* where, const SRecord24& value, const SScalarTag& tag, unsigned int count, bool atEnd)
    {
        const unsigned int oldSize = size();
        const unsigned int len = oldSize + (max)(oldSize, count);
        SRecord24* newStart = allocate(len);
        SRecord24* newFinish = newStart;
        newFinish = __uninitialized_copy(first, where, newStart, tag);
        if (count == 1) { _Construct(newFinish, value); ++newFinish; }
        else newFinish = __uninitialized_fill_n(newFinish, count, value, tag);
        if (!atEnd) newFinish = __uninitialized_copy(where, last, newFinish, tag);
        Deallocate();
        first = newStart; last = newFinish; end = newStart + len;
    }
};
static SRecord24 R(int k) { SRecord24 r; r.m_a = k; r.m_b = k + 1; r.m_c = k + 2; r.m_d = k + 3; r.m_e = k + 4; r.m_f = k * 10; return r; }
int main()
{
    CRetailVector_SRecord24 v; v.first = v.last = v.end = 0;
    v.InsertGrow(v.last, R(1), SScalarTag(), 1, true);            // empty -> 1 element, capacity 1
    if (v.size() != 1 || v.end - v.first != 1 || v.first[0].m_f != 10 || g_frees != 0) { std::printf("00b6d0b0_TEST FAIL\n"); return 1; }
    v.InsertGrow(v.last, R(2), SScalarTag(), 1, true);            // grow to capacity 2
    v.InsertGrow(v.first + 1, R(5), SScalarTag(), 3, false);      // insert 3 copies in the middle: 1,5,5,5,2
    if (v.size() != 5 || v.end - v.first != 5 || v.first[0].m_a != 1 || v.first[1].m_a != 5 || v.first[3].m_f != 50
        || v.first[4].m_a != 2 || v.first[4].m_e != 6 || g_frees != 2) { std::printf("00b6d0b0_TEST FAIL\n"); return 1; }
    std::free(v.first);
    std::printf("00b6d0b0_TEST PASS\n");
    return 0;
}