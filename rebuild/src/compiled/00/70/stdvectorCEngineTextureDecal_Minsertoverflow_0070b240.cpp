// std::vector<CEngineTextureDecal>::_M_insert_overflow(pointer pos, const CEngineTextureDecal& x, const __false_type&, size_type fill_len, bool atend) -- STLport grow-and-insert of engine texture decals: allocate old + max(old, fill_len), copy the head, construct fill_len copies, copy the tail unless appending, free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B240, length 218 bytes (+ int3 padding). __fastcall (this in ecx), `ret 14h`.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E, __uninitialized_copy<CEngineTextureDecal*> @ 0x0070AA50 (twice),
//          vector<CCompositeBlendDesc>::vector(const vector&) @ 0x005BB346 (the single-element construct; its manifest
//          label "std::vector::operator=" is propagated), __uninitialized_fill_n<CEngineTextureDecal*> @ 0x0070AAF0,
//          vector<CEngineTextureDecal>::_M_clear() @ 0x0070B1C0.
// Identity: the manifest label "Vector_Reallocate_RandomAppearance" is propagated. Callers: the inline push_back of
//   CTCTextureDecal::PeekEngineDecals (0x0070A6DC) and the out-of-line push_back 0x0070B350; the element is the 20-byte
//   retail CEngineTextureDecal {OriginalTextureIndex, DecalID, vector<CCompositeBlendDesc> OverlaidBlends} (`imul
//   66666667h ; sar edx,3` = divide by 20).
// Shape: STLport 4.x `_M_insert_overflow(pos, x, __false_type, fill_len, atend)` verbatim: `__old_size = size()` lives in the
//   `push ecx` word because `(max)(__old_size, __fill_len)` takes both by const reference (`lea ecx,[..] ; jb`), `allocate(len)`
//   is `len != 0 ? malloc(len * 20) : 0`, the four helper calls pass the empty __false_type temporary by reference (homed
//   in the dead `atend` byte's slot), the single-element case is the inline `_Construct` (null test, two words, the
//   out-of-line blend-list copy), then `_M_clear()` and `_M_set(new_start, new_finish, new_start + len)`.
//   The two algorithm callees are same-TU stand-in bodies under auto_inline(off): with declarations only VC7.1 gives the
//   first tag temporary its own frame byte (`sub esp,8`, `lea ecx,[esp+13h]`) instead of the dead `atend` slot.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (import thunk)
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };

struct CCompositeBlendDesc;
struct CCompositeBlendDescVector {              // vector<CCompositeBlendDesc> (retail STLport, 12 bytes)
    CCompositeBlendDesc* _M_start;
    CCompositeBlendDesc* _M_finish;
    CCompositeBlendDesc* _M_end_of_storage;
    CCompositeBlendDescVector(const CCompositeBlendDescVector& __x);   // 0x005BB346
};

struct CEngineTextureDecal {                    // Ego_r.pdb; 20 bytes retail
    long OriginalTextureIndex;                  // +0x00
    long DecalID;                               // +0x04
    CCompositeBlendDescVector OverlaidBlends;   // +0x08
};

template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// Same-TU stand-in bodies (the real ones are out of line in this TU): VC7.1 uses what it learns from them.
#pragma auto_inline(off)
namespace std {
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)   // 0x0070AA50
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)   // 0x0070AAF0
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}
#pragma auto_inline(on)

template <class _Tp> struct allocator {        // std::allocator (empty)
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Alloc> struct _STLP_alloc_proxy : _Alloc {   // the end-of-storage pointer
    _Value _M_data;
};

template <class _Tp> struct vector {           // STLport vector (12 bytes)
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, allocator<_Tp> > _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear();                            // 0x0070B1C0
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    _Tp* __new_start = _M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __new_start;
    __new_finish = std::__uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = std::__uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = std::__uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template struct vector<CEngineTextureDecal>;