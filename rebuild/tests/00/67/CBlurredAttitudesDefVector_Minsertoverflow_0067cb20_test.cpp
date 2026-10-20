// Behaviour test for vector<CBlurredAttitudesDef>::_M_insert_overflow (0x0067CB20): growth to size + max(size, n),
// head copied, n copies of x, tail copied unless appending, old elements destroyed and the old block freed.
#include <cstdio>
#include <cstdlib>
#include <new>
struct __false_type {};
static int g_live = 0;
struct CDef {
    int V;
    CDef(int v) : V(v) { ++g_live; }
    CDef(const CDef& o) : V(o.V) { ++g_live; }
    virtual ~CDef() { --g_live; }
};
static CDef* __uninitialized_copy(CDef* first, CDef* last, CDef* result, const __false_type&)
{ for (; first != last; ++first, ++result) new (result) CDef(*first); return result; }
static CDef* __uninitialized_fill_n(CDef* first, unsigned long n, const CDef& x, const __false_type&)
{ for (; n; --n, ++first) new (first) CDef(x); return first; }
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
struct CDefVector {
    CDef* _M_start; CDef* _M_finish; CDef* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    static CDef* allocate(unsigned long n) { return n != 0 ? (CDef*)std::malloc(n * sizeof(CDef)) : 0; }
    void _M_clear() { for (CDef* p = _M_start; p != _M_finish; ++p) p->~CDef(); if (_M_start) std::free(_M_start); }
    void _M_insert_overflow(CDef* position, const CDef& x, const __false_type&, unsigned long fill_len, bool atend)
    {
        const unsigned long old_size = size();
        const unsigned long len = old_size + (max)(old_size, fill_len);
        CDef* new_start = allocate(len);
        CDef* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
        if (fill_len == 1) { new (new_finish) CDef(x); ++new_finish; }
        else new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
        if (!atend) new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
        _M_clear();
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};
int main()
{
    CDefVector v; v._M_start = (CDef*)std::malloc(3 * sizeof(CDef));
    for (int i = 0; i < 3; ++i) new (v._M_start + i) CDef(i + 1);
    v._M_finish = v._M_end_of_storage = v._M_start + 3;
    CDef x(9);
    v._M_insert_overflow(v._M_start + 1, x, __false_type(), 2, false);       // 1 9 9 2 3, capacity 3 + 3
    bool ok = v.size() == 5 && v._M_end_of_storage - v._M_start == 6 && v._M_start[0].V == 1 && v._M_start[1].V == 9
        && v._M_start[2].V == 9 && v._M_start[3].V == 2 && v._M_start[4].V == 3 && g_live == 6;
    v._M_insert_overflow(v._M_finish, x, __false_type(), 1, true);           // append one: capacity 5 + 5
    ok = ok && v.size() == 6 && v._M_end_of_storage - v._M_start == 10 && v._M_start[5].V == 9 && g_live == 7;
    v._M_clear();
    if (!ok) { std::printf("0067cb20_TEST FAIL\n"); return 1; }
    std::printf("0067cb20_TEST PASS\n");
    return 0;
}