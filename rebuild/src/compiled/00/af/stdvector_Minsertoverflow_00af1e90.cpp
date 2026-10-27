// std::vector::_M_insert_overflow(NParticleEngine::CParticleEmitterInfo* position, const CParticleEmitterInfo& x, const __false_type&, unsigned int fill_len, bool atend) -- grow a particle data bank's emitter-info vector into a size + max(size, n) block with x inserted fill_len times at position
// Leaf: ?_M_insert_overflow@?$vector@VCParticleEmitterInfo@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF1E90, 241 bytes, thiscall `ret 14h`.
// Identity: the 12-byte element (`imul 2AAAAAABh ; sar edx,1`) is NParticleEngine::CParticleEmitterInfo
//   { CIVCountedPointer<CEmitterData> Data; CParticleDataBank* Bank; unsigned long Index; } (Ego_r.pdb): the helpers it
//   calls are the landed CParticleEmitterInfo __uninitialized_copy 0x00AF1C10 (whose notes name this caller),
//   __uninitialized_fill_n 0x00AF1AA0 and __destroy_aux 0x00AF1D90 (CParticleDataBank::ParticleEmitters). Single
//   caller 0x00AF2142 (push_back). The catalog's "Vector_InsertRange" is descriptive.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14 and the three above.
// Shape: STLport 4.6 verbatim (as this chunk's 0x00AD83D0) with the copy/fill/destroy helpers as same-TU noinline
//   bodies (each `__false_type()` tag homed on the `atend` slot) and the fill_len == 1 copy inline: the intrusive
//   pointer's user copy constructor (copy, null test, `inc [obj+4]`) then the two plain words.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CIVCountedPointeeBase {
public:
    virtual void v00();
    unsigned long IVRefCount;                               // +0x4
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;                          // +0x0
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVCountedPointer();
};

namespace NParticleEngine {
class CParticleDataBank;
class CParticleEmitterInfo {                                // Ego_r.pdb, 12 bytes
public:
    class CEmitterData;
    CIVCountedPointer<CEmitterData> Data;                   // +0x0
    CParticleDataBank*              Bank;                   // +0x4
    unsigned long                   Index;                  // +0x8
};
}

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// The helpers' real bodies (landed 0x00AF1C10 / 0x00AF1AA0 / 0x00AF1D90) sit in this TU as in retail; out of line
// (auto_inline off + noinline) they let VC7.1 home each __false_type() tag on the dead `atend` slot. Not graded here.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00AF1C10
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                                                              // 0x00AF1AA0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{                                                                                              // 0x00AF1D90
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
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
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
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

}   // namespace std

template void std::vector<NParticleEngine::CParticleEmitterInfo>::_M_insert_overflow(
    NParticleEngine::CParticleEmitterInfo*, const NParticleEngine::CParticleEmitterInfo&, const std::__false_type&,
    unsigned int, bool);