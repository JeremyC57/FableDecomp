// std::_Vector_base<CCountedPointer<NCreatureAction::CActionEventBase>>::_Vector_base(size_t n, const allocator_type& a) -- STLport vector base sized constructor: null the three pointers, malloc n elements (none for zero), finish = start, end of storage = start + n
// Leaf: ??0?$_Vector_base@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062EE20, 56 bytes, thiscall, `ret 8` (n and the allocator reference; returns this).
// Identity (probable): its only caller is the landed 8-byte CCountedPointer vector copy constructor 0x0062EDD0, itself called only by CCreatureActionBase::CCreatureActionBase 0x0062EBC0 -- Ego_r.pdb CCreatureActionBase::ActionEvents is vector<CCountedPointer<NCreatureAction::CActionEventBase> >.
// Callee: malloc 0x00BFEA0E (`n ? malloc(n * size) : 0`, the zero register doubling as the null result).
// Shape (family K126, representative 0x005128B0): the landed G010 _Vector_base 0x004F9450 body in a speed TU (`add esp,4`).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);              // 0x00BFEA0E
namespace NCreatureAction { class CActionEventBase; }
template <class T> class CCountedPointer { public: T* Data; void* Info; };   // 8 bytes
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(size_t n, const void* = 0) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class Value, class Tp, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const MaybeReboundAlloc& a, Value p) : MaybeReboundAlloc(a), _M_data(p) {}
};
template <class T, class Alloc>
class _Vector_base {
public:
    _Vector_base(size_t n, const Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;     // +0x08
};
template class _Vector_base<CCountedPointer<NCreatureAction::CActionEventBase>, allocator<CCountedPointer<NCreatureAction::CActionEventBase> > >;
}