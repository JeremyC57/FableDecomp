// Behaviour test for 0x005F8EC0 std::vector<EHeroAbility>::vector(size_type n): allocate n elements through
// _Vector_base(n, allocator()) and value-initialise them (uninitialized_fill_n 0x005F8220 with T()).
#include <cstdio>
#include <cstdlib>
#include <new>

enum EHeroAbility { ABILITY_NULL = 0, ABILITY_A = 3, EHeroAbility_Force32 = 0x7fffffff };

namespace stl {
template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
};

template <class Value, class T, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const MaybeReboundAlloc& a, Value p) : MaybeReboundAlloc(a), _M_data(p) { }
};

template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}

template <class T, class Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int n, const Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    typedef Alloc allocator_type;
    typedef unsigned int size_type;
    explicit vector(size_type n) : _Vector_base<T, Alloc>(n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};
}

int main()
{
    int ok = 1;
    // fill freshly malloc'd memory with garbage first so the zero fill is observable
    void* probe = std::malloc(5 * sizeof(EHeroAbility));
    for (int i = 0; i < 5; ++i) ((EHeroAbility*)probe)[i] = ABILITY_A;
    std::free(probe);
    stl::vector<EHeroAbility> v(5);
    if (v._M_start == 0 || v._M_finish != v._M_start + 5 || v._M_end_of_storage._M_data != v._M_start + 5) ok = 0;
    for (int i = 0; i < 5 && v._M_start; ++i)
        if (v._M_start[i] != ABILITY_NULL) ok = 0;
    std::free(v._M_start);
    stl::vector<EHeroAbility> e(0);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage._M_data != 0) ok = 0;
    if (!ok) { std::printf("005f8ec0_TEST FAIL\n"); return 1; }
    std::printf("005f8ec0_TEST PASS\n");
    return 0;
}