// std::vector<float>::vector(size_type n, const float& value, const allocator<float>& a) -- STLport 4.6 fill constructor:
// base-construct n elements, then fill them with the value (the POD path, fill_n)
// Leaf: ??0?$vector@MV?$allocator@M@std@@@std@@QAE@IABMABV?$allocator@M@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004516E7, length 40 bytes, `ret 0Ch` (returns this): push esi ; push [esp+10h] ;
// mov esi,ecx ; push [esp+0Ch] ; call 0x00450E3A ; push [esp+0Ch] ; mov ecx,[esi] ; mov edx,[esp+0Ch] ; call 0x0043E060 ;
// mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 0Ch.
// Callees: _Vector_base(size_t n, const allocator&) @ 0x00450E3A and the __fastcall fill_n(first, n, const value&) @ 0x0043E060
// (both COMDAT-folded with the identical 4-byte-element instantiations: _Vector_base<void*> / fill_n<unsigned long*, unsigned
// int, unsigned long>, landed under those names).
// Name (agentNF27, 2026-11-12; was the propagated stand-in CTCEnemy::SetFaction with ?SF_helper@@YGXHH@Z / ?SF_make@@YIHPAXHH@Z):
// STLport 4.6 `vector(size_type __n, const _Tp& __val, const allocator_type& __a = allocator_type()) : _Vector_base(__n, __a)
// { _M_finish = uninitialized_fill_n(_M_start, __n, __val); }` (the three stack arguments, the base call with (n, a), the fill
// call with (start, n, &value), the result stored to +4); the caller COpinionSet::COpinionSet 0x004538EB builds Ego_r.pdb's
// COpinionSet::OpinionValues (CArray<float>, a std::vector<float>) through it with n = 5. A size-optimised COMDAT like
// 0x00477428 (vector<void*> copy ctor), modelled the same way.
#include "fable_compiler.h"
#pragma optimize("s", on)

namespace std {

struct __true_type { };

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _OutputIter, class _Size, class _Tp>
_OutputIter __fastcall fill_n(_OutputIter __first, _Size __n, const _Tp& __val);                       // 0x0043E060 (float*, unsigned int, float)
template <class _ForwardIter, class _Size, class _Tp>
FABLE_FORCEINLINE _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}
template <class _ForwardIter, class _Size, class _Tp>
FABLE_FORCEINLINE _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a);                                              // 0x00450E3A
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    vector(size_type __n, const _Tp& __val, const allocator_type& __a = allocator_type());
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>::vector(size_type __n, const _Tp& __val, const allocator_type& __a)
    : _Vector_base<_Tp, _Alloc>(__n, __a)
{
    this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
}

template vector<float>::vector(unsigned int, const float&, const allocator<float>&);

}
