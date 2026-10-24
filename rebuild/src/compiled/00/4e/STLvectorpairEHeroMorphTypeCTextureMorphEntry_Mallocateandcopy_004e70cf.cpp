// _STL::vector<pair<EHeroMorphType,CTextureMorphEntry>>::_M_allocate_and_copy(size_t n, const pair* first, const pair* last) -- STLport vector regrowth helper for CTextureMorphs' CVectorMap<EHeroMorphType, CTextureMorphEntry> (24-byte pairs): malloc n elements (none for zero) and copy-construct [first, last) into them
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E70CF, 79 bytes, `ret 0Ch` (size-optimised TU: unaligned entry,
//   `pop ecx` after malloc, `push 5 ; pop ecx`). `this` (the vector) is never read: the allocator is empty.
// Caller: the vector's operator= at 0x004E6FBC (catalogued as the propagated `CActiveFile::operator_assign`), next to
//   CHeroMorphDef::Copy 0x004E6F06 (CHeroMorphDef holds CTextureMorphs at +0x28).
// Callee: CRT malloc 0x00BFEA0E.
// Label correction: the catalog's `AllocateAndCopyActiveFileStride24` is descriptive; the element is Ego_r.pdb's
//   pair<EHeroMorphType, CTextureMorphEntry> (24 bytes; CTextureMorphEntry = MorphType, TextureLayer,
//   ReplacingTextureIndex, BankIndex, Blend).
// Shape: the 0x004E6088 model (STLport `_M_allocate_and_copy` with `allocate(n)` = `n != 0 ? malloc(n * 24) : 0` and
//   the `__false_type` copy loop, allocate / _Construct / the loop `__forceinline` under `optimize("s")`); the element
//   copy is STLport pair's memberwise copy constructor: the key dword, then the 20-byte CTextureMorphEntry by its
//   implicit copy (`lea esi,[edx+4] ; push 5 ; pop ecx ; rep movsd`).
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

namespace _STL {

struct __false_type { __false_type() { } };

enum EHeroMorphType { HERO_MORPH_TYPE_NONE };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE };

struct CTextureMorphEntry {                     // Ego_r.pdb, 20 bytes
    EHeroMorphType      MorphType;              // +0x00
    long                TextureLayer;           // +0x04
    long                ReplacingTextureIndex;  // +0x08
    long                BankIndex;              // +0x0c
    ECompositeBlendType Blend;                  // +0x10
};

template <class T1, class T2> struct pair {     // STLport pair: memberwise copy constructor
    T1 first;
    T2 second;
    __forceinline pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<EHeroMorphType, CTextureMorphEntry> STextureMorphPair;   // 24 bytes

template <class T> __forceinline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T> struct allocator {           // STLport allocator<T>: empty
    allocator() { }
    __forceinline T* allocate(size_t n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};

template <class T> struct vector {              // STLport vector (12 bytes in retail)
    T*                   _M_start;              // +0
    T*                   _M_finish;             // +4
    _STLP_alloc_proxy<T> _M_end_of_storage;     // +8
    T* _M_allocate_and_copy(size_t n, const T* first, const T* last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
};

template STextureMorphPair* vector<STextureMorphPair>::_M_allocate_and_copy(size_t, const STextureMorphPair*, const STextureMorphPair*);

}