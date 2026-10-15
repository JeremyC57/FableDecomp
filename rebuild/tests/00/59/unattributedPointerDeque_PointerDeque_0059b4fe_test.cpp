// Behaviour test for the STLport deque<T*> copy constructor (0x0059B4FE): the new deque is sized from the source (finish - start) and receives a copy of every element.
// Model: a single-block deque (the block arithmetic of the out-of-line iterator difference is not under test).
#include <cstdio>
struct SAllocator { SAllocator() {} };
struct SFalseType { SFalseType() {} };
struct SDequeIterator {
    void** _M_cur;
    SDequeIterator() : _M_cur(0) {}
    long operator-(const SDequeIterator& other) const { return (long)(_M_cur - other._M_cur); }
};
static void* g_storage[2][32];
static int g_nextBlock = 0;
static unsigned long g_baseCount = 0;
static SDequeIterator UninitializedCopy(SDequeIterator first, SDequeIterator last, SDequeIterator result, const SFalseType&)
{
    for (; first._M_cur != last._M_cur; ++first._M_cur, ++result._M_cur)
        *result._M_cur = *first._M_cur;
    return result;
}
struct CDequeBase {
    SDequeIterator _M_start, _M_finish;
    CDequeBase(const SAllocator&, unsigned long count)
    {
        g_baseCount = count;
        _M_start._M_cur = g_storage[g_nextBlock++];
        _M_finish._M_cur = _M_start._M_cur + count;
    }
};
struct PointerDeque : CDequeBase {
    PointerDeque(unsigned long n, void* const* values) : CDequeBase(SAllocator(), n)
    {
        for (unsigned long i = 0; i < n; ++i) _M_start._M_cur[i] = values[i];
    }
    SAllocator get_allocator() const { return SAllocator(); }
    unsigned long size() const { return _M_finish - _M_start; }
    SDequeIterator begin() const { return _M_start; }
    SDequeIterator end() const { return _M_finish; }
    PointerDeque(const PointerDeque& other) : CDequeBase(other.get_allocator(), other.size())
    {
        UninitializedCopy(other.begin(), other.end(), _M_start, SFalseType());
    }
};
int main()
{
    int a = 1, b = 2, c = 3;
    void* values[3] = { &a, &b, &c };
    PointerDeque source(3, values);
    PointerDeque copy(source);
    if (g_baseCount != 3 || copy.size() != 3 || copy._M_start._M_cur == source._M_start._M_cur ||
        copy._M_start._M_cur[0] != &a || copy._M_start._M_cur[1] != &b || copy._M_start._M_cur[2] != &c) { std::printf("0059b4fe_TEST FAIL\n"); return 1; }
    std::printf("0059b4fe_TEST PASS\n");
    return 0;
}