#include <cstdio>
#include <stdlib.h>
namespace NHeroInformationScreens { class CBase; }
namespace std {
template <class T> class allocator {
public:
    allocator() {}
    __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};
}
template class std::_Vector_base<const NHeroInformationScreens::CBase*, std::allocator<const NHeroInformationScreens::CBase*> >;

int main()
{
    typedef const NHeroInformationScreens::CBase* P;
    std::allocator<P> a;
    std::_Vector_base<P, std::allocator<P> > b(11, a);
    bool ok = b._M_start != 0 && b._M_finish == b._M_start && b._M_end_of_storage._M_data == b._M_start + 11;
    free(b._M_start);
    std::_Vector_base<P, std::allocator<P> > z(0, a);
    ok = ok && z._M_start == 0 && z._M_finish == 0 && z._M_end_of_storage._M_data == 0;
    std::printf(ok ? "0066ec8a_TEST PASS\n" : "FAIL\n");
    return 0;
}