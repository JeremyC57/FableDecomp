// _STL::vector::_M_insert_overflow(CTextureManagerPoolInfo* position, const CTextureManagerPoolInfo& x, const __false_type&, unsigned int fill_len, bool atend) -- (vector<CTextureManagerPoolInfo>) STLport grow-and-insert: allocate old + max(old, fill_len) slots, copy the head, construct x once or fill fill_len copies, copy the tail unless inserting at the end, free the old block and adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3EA70, 232 bytes. __fastcall, `ret 14h`.
// Identity: the STLport 4.6 vector<_Tp>::_M_insert_overflow for the 20-byte CTextureManagerPoolInfo (divide by 20:
//   imul 0x66666667, sar 3); callers 0x00B3DAD4 / 0x00B3DB48 / 0x00B3DBB0 / 0x00B3EC74 are the push_back / insert /
//   resize paths of that vector (CArray<CTextureManagerPoolInfo>, whose sized ctor 0x00B3EA40 and vector(n) 0x00B3E920
//   sit beside it). The catalog label "CEngineResourceManager_ReallocateTextures" is descriptive.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, STLport __uninitialized_copy(first, last, result, const __false_type&)
//   0x00B3E340 (catalog "_Move<NThingMap::CMapwhoLevelInfo*, ...>" is propagated) and __uninitialized_fill_n(first, n, x,
//   const __false_type&) 0x00B3E390 (catalog "fill<...>" is propagated), both `__fastcall` with the empty tag pushed.
//   Retail homes those tags in the LIVE `atend` argument slot, so both callees get their real STLport bodies here as
//   same-TU noinline functions that never touch the tag.
// Shape: STLport 4.6 body (no EH: _STLP_TRY/_STLP_UNWIND compile away without /EHsc); `(max)(old, fill_len)` returns a
//   reference (the `lea`/`jb` address select); the single-element arm is `_Construct` (placement new with the null
//   test, five dword moves); _M_clear's trivial _Destroy is empty and its deallocate is the `if (p) free(p)`.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* p) throw() { return p; }

class CPixelFormat {
public:
    long FormatIndex;                                   // +0x00
    CPixelFormat() : FormatIndex(-1) { }
};

class CTextureManagerPoolInfo {                         // 20 bytes
public:
    unsigned long Width;                                // +0x00
    unsigned long Height;                               // +0x04
    CPixelFormat  PixelFormat;                          // +0x08
    bool          UseMipmaps;                           // +0x0C
    unsigned long Count;                                // +0x10
};

struct __false_type { __false_type() { } };

namespace _STL {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
// 0x00B3E340
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
// 0x00B3E390
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }   // trivially destructible element

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
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
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
}

template void _STL::vector<CTextureManagerPoolInfo>::_M_insert_overflow(CTextureManagerPoolInfo*,
    const CTextureManagerPoolInfo&, const __false_type&, unsigned int, bool);