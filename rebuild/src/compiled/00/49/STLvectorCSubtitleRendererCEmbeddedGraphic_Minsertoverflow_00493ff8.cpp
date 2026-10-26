// std::vector<CSubtitleRenderer::CEmbeddedGraphic>::_M_insert_overflow(CEmbeddedGraphic*, const CEmbeddedGraphic&, const __false_type&, unsigned int, bool) -- STLport reallocating insert of 16-byte embedded-graphic records: allocate old_size + max(old_size, n), copy the head, construct the n new records, copy the tail unless appending, free the old block
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- same-TU auto_inline(off) __uninitialized_copy/__uninitialized_fill_n bodies + optimize(y off), the landed twin 0x00580A7B shape: the tag temporaries pack into the __atend slot padding
// Leaf: ?_M_insert_overflow@?$vector@VCEmbeddedGraphic@CSubtitleRenderer@@@std@@QAEXPAVCEmbeddedGraphic@CSubtitleRenderer@@ABV34@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493FF8, 180 bytes, `ret 14h` (__fastcall; size-optimised TU with an ebp
//   frame: `#pragma optimize("s", on)`).
// Callees: 0x00BFEA0E malloc / 0x00BFEA14 free (STLport's malloc allocator: `n != 0 ? malloc(n * 16) : 0`, `if (p) free(p)`),
//   0x00493959 __uninitialized_copy (landed `UninitCopy_CElement16`, __fastcall first/last, result + tag on the stack),
//   0x004935F8 CEmbeddedGraphic copy constructor (landed as GFToScreen, a 4-dword copy), 0x00493984
//   __uninitialized_fill_n (landed `UninitFillN_CElement16`).
// Caller: the landed push_back 0x0049417D (`_M_insert_overflow(_M_finish, x, __false_type(), 1, true)`), reached from
//   CSubtitleRenderer::ProcessEmbeddedGraphicTags -> vector<CSubtitleRenderer::CEmbeddedGraphic> EmbeddedGraphics.
// Shape: STLport 4.x `_M_insert_overflow` (non-POD path): `len = old_size + (max)(old_size, fill_len)` (max by reference:
//   `lea ecx,[ebp+14h] ; jb ; lea ecx,[ebp-4]`), new_start reusing the old_size frame word, the `__false_type()` tag
//   temporaries passed by const reference, `_M_clear()` = trivial _Destroy + deallocate, `_M_set(start, finish,
//   start + len)` stored end-of-storage first. Element = Ego_r.pdb CSubtitleRenderer::CEmbeddedGraphic (Graphic +0,
//   CharIndex +4, C2DVector Offset +8; 16 bytes); STLport vector {_M_start, _M_finish, _M_end_of_storage} (12 bytes;
//   the PDB's 16-byte Dinkumware vector is the debug build's).
// Label correction: catalogued as `Std_Vector_Insert_Range_CBodyPartMesh`; the element is CEmbeddedGraphic (the
//   landed push_back's only caller is ProcessEmbeddedGraphicTags) and the body is STLport `_M_insert_overflow`.
// Fix (agent901): the landed byte-identical twin 0x00580A7B (vector<pair<const CRegion*, C3DVector>>) shape -- same-TU
//   auto_inline(off) bodies for __uninitialized_copy / __uninitialized_fill_n (tag unnamed, never touched) and
//   `#pragma optimize("y", off)`: the three tag temporaries then pack into the bool __atend slot padding as retail.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
#include <new>

extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

class C2DVector { public:
    float X;
    float Y;
};

class CSubtitleRenderer {
public:
    class CEmbeddedGraphic { public:                       // Ego_r.pdb, 16 bytes
        long      Graphic;                          // +0x00
        long      CharIndex;                        // +0x04
        C2DVector Offset;                           // +0x08
        CEmbeddedGraphic(const CEmbeddedGraphic& other);   // 0x004935F8
    };
};

namespace std {

struct __false_type { __false_type() { } };

typedef CSubtitleRenderer::CEmbeddedGraphic CEmbeddedGraphic;

#pragma auto_inline(off)
CEmbeddedGraphic* __fastcall __uninitialized_copy(const CEmbeddedGraphic* __first, const CEmbeddedGraphic* __last,
                                                CEmbeddedGraphic* __result, const __false_type&)        // 0x00493959 (same-TU stand-in body)
{
    CEmbeddedGraphic* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) CEmbeddedGraphic(*__first);
    return __cur;
}
CEmbeddedGraphic* __fastcall __uninitialized_fill_n(CEmbeddedGraphic* __first, size_t __n, const CEmbeddedGraphic& __x,
                                                  const __false_type&)                                // 0x00493984 (same-TU stand-in body)
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CEmbeddedGraphic(__x);
    return __first;
}
#pragma auto_inline(on)

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct allocator {
    __forceinline _Tp* allocate(size_t __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    __forceinline void deallocate(_Tp* __p, size_t) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp*                                                _M_start;             // +0x00
    _Tp*                                                _M_finish;            // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> >       _M_end_of_storage;    // +0x08
    size_t size() const { return size_t(_M_finish - _M_start); }
    __forceinline void _M_clear() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                            bool __atend = false);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                                     bool __atend)
{
    const size_t __old_size = size();
    const size_t __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = _M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CEmbeddedGraphic>::_M_insert_overflow(CEmbeddedGraphic*, const CEmbeddedGraphic&, const __false_type&,
                                                         size_t, bool);

}