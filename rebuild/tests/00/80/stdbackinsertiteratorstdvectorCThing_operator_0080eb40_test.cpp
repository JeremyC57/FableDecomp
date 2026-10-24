// Behaviour test for 0x0080EB40 back_insert_iterator<vector<CThing*> >::operator= (model copy).
#include <cstdio>
#include <cstdlib>
#include <new>
struct CThing { int id; };
template <class T> struct vec {
    typedef T value_type;
    T* _M_start; T* _M_finish; T* _M_end_of_storage; int overflows;
    vec() : _M_start(0), _M_finish(0), _M_end_of_storage(0), overflows(0) { }
    ~vec() { if (_M_start) std::free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(T* position, const T& x, unsigned int fill_len)
    {
        ++overflows;
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size > fill_len ? old_size : fill_len);
        T* ns = (T*)std::malloc(len * sizeof(T));
        T* nf = ns;
        for (T* p = _M_start; p != position; ++p, ++nf) *nf = *p;
        for (unsigned int n = fill_len; n > 0; --n, ++nf) *nf = x;
        if (_M_start) std::free(_M_start);
        _M_start = ns; _M_finish = nf; _M_end_of_storage = ns + len;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) T(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1u);
    }
};
template <class C> struct back_ins {
    C* container;
    explicit back_ins(C& x) : container(&x) { }
    back_ins& operator=(const typename C::value_type& v) { container->push_back(v); return *this; }
    back_ins& operator*() { return *this; }
    back_ins& operator++() { return *this; }
};
int main()
{
    CThing t[5];
    vec<CThing*> v;
    back_ins<vec<CThing*> > it(v);
    int i, ok = 1;
    for (i = 0; i < 5; ++i) { t[i].id = i; CThing* p = &t[i]; if (&(it = p) != &it) ok = 0; }
    if (v.size() != 5u || v.overflows < 1) ok = 0;
    for (i = 0; i < 5; ++i) if (v._M_start[i] != &t[i]) ok = 0;
    if (!ok) { std::printf("0080eb40_TEST FAIL\n"); return 1; }
    std::printf("0080eb40_TEST PASS\n");
    return 0;
}