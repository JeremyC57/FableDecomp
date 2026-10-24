// _STL::vector<CSubtitleRenderer::CEmbeddedGraphic>::_M_insert_overflow(CEmbeddedGraphic*, const CEmbeddedGraphic&, const __false_type&, unsigned int, bool) -- STLport reallocating insert of 16-byte embedded-graphic records: allocate old_size + max(old_size, n), copy the head, construct the n new records, copy the tail unless appending, free the old block
// Grade: DIFFER(181v180) -- the three `__false_type()` tag temporaries get their own frame word ([ebp-1], `sub esp` 8) where retail packs them into the padding of the bool `__atend` argument slot ([ebp+0x1b], one `push ecx` word); everything else matches
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493FF8, 180 bytes, `ret 14h` (__thiscall; size-optimised TU with an ebp
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
#pragma optimize("s", on)
#include <stdlib.h>                                 // malloc @ 0x00BFEA0E, free @ 0x00BFEA14
#include <new>

struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary

struct C2DVector {
    float X;
    float Y;
};

struct CEmbeddedGraphic {
    long      Graphic;                                  // +0x00
    long      CharIndex;                                // +0x04
    C2DVector Offset;                                   // +0x08
    CEmbeddedGraphic(const CEmbeddedGraphic& other);    // 0x004935F8
};

CEmbeddedGraphic* __fastcall __uninitialized_copy(CEmbeddedGraphic* first, CEmbeddedGraphic* last, CEmbeddedGraphic* result, const __false_type&);       // 0x00493959
CEmbeddedGraphic* __fastcall __uninitialized_fill_n(CEmbeddedGraphic* first, unsigned int n, const CEmbeddedGraphic& x, const __false_type&);             // 0x00493984

template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp>
struct allocator {
    allocator() { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    __forceinline void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

struct CEmbeddedGraphicVector {
    CEmbeddedGraphic* _M_start;                         // +0x00
    CEmbeddedGraphic* _M_finish;                        // +0x04
    _STLP_alloc_proxy<CEmbeddedGraphic*, CEmbeddedGraphic, allocator<CEmbeddedGraphic> > _M_end_of_storage;   // +0x08

    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    void _M_clear()
    {
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    void _M_set(CEmbeddedGraphic* __s, CEmbeddedGraphic* __f, CEmbeddedGraphic* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(CEmbeddedGraphic* __position, const CEmbeddedGraphic& __x, const __false_type&,
                            unsigned int __fill_len, bool __atend);
};

void CEmbeddedGraphicVector::_M_insert_overflow(CEmbeddedGraphic* __position, const CEmbeddedGraphic& __x, const __false_type&,
                                                unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    CEmbeddedGraphic* __new_start = _M_end_of_storage.allocate(__len);
    CEmbeddedGraphic* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        new (__new_finish) CEmbeddedGraphic(__x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
