// std::_Vector_base::_Vector_base(size_t n, const allocator_type& a) -- (_Vector_base<EOpinionAttitudeType >) STLport vector base sized constructor: null the three pointers, malloc n elements (none for zero), finish = start, end of storage = start + n
// Leaf: ??0?$_Vector_base@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471FD9, 52 bytes (unaligned entry, size TU), thiscall, `ret 8` (n and the
//   allocator reference; returns this).
// Identity: element EOpinionAttitudeType (4 bytes: `shl eax,2` / `lea [eax+edi*4]`) -- proven; evidence: its only caller is the landed vector<EOpinionAttitudeType> copy constructor 0x00471F97 (COpinionDeedReactionDef::Attitudes, Ego_r vector<EOpinionAttitudeType>).
//   Family G010 (10 bodies, 4- and 8-byte elements, each its own COMDAT: retail has no ICF, so each address is one element
//   type); the manifest label "Vector_InitPointerStorage" is propagated/behavioural.
// Callees: malloc 0x00BFEA0E (`__cdecl`, `pop ecx`) through the inline allocator::allocate (`n ? malloc(n * size) : 0`; the
//   zero register doubles as the null result).
// Body: STLport 4.x _vector.h
//     _Vector_base(size_t __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
//     { _M_start = _M_end_of_storage.allocate(__n); _M_finish = _M_start; _M_end_of_storage._M_data = _M_start + __n; }
#pragma optimize("s", on)
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);              // 0x00BFEA0E
enum EOpinionAttitudeType { OPINION_ATTITUDE_TYPE_0 };
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
typedef EOpinionAttitudeType _Elem;
template class _Vector_base<_Elem, allocator<_Elem> >;
}