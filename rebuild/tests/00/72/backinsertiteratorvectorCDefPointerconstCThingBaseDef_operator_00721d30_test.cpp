// Behaviour test for 0x00721D30 back_insert_iterator<vector<CDefPointer<..> > >::operator= (growth modelled).
#include <cstdio>
#include <cstdlib>
#include <new>
struct CDef { long RefCount; };
struct TDefPtr {
    CDef* Object;
    explicit TDefPtr(CDef* p) : Object(p) { if (Object) ++Object->RefCount; }
    TDefPtr(const TDefPtr& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
};
struct CDefPointerVector {
    TDefPtr* _M_start; TDefPtr* _M_finish; TDefPtr* _M_end_of_storage;
    int overflows;
    void _M_insert_overflow(TDefPtr* pos, const TDefPtr& x, unsigned int fill_len, bool atend)
    {
        unsigned int n = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = n ? 2 * n : 1;
        TDefPtr* p = (TDefPtr*)std::malloc(cap * sizeof(TDefPtr));
        for (unsigned int i = 0; i < n; ++i) p[i].Object = _M_start[i].Object;
        new (p + n) TDefPtr(x);
        std::free(_M_start);
        _M_start = p; _M_finish = p + n + fill_len; _M_end_of_storage = p + cap;
        ++overflows;
        (void)pos; (void)atend;
    }
    void push_back(const TDefPtr& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) TDefPtr(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
};
struct back_insert_iterator {
    CDefPointerVector* container;
    back_insert_iterator& operator=(const TDefPtr& value) { container->push_back(value); return *this; }
};
int main()
{
    CDef d; d.RefCount = 1;
    CDefPointerVector v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0; v.overflows = 0;
    back_insert_iterator it; it.container = &v;
    TDefPtr p(&d);
    for (int i = 0; i < 5; ++i) {
        back_insert_iterator& r = (it = p);
        if (&r != &it) { std::printf("00721d30_TEST FAIL\n"); return 1; }
    }
    if (v._M_finish - v._M_start != 5 || d.RefCount != 1 + 1 + 5 || v.overflows != 4 || v._M_start[4].Object != &d) { std::printf("00721d30_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    std::printf("00721d30_TEST PASS\n");
    return 0;
}