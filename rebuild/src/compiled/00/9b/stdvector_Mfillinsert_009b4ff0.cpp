// std::vector::_M_fill_insert(pointer position, size_type n, const value_type& x) -- (vector<CDefinitionManager::CInstantiatedDefInfo*>) STLport insert of n copies of a 4-byte POD: memmove the tail inside the spare capacity, else grow through the __true_type _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@PAVCInstantiatedDefInfo@CDefinitionManager@@V?$allocator@PAVCInstantiatedDefInfo@CDefinitionManager@@@std@@@std@@QAEXPAPAVCInstantiatedDefInfo@CDefinitionManager@@IABQAV34@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B4FF0, 276 bytes (+ int3 padding), thiscall `ret 0Ch`.
// Identity (proven): vector<CDefinitionManager::CInstantiatedDefInfo*>::_M_fill_insert; evidence: its caller resize 0x009B70D0 is applied by CDefinitionManager::LoadBinaryDefinitions 0x009AFB90 to this+0xA4 -- Ego_r.pdb's CDefinitionManager::InstantiatedDefs (+0xBC behind larger debug containers), CArray<CDefinitionManager::CInstantiatedDefInfo*>, the class's only vector of 4-byte elements (SymbolPathList / CompilePathList hold CWideStrings, CompressedDefData bytes)
// Label: the manifest's "Vector_InsertWithFill_ENavigatorType" is propagated (family H082: six byte-identical bodies, one COMDAT per element type;
//   retail has no identical-code folding, so each address is exactly one instantiation); the body is STLport 4.6
//   `vector<_Tp>::_M_fill_insert` verbatim, family pass 2's G001 model.
// Callees: memmove 0x00BFEAE6 (the __true_type __uninitialized_copy / __copy_backward_ptrs via __copy_trivial and
//   __copy_trivial_backward), fill<CDefinitionManager::CInstantiatedDefInfo**, CDefinitionManager::CInstantiatedDefInfo*> 0x009B15F0 (`__fastcall`, `ret 4`), _M_insert_overflow(.., const __true_type&, ..)
//   0x009B4BD0 (protected, as in STLport 4.6; atend defaulted false: `push 0`).
// Shape: the inline uninitialized_fill_n -> __uninitialized_fill_n(__true_type) -> fill_n loop of a 4-byte value becomes
//   `rep stosd`; fill is a same-TU noinline `__fastcall` body under auto_inline(off) (retail calls it out of line).
class CDefinitionManager { public: class CInstantiatedDefInfo; };
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                         const __true_type&)
{
    return (_ForwardIter)__copy_trivial(__first, __last, __result);
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __true_type&)
{
    return (_BidirectionalIter2)__copy_trivial_backward(__first, __last, __result);
}

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
}

#pragma auto_inline(off)
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x009B15F0
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __true_type _TrivialAss;
    typedef __true_type _IsPODType;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x009B4BD0
public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<CDefinitionManager::CInstantiatedDefInfo* >::_M_fill_insert(CDefinitionManager::CInstantiatedDefInfo**, unsigned int, CDefinitionManager::CInstantiatedDefInfo* const&);
}