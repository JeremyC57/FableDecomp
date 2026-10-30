// std::vector::_M_fill_insert(NParticleEngine::CParticleEmitterInfo* position, unsigned int n, const NParticleEngine::CParticleEmitterInfo& x) -- (vector<NParticleEngine::CParticleEmitterInfo>) STLport insert of n copies of a 12-byte particle emitter info: slide the tail inside the spare capacity and fill the hole from a copy of x, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCParticleEmitterInfo@NParticleEngine@@V?$allocator@VCParticleEmitterInfo@NParticleEngine@@@std@@@std@@QAEXPAVCParticleEmitterInfo@NParticleEngine@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2010, 318 bytes, thiscall `ret 0Ch`, no EH frame (STLport built without exceptions).
// Identity: the landed vector<NParticleEngine::CParticleEmitterInfo>::resize(n, x) 0x00AF2170 (CParticleDataBank::
//   ParticleEmitters, Ego_r.pdb CArray<NParticleEngine::CParticleEmitterInfo>; FableWin.pdb names that vector's resize) calls
//   this row through the inline `insert(end(), n, x)` and binds this exact `_M_fill_insert` spelling; the landed
//   _M_insert_overflow 0x00AF1E90, __uninitialized_copy 0x00AF1C10 and __uninitialized_fill_n 0x00AF1AA0 (both bound over
//   CParticleEmitterInfo) are this row's callees. The 0x2AAAAAAB / sar 1 reciprocal is the 12-byte element
//   (Data CIVCountedPointer<CEmitterData> +0, Bank +4, Index +8). STLport 4.6 vector.c verbatim.
// Callees (free helpers __fastcall): __uninitialized_copy 0x00AF1C10 (twice), __copy_backward<..., int> 0x00AF1B30 (five
//   arguments: behind the inline __copy_backward_ptrs), __uninitialized_fill_n 0x00AF1AA0 (behind the inline
//   uninitialized_fill_n), fill 0x00AF1C70 (both fills tail-merged), _M_insert_overflow 0x00AF1E90; the pointee's virtual
//   Delete (CIVCountedPointeeBase slot 1) from x_copy's inline ~CIVCountedPointer.
// Shape: x_copy (12 bytes) is the whole frame; retail homes the `__false_type()` / iterator tags on the dead x slot (finish - n
//   and n - elems_after are stored there and re-read), so the out-of-line helpers get same-TU noinline bodies that never touch
//   the tag (the live-slot rule of 0x00BCDCA0). x_copy is built by the implicit element copy with the intrusive pointer's
//   user copy ctor (`inc [obj+4]`) and released at the shared exit (`dec [obj+4] ; jne ; call [vt+4]`).
//   fill (which gets &x_copy) needs its same-TU noinline body too: declared only, VC7.1 treats x_copy as clobbered and
//   splits the release into `mov/dec/mov` (330v318). The fill and copy helpers' bodies here are not graded (landed rows).
// Shared header: engine/CIVCountedPointeeBase.h retail opt-in (slot 1 Delete, AddRef / Release accessors, IVRefCount +4).
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"

inline void* operator new(unsigned int, void* p) { return p; }

template <class T> class CIVCountedPointer {               // Ego_r.pdb, 4 bytes
public:
    CIVCountedPointeeBase* Object;                         // +0x0
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
    ~CIVCountedPointer()
    {
        if (Object) {
            if (Object->Release() == 0)
                Object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (Object->Release() == 0)
                    Object->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
};

namespace NParticleEngine {
class CParticleDataBank;
class CParticleEmitterInfo {                               // Ego_r.pdb, 12 bytes
public:
    class CEmitterData;
    CIVCountedPointer<CEmitterData> Data;                  // +0x0
    CParticleDataBank*              Bank;                  // +0x4
    unsigned long                   Index;                 // +0x8
};
}

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00AF1C10
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x00AF1AA0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x00AF1B30
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                              // 0x00AF1C70
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x00AF1E90
public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}

template void std::vector<NParticleEngine::CParticleEmitterInfo>::_M_fill_insert(NParticleEngine::CParticleEmitterInfo*,
                                                                                 unsigned int,
                                                                                 const NParticleEngine::CParticleEmitterInfo&);