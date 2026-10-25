// std::vector::_M_insert_overflow(CScriptEntry* position, const CScriptEntry& x, const __false_type&, unsigned int fillLen, bool atEnd) -- (vector<CRangeCompressor::CScriptEntry>) grow: allocate size + max(size, fillLen) entries, copy the prefix, construct the fill, copy the tail unless at the end, free the old block
// Leaf: ?_M_insert_overflow@?$vector@UCScriptEntry@CRangeCompressor@@V?$allocator@UCScriptEntry@CRangeCompressor@@@std@@@std@@IAEXPAUCScriptEntry@CRangeCompressor@@ABU34@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3B330, 238 bytes (+ int3 padding), thiscall `ret 14h`.
// Identity: only caller is vector<CScriptEntry>::push_back 0x00F3B420 (landed as CRetailVector_SRecord24), used by
//   CRangeCompressor::CalcCompressionScript 0x00F3ABF0; element = Ego_r.pdb CRangeCompressor::CScriptEntry (24 bytes).
//   The catalog's "Vector_Reallocate_EHeroMorphType" is descriptive/propagated.
// Callees: malloc @ 0x00BFEA0E, __uninitialized_copy<CScriptEntry*> @ 0x00F3B1D0 (twice), __uninitialized_fill_n
//   <CScriptEntry*> @ 0x00F3B220 (landed), free @ 0x00BFEA14 (the inlined _M_clear: the trivial destroy loop vanishes).
// Shape: STLport 4.6 `_M_insert_overflow(..., const __false_type&, ...)` as 0x00F38770 (max through references, the
//   __false_type() temporaries homed on the live atEnd slot), with _M_clear and _M_set inline.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void  __cdecl free(void* p);                    // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CRangeCompressor {
public:
    struct CScriptEntry {                        // 24 bytes (Ego_r.pdb), all scalars: implicit trivial copy
        unsigned long Flags;                     // +0x00
        long          BitCount;                  // +0x04
        unsigned long RangeBias;                 // +0x08
        long          RangeShift;                // +0x0c
        unsigned long ShuffleMask;               // +0x10
        unsigned long OrMask;                    // +0x14
    };
};
typedef CRangeCompressor::CScriptEntry CScriptEntry;

namespace std {

struct __false_type { __false_type() { } };

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

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1> inline void _Copy_Construct(_T1* __p, const _T1& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)   // 0x00F3B220
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

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
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e) { this->_M_start = __s; this->_M_finish = __f; this->_M_end_of_storage._M_data = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CScriptEntry>::_M_insert_overflow(CScriptEntry*, const CScriptEntry&, const __false_type&, unsigned int, bool);

}   // namespace std