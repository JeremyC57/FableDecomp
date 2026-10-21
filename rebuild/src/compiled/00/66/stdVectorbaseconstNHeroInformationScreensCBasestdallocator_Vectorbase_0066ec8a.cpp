// std::_Vector_base<const NHeroInformationScreens::CBase*, std::allocator<...> >::_Vector_base(unsigned int n, const allocator& a) -- allocate room for n pointers
// Leaf: ??0?$_Vector_base@
// Retail: Fable.exe @ 0x0066EC8A, 50 bytes, `ret 8`; only caller the vector(n, value, alloc) ctor 0x0066ECBE (then
// uninitialized_fill_n 0x0066EC31), itself called from the NHeroInformationScreens::CManager ctor (0x0066EAD5) for the
// Screens vector (+4, Ego_r.pdb vector<const NHeroInformationScreens::CBase*>) with n = 11. STLport: zero the three
// pointers, _M_start = allocate(n) (n ? malloc(n * 4) : 0, malloc = 0x00BFEA0E), _M_finish = _M_start,
// _M_end_of_storage = _M_start + n. Catalog label Vector_AllocateUints is descriptive only.
// Layout: STLport _Vector_base {_M_start +0, _M_finish +4, _STLP_alloc_proxy _M_end_of_storage +8} (12 bytes).
// Opt: size TU (unaligned entry, `pop ecx` after the cdecl malloc); allocate() is __forceinline -- plain inline stays a call.
#pragma optimize("s", on)
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