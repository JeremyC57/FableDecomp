// std::vector::_M_insert_overflow(pair<EPSCType,CComponentInstance>* __position, const pair<EPSCType,CComponentInstance>& __x, const __false_type&, unsigned int __fill_len, bool __atend) -- (vector<pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> >) STLport 4.6 growth insert: allocate old + max(old, n) elements, copy the prefix, one or n copies of x and (unless at the end) the suffix, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@V?$allocator@U?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@@2@@std@@IAEXPAU?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@2@ABU32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF7460, length 260 bytes, thiscall, `ret 14h`.
// Identity: callers 0x00AF76AC (in 0x00AF7570, the _M_fill_insert of the same vector) and 0x00AF7872 (in 0x00AF7730, its
//   single-element insert); every callee is a landed member of the same instantiation: __uninitialized_copy 0x00AF6920,
//   __uninitialized_fill_n 0x00AF6980, __destroy_aux 0x00AF6F90 (all pair<EPSCType, CComponentInstance>, the vector inside
//   NParticleEngine::CParticleSystem's Components CVectorMap). The 24-byte element copy inlined at the fill_len == 1 arm
//   (first, CComponentInstance vtable 0x0129F478, the CCountedPointer<CPSCBase> copy with its addref, Enabled, InstanceName)
//   is the same implicit pair copy those landed rows model. The catalog label "Vector_InsertWithCount_24Byte" is descriptive.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14 (the inline _M_clear's deallocate), and the three helpers above (`__fastcall`).
// Shape: STLport 4.6 vector<_Tp>::_M_insert_overflow(__false_type) verbatim (no _STLP_TRY). Retail homes every __false_type
//   temporary in the __atend argument slot while __atend is still live (it is tested after the fill), which VC7.1 only does
//   when it can see that the tag-taking callees never read the tag: the three helpers are given their real STLport bodies
//   in this TU under auto_inline(off) + noinline (as the landed erase 0x00BCDB10 does for __copy). _M_clear inlines
//   `_Destroy(start, finish)` (the out-of-line __destroy_aux) and the allocator's null-tested free.
// Layout: CComponentInstance (Ego_r.pdb, 0x14): CBaseClass vptr, Component CCountedPointer<CPSCBase> +4, Enabled +0xC,
//   InstanceName +0x10; RTTI .?AVCComponentInstance@NParticleEngine@@ has the single base CBaseClass (rtti_bases.tsv), so
//   the shared engine/CBaseClass.h is the base (VC7.1 drops the dead CBaseClass vtable store of the implicit copy).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
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

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>                                      // 0x00AF6920 (same-TU body)
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>                                // 0x00AF6980 (same-TU body)
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIterator>                                                    // 0x00AF6F90 (same-TU body)
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    __forceinline void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    __forceinline size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    __forceinline void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

} // namespace std

typedef std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> CComponentPair;
template void std::vector<CComponentPair>::_M_insert_overflow(CComponentPair*, const CComponentPair&, const std::__false_type&, unsigned int, bool);