// Behaviour test for 0x005F9BFE std::vector<pair<long, CDefPointer<const CHeroAbilityDef> > >::vector(n, value, a):
// allocate n pairs through _Vector_base(n, a) and copy-construct each from value (one add-ref per copy).
#include <cstdio>
#include <cstdlib>
#include <new>

struct CDefPointeeBase {
    virtual void v00() { }
    virtual void Destroy() const { }
    mutable long m_refCount;
    CDefPointeeBase() : m_refCount(0) { }
    virtual ~CDefPointeeBase() { }
};
struct CHeroAbilityDef : CDefPointeeBase { };

template <class T> struct CDefPointer {
    T* Object;
    explicit CDefPointer(T* p) : Object(p) { if (Object) ++Object->m_refCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
};

namespace stl {
struct __false_type { __false_type() { } };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
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
ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (&*cur) T(x);
    return cur;
}
template <class ForwardIter, class Size, class T>
inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}
template <class T, class Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int n, const Alloc& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
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
    vector(size_type n, const T& value, const allocator_type& a) : _Vector_base<T, Alloc>(n, a)
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, value);
    }
};
}

typedef CDefPointer<const CHeroAbilityDef> DP;
typedef stl::pair<long, DP> P;

int main()
{
    int ok = 1;
    CHeroAbilityDef def;
    {
        P value(7, DP(&def));                              // value holds one reference (the temporary's is the second)
        long base = def.m_refCount;
        stl::vector<P> v(4, value, stl::allocator<P>());
        if (v._M_finish != v._M_start + 4 || v._M_end_of_storage._M_data != v._M_start + 4) ok = 0;
        for (int i = 0; i < 4; ++i)
            if (v._M_start[i].first != 7 || v._M_start[i].second.Object != &def) ok = 0;
        if (def.m_refCount != base + 4) ok = 0;
        std::free(v._M_start);
    }
    if (!ok) { std::printf("005f9bfe_TEST FAIL\n"); return 1; }
    std::printf("005f9bfe_TEST PASS\n");
    return 0;
}