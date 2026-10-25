// Behaviour test for 0x004CD9D0: STLport _Vector_base<unsigned long>::_Vector_base(n, a) -- allocate room for n
// elements (none for n == 0), start == finish, end of storage n elements on.
#include <cstdio>
#include <cstdlib>

static int gMallocs = 0;
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { if (__n != 0) ++gMallocs; return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
}
typedef std::_Vector_base<unsigned long, std::allocator<unsigned long> > TBase;

int main()
{
    std::allocator<unsigned long> alloc;
    TBase b(10, alloc);
    bool ok = b._M_start != 0 && b._M_finish == b._M_start && b._M_end_of_storage._M_data - b._M_start == 10 && gMallocs == 1;
    for (int i = 0; ok && i < 10; ++i)
        b._M_start[i] = (unsigned long)(i * 3);
    for (int i = 0; ok && i < 10; ++i)
        if (b._M_start[i] != (unsigned long)(i * 3))
            ok = false;
    std::free(b._M_start);
    TBase z(0, alloc);
    ok = ok && z._M_start == 0 && z._M_finish == 0 && z._M_end_of_storage._M_data == 0 && gMallocs == 1;
    if (!ok) {
        std::printf("004cd9d0_TEST FAIL\n");
        return 1;
    }
    std::printf("004cd9d0_TEST PASS\n");
    return 0;
}