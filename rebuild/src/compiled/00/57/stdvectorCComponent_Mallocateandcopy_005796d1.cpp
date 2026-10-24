// std::vector<CComponent*>::_M_allocate_and_copy(size_type n, const_pointer first, const_pointer last) -- STLport: allocate n pointers and memmove the source range into them
// Leaf: ??$_M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x005796D1, 45 bytes, thiscall `ret 0Ch` (`this` unused: the allocator is
//   empty). Called by the vector's operator= (0x005798C8) when the source outgrows the capacity; that assignment
//   is applied to CTCInventoryExperience::ExpSliders (retail +0x23C, vector<NUISystem::CComponent*>; landed ctor
//   0x0057832D) from CTCInventoryExperience slots 86/87 (0x00577216, 0x00577AC0). The catalog
//   "Vector_AllocAndMove" is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocator::allocate, null for n == 0), STLport __copy_trivial @ 0x00405BA0
//   (__fastcall first/last + result: `last == first ? result : memmove(...) + n`, out of line in retail).
// Shape: STLport 4.x verbatim: `pointer __result = _M_end_of_storage.allocate(__n); __uninitialized_copy(__first,
//   __last, __result, _IsPODType()); return __result;` -- the pointer element's __true_type traits reduce the copy
//   to __copy_aux -> __copy_ptrs -> __copy_trivial; the copy's return value is dropped. Size TU (`pop ecx`).
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

namespace NUISystem { class CComponent; }

namespace std {

struct __true_type { };
struct __false_type { };

void* __fastcall __copy_trivial(const void* __first, const void* __last, void* __result);   // 0x00405BA0

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return __copy_ptrs(__first, __last, __result, __true_type());
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                         const __true_type&)
{
    return __copy_aux(__first, __last, __result, __true_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    typedef __true_type _IsPODType;                 // __type_traits<_Tp*>::is_POD_type
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
};

template NUISystem::CComponent** vector<NUISystem::CComponent*>::_M_allocate_and_copy(
    unsigned int, NUISystem::CComponent* const*, NUISystem::CComponent* const*);

}