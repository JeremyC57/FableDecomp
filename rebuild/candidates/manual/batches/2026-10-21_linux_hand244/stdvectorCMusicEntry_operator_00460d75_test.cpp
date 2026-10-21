#include <cstdio>
#include <cstdlib>
#include <new>
// behaviour test: std::vector<CMusicEntry>::operator= -- the three STLport paths (grow, shrink-copy, copy + construct tail)
struct CMusicEntry { int a; int b; float c; int d; };
static int g_allocs = 0, g_clears = 0;
struct CMusicEntryVector {
    typedef CMusicEntry* pointer;
    typedef const CMusicEntry* const_pointer;
    pointer _M_start, _M_finish, _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int n, const_pointer first, const_pointer last)
    {
        ++g_allocs;
        pointer p = (CMusicEntry*)std::malloc(n * sizeof(CMusicEntry));
        pointer q = p;
        for (; first != last; ++first, ++q) new (q) CMusicEntry(*first);
        return p;
    }
    void _M_clear() { ++g_clears; std::free(_M_start); }
    CMusicEntryVector& operator=(const CMusicEntryVector& __x);
};
template <class I, class O> O copy_n_(I first, I last, O result)
{
    for (long n = (long)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
template <class I, class O> O ucopy_(I first, I last, O result)
{
    O cur = result;
    for (; first != last; ++first, ++cur) new (&*cur) CMusicEntry(*first);
    return cur;
}
CMusicEntryVector& CMusicEntryVector::operator=(const CMusicEntryVector& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, __x._M_start, __x._M_finish);
            _M_clear();
            _M_start = __tmp;
            _M_end_of_storage = _M_start + __xlen;
        }
        else if (size() >= __xlen) {
            copy_n_((const_pointer)__x._M_start, (const_pointer)__x._M_finish, _M_start);
        }
        else {
            copy_n_((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), _M_start);
            ucopy_((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish, _M_finish);
        }
        _M_finish = _M_start + __xlen;
    }
    return *this;
}
static void init(CMusicEntryVector& v, int n, int cap, int base)
{
    v._M_start = (CMusicEntry*)std::malloc(cap * sizeof(CMusicEntry));
    v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_start + cap;
    for (int i = 0; i < n; ++i) { v._M_start[i].a = base + i; v._M_start[i].b = i; v._M_start[i].c = 0.5f * i; v._M_start[i].d = -i; }
}
static bool same(const CMusicEntryVector& x, const CMusicEntryVector& y)
{
    if (x.size() != y.size()) return false;
    for (unsigned i = 0; i < x.size(); ++i)
        if (x._M_start[i].a != y._M_start[i].a || x._M_start[i].c != y._M_start[i].c || x._M_start[i].d != y._M_start[i].d) return false;
    return true;
}
int main()
{
    CMusicEntryVector src, a, b, c;
    init(src, 4, 4, 100);
    init(a, 1, 2, 0);   // grow path
    init(b, 6, 8, 0);   // shrink-copy path
    init(c, 2, 6, 0);   // copy + construct tail
    bool ok = (&(a = src) == &a) && same(a, src) && g_allocs == 1 && g_clears == 1 && a.capacity() == 4;
    ok = ok && (&(b = src) == &b) && same(b, src) && b.capacity() == 8 && g_allocs == 1;
    ok = ok && (&(c = src) == &c) && same(c, src) && c.capacity() == 6 && g_allocs == 1;
    ok = ok && (&(c = c) == &c) && same(c, src);
    if (!ok) { std::printf("00460d75_TEST FAIL\n"); return 1; }
    std::printf("00460d75_TEST PASS\n");
    return 0;
}
