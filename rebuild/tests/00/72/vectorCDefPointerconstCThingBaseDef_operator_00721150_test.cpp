// Behaviour test for 0x00721150 vector<CDefPointer<const CThingBaseDef> >::operator= (the out-of-line
// callees _M_allocate_and_copy / __copy / __uninitialized_copy / __destroy_aux modelled with ref-counting).
#include <cstdio>
#include <cstdlib>
struct CDef { long RefCount; };
struct TDefPtr { CDef* Object; };
static void addref(CDef* p) { if (p) ++p->RefCount; }
static void release(CDef* p) { if (p) --p->RefCount; }
static TDefPtr* uninit_copy(const TDefPtr* first, const TDefPtr* last, TDefPtr* result)
{
    for (; first != last; ++first, ++result) { result->Object = first->Object; addref(result->Object); }
    return result;
}
static TDefPtr* copy_(const TDefPtr* first, const TDefPtr* last, TDefPtr* result)
{
    for (long n = (long)(last - first); n > 0; --n, ++first, ++result)
        if (result->Object != first->Object) { release(result->Object); result->Object = first->Object; addref(result->Object); }
    return result;
}
static void destroy_(TDefPtr* first, TDefPtr* last) { for (; first != last; ++first) { release(first->Object); first->Object = 0; } }
struct CDefPointerVector {
    typedef unsigned int size_type;
    TDefPtr* _M_start;
    TDefPtr* _M_finish;
    TDefPtr* _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage - _M_start); }
    TDefPtr* _M_allocate_and_copy(size_type n, const TDefPtr* first, const TDefPtr* last)
    {
        TDefPtr* r = n ? (TDefPtr*)std::malloc(n * sizeof(TDefPtr)) : 0;
        uninit_copy(first, last, r);
        return r;
    }
    void _M_clear() { destroy_(_M_start, _M_finish); if (_M_start) std::free(_M_start); }
    CDefPointerVector& operator=(const CDefPointerVector& x)
    {
        if (&x != this) {
            const size_type xlen = x.size();
            if (xlen > capacity()) {
                TDefPtr* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
                _M_clear();
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                TDefPtr* i = copy_(x._M_start, x._M_finish, _M_start);
                destroy_(i, _M_finish);
            } else {
                copy_(x._M_start, x._M_start + size(), _M_start);
                uninit_copy(x._M_start + size(), x._M_finish, _M_finish);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};
static void init(CDefPointerVector& v, CDef** defs, int n, int cap)
{
    v._M_start = (TDefPtr*)std::malloc(cap * sizeof(TDefPtr));
    v._M_finish = v._M_start + n;
    v._M_end_of_storage = v._M_start + cap;
    for (int i = 0; i < n; ++i) { v._M_start[i].Object = defs[i]; addref(defs[i]); }
}
int main()
{
    CDef d[6];
    for (int i = 0; i < 6; ++i) d[i].RefCount = 0;
    CDef* s1[4] = { &d[0], &d[1], &d[2], &d[3] };
    CDef* s2[1] = { &d[4] };
    CDef* s3[2] = { &d[5], &d[0] };
    CDefPointerVector a, b, c;
    init(a, s1, 4, 4);
    init(b, s2, 1, 1);
    init(c, s3, 2, 3);
    b = a;                                   // grow: reallocate
    if (b.size() != 4 || b._M_start[3].Object != &d[3] || d[4].RefCount != 0 || d[0].RefCount != 3) { std::printf("00721150_TEST FAIL\n"); return 1; }
    a = c;                                   // shrink: assign 2, destroy 2
    if (a.size() != 2 || a._M_start[0].Object != &d[5] || d[2].RefCount != 1 || d[3].RefCount != 1 || d[5].RefCount != 2) { std::printf("00721150_TEST FAIL\n"); return 1; }
    c = b;                                   // capacity 3 < 4: reallocate
    if (c.size() != 4 || c.capacity() != 4) { std::printf("00721150_TEST FAIL\n"); return 1; }
    CDefPointerVector e;
    CDef* s4[1] = { &d[1] };
    init(e, s4, 1, 3);
    e = a;                                   // size 1 < 2 <= capacity 3: assign 1, construct 1
    if (e.size() != 2 || e._M_start[1].Object != &d[0] || e._M_end_of_storage - e._M_start != 3) { std::printf("00721150_TEST FAIL\n"); return 1; }
    e = e;
    if (e.size() != 2) { std::printf("00721150_TEST FAIL\n"); return 1; }
    std::printf("00721150_TEST PASS\n");
    return 0;
}