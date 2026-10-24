// std::vector<CIntelligentPointer<CParticleEmitter> >::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path: allocate size + max(size, n), copy-construct prefix, n copies of x and (unless at the end) the suffix, release the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x006566C3, 172 bytes, thiscall `ret 14h` (position, x, the empty tag by reference,
// fillLength, atEnd). Only caller: the overflow branch of vector<CIntelligentPointer<CParticleEmitter>>::_M_fill_insert
// @ 0x006568F2 (call at 0x006569D0), so the element is the 8-byte CIntelligentPointer<NParticleEngine::CParticleEmitter>
// (copy constructor 0x00654DAF, vtable 0x0125A26C).
// Callees: malloc @ 0x00BFEA0E (allocate, inline), __uninitialized_copy(first, last, result, const __false_type&)
// @ 0x006556A8 (`__fastcall`), __uninitialized_fill_n(first, n, x, const __false_type&) @ 0x00654E90 (`__fastcall`),
// the CIntelligentPointer copy constructor @ 0x00654DAF (the inline `_Copy_Construct` of the fillLength == 1 path),
// vector::_M_clear @ 0x0065605F (destroy [start, finish) then free; manifest "CActiveFile::OnReadFinished" is propagated).
// Label correction: the catalog "std_vector_insert_IntelligentPointer_range" is descriptive; this is STLport 4.x
// `vector::_M_insert_overflow(.., const __false_type&, ..)`.
// Shape: STLport verbatim with no `_STLP_TRY` (exceptions off): len = size + max(size, n) through the const-reference
// `(max)` (size in [ebp-4], `lea` select), allocate, copy the prefix, construct one copy (or fill n), copy the suffix
// unless atEnd, _M_clear, _M_set. The empty `__false_type` temporaries sit in the dead high byte of the atEnd slot
// ([ebp+1Bh]): reproduced by same-TU `auto_inline(off)` noinline template bodies of the two helpers whose tag parameter
// is unnamed (GOTCHAS empty-tag rule).
// Opt: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CBaseObject;
namespace NParticleEngine { class CParticleEmitter; }

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();               // 0x00A01C10
    CBaseObject* PItem;                               // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer<T>& other);   // 0x00654DAF
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Copy_Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Copy_Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    __forceinline _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    pointer _M_start;                                                   // +0x00
    pointer _M_finish;                                                  // +0x04
    _STLP_alloc_proxy<pointer, _Tp, allocator<_Tp> > _M_end_of_storage; // +0x08
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear();                                                    // 0x0065605F
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage._M_data = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::_M_insert_overflow(
    CIntelligentPointer<NParticleEngine::CParticleEmitter>*, const CIntelligentPointer<NParticleEngine::CParticleEmitter>&,
    const __false_type&, unsigned int, bool);
}