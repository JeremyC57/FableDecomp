// Behaviour test for 0x007CE1F0 __copy(CThing**, CThing**, back_insert_iterator<vector<CIntelligentPointer<CThing> > >,
// tag, int*): every pointer is wrapped and appended in order (growing when full); an empty range appends nothing; the
// returned inserter still targets the vector.
#include <cstdio>
#include <cstdlib>
#include <new>

class CThing { public: int id; };
template <class T> struct CIntelligentPointer {
    T* Item;
    CIntelligentPointer(T* item) : Item(item) { }
    CIntelligentPointer(const CIntelligentPointer& other) : Item(other.Item) { }
};
struct random_access_iterator_tag { };
template <class T> class vector {
public:
    typedef T value_type;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    int overflows;
    void _M_insert_overflow(T* position, const T& x)
    {
        ++overflows;
        unsigned int n = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = n ? 2 * n : 1;
        T* s = (T*)std::malloc(cap * sizeof(T));
        T* out = s;
        for (T* p = _M_start; p != position; ++p, ++out) new (out) T(*p);
        new (out) T(x); ++out;
        std::free(_M_start);
        _M_start = s; _M_finish = out; _M_end_of_storage = s + cap;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) T(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x);
    }
};
template <class C> class back_insert_iterator {
public:
    C* container;
    explicit back_insert_iterator(C& x) : container(&x) { }
    back_insert_iterator& operator=(const typename C::value_type& v) { container->push_back(v); return *this; }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};
template <class I, class O, class D>
O __copy(I first, I last, O result, const random_access_iterator_tag&, D*)
{
    for (D n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    CThing t[5];
    CThing* src[5];
    for (int i = 0; i < 5; ++i) { t[i].id = i; src[i] = &t[i]; }
    vector<CIntelligentPointer<CThing> > v; v._M_start = v._M_finish = v._M_end_of_storage = 0; v.overflows = 0;
    back_insert_iterator<vector<CIntelligentPointer<CThing> > > it(v);
    back_insert_iterator<vector<CIntelligentPointer<CThing> > > r = __copy(src, src + 5, it, random_access_iterator_tag(), (int*)0);
    bool ok = r.container == &v && v._M_finish - v._M_start == 5 && v.overflows > 0;
    for (int i = 0; i < 5 && ok; ++i) ok = v._M_start[i].Item == &t[i];
    r = __copy(src, src, it, random_access_iterator_tag(), (int*)0);
    ok = ok && r.container == &v && v._M_finish - v._M_start == 5;
    std::free(v._M_start);
    if (!ok) { std::printf("007ce1f0_TEST FAIL\n"); return 1; }
    std::printf("007ce1f0_TEST PASS\n");
    return 0;
}