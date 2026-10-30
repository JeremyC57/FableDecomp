// std::vector::_M_fill_insert(std::pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>* position, unsigned int n, const std::pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>& x) -- (vector<pair<EPSCType, CComponentInstance> >) STLport insert of n copies of a particle-system component slot: slide the tail inside the spare capacity and fill the hole from a copy of x, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@U?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@V?$allocator@U?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@@2@@std@@QAEXPAU?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@2@IABU32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF7570, 328 bytes, thiscall `ret 0Ch`, no EH frame (STLport built without exceptions).
// Identity: the landed vector<pair<EPSCType, CComponentInstance> >::resize 0x00AF7890 path (NParticleEngine::CParticleSystem's
//   Components CVectorMap) binds this exact `_M_fill_insert` spelling; the landed _M_insert_overflow 0x00AF7460 (chunk 585)
//   names this row as a caller, and __uninitialized_copy 0x00AF6920 / __uninitialized_fill_n 0x00AF6980 are bound over the
//   same pair. The 0x2AAAAAAB / sar 2 reciprocal is the 24-byte pair; the inline x_copy stores the CComponentInstance vtable
//   0x0129F478 and add-refs the CCountedPointer<CPSCBase> info. STLport 4.6 vector.c verbatim.
// Callees (free helpers __fastcall): __uninitialized_copy 0x00AF6920 (twice), __copy_backward<..., int> 0x00AF6E10 (five
//   arguments: behind the inline __copy_backward_ptrs), __uninitialized_fill_n 0x00AF6980 (behind the inline
//   uninitialized_fill_n), fill 0x00AF6D90 (both fills tail-merged), ~pair 0x00AF65B0 (x_copy, out of line),
//   _M_insert_overflow 0x00AF7460.
// Shape: x_copy (24 bytes) is the whole frame; retail homes the `__false_type()` / iterator tags on the dead x slot (finish - n
//   and n - elems_after are stored there and re-read), so the tag-taking helpers get same-TU noinline bodies that never touch
//   the tag (the live-slot rule of 0x00BCDCA0). The pair's destructor is declared (out of line in retail, 0x00AF65B0).
// Layout: CComponentInstance (Ego_r.pdb, 0x14): CBaseClass vptr, Component CCountedPointer<CPSCBase> +4, Enabled +0xC,
//   InstanceName +0x10; base CBaseClass from the shared engine/CBaseClass.h (no engine header for CComponentInstance,
//   CCountedPointer or CCPPointerInfo; the landed 0x00AF7460 model).
inline void* operator new(unsigned int, void* p) { return p; }

#include "engine/CBaseClass.h"

class CCPPointerInfo {
public:
    unsigned long RefCount;                       // +0
};

template <class T> class CCountedPointer {
public:
    T*              Data;                         // +0
    CCPPointerInfo* Info;                         // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    CCountedPointer& operator=(const CCountedPointer& other);
};

namespace NParticleEngine {
enum EPSCType { PSC_TYPE_FIRST = 0 };
class CPSCBase;
class CComponentInstance : public CBaseClass {    // vtable 0x0129F478
public:
    CCountedPointer<CPSCBase> Component;          // +0x04
    bool                      Enabled;            // +0x0c
    unsigned long             InstanceName;       // +0x10
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    ~pair();                                               // pair<EPSCType, CComponentInstance>: 0x00AF65B0 (out of line)
};
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
{                                                                                              // 0x00AF6920
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x00AF6980
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
{                                                                                              // 0x00AF6E10
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
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                     // 0x00AF6D90

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
                            bool __atend = false);                       // 0x00AF7460
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

typedef std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> CComponentPair;
template void std::vector<CComponentPair>::_M_fill_insert(CComponentPair*, unsigned int, const CComponentPair&);