// Behaviour test for 0x00721F70 __copy(const CDefPointer*, const CDefPointer*, back_insert_iterator, tag, int*).
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
    void _M_insert_overflow(TDefPtr* pos, const TDefPtr& x, unsigned int fill_len, bool atend)
    {
        unsigned int n = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = n ? 2 * n : 2;
        TDefPtr* p = (TDefPtr*)std::malloc(cap * sizeof(TDefPtr));
        for (unsigned int i = 0; i < n; ++i) p[i].Object = _M_start[i].Object;
        new (p + n) TDefPtr(x);
        std::free(_M_start);
        _M_start = p; _M_finish = p + n + fill_len; _M_end_of_storage = p + cap;
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
    explicit back_insert_iterator(CDefPointerVector& x) : container(&x) {}
    back_insert_iterator& operator=(const TDefPtr& value) { container->push_back(value); return *this; }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};
struct random_access_iterator_tag { };
static back_insert_iterator copy_(const TDefPtr* first, const TDefPtr* last, back_insert_iterator result, const random_access_iterator_tag&, int*)
{
    for (long n = (long)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
int main()
{
    CDef d[3];
    for (int i = 0; i < 3; ++i) d[i].RefCount = 0;
    TDefPtr* src = (TDefPtr*)std::malloc(5 * sizeof(TDefPtr));
    for (int i = 0; i < 5; ++i) new (src + i) TDefPtr(&d[i % 3]);
    CDefPointerVector v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    back_insert_iterator r = copy_(src, src + 5, back_insert_iterator(v), random_access_iterator_tag(), (int*)0);
    if (r.container != &v || v._M_finish - v._M_start != 5 || v._M_start[3].Object != &d[0] || d[0].RefCount != 4 || d[2].RefCount != 2) { std::printf("00721f70_TEST FAIL\n"); return 1; }
    back_insert_iterator r2 = copy_(src, src, back_insert_iterator(v), random_access_iterator_tag(), (int*)0);
    if (r2.container != &v || v._M_finish - v._M_start != 5) { std::printf("00721f70_TEST FAIL\n"); return 1; }
    std::free(v._M_start); std::free(src);
    std::printf("00721f70_TEST PASS\n");
    return 0;
}