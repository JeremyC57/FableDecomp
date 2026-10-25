// std::vector<pair<EHeroMorphType,CParticleMorphs::CEntry>>::_M_allocate_and_copy(size_t n, const pair* first, const pair* last) -- STLport vector regrowth helper for CParticleMorphs' CVectorMap<EHeroMorphType, CParticleMorphs::CEntry> (40-byte pairs): malloc n elements (none for zero) and copy-construct [first, last) into them
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7279, 79 bytes, `ret 0Ch` (size-optimised TU: unaligned entry,
//   `pop ecx` after malloc, `push 9 ; pop ecx`). `this` (the vector) is never read: the allocator is empty.
// Caller: the vector's operator= at 0x004E7166 (catalogued as the propagated `CActiveFile::operator_assign`).
// Callee: CRT malloc 0x00BFEA0E.
// Label correction: the catalog's `AllocateAndCopyActiveFileStride40` is descriptive; the element is Ego_r.pdb's
//   pair<EHeroMorphType, CParticleMorphs::CEntry> (40 bytes; CEntry = MorphType, Blending, Location, LocationName,
//   LocationIndex, ParticleIndex, SecondParticleIndex, StartAtMorphStrength, EndAtMorphStrength).
// Shape: the 0x004E6088 model (STLport `_M_allocate_and_copy`, `allocate(n)` = `n != 0 ? malloc(n * 40) : 0`, the
//   `__false_type` copy loop, `__forceinline` helpers under `optimize("s")`); the element copy is STLport pair's
//   memberwise copy constructor: the key dword, then the 36-byte CEntry by its implicit copy (`rep movsd` x9).
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

namespace std {

struct __false_type { __false_type() { } };

enum EHeroMorphType { HERO_MORPH_TYPE_NONE };
enum EMeshLocationPoint { MESH_LOCATION_NONE };

struct CDefString {                             // 4 bytes, trivially copied
    unsigned long m_index;
};

struct CParticleMorphEntry {                    // CParticleMorphs::CEntry (Ego_r.pdb, 36 bytes)
    EHeroMorphType     MorphType;               // +0x00
    bool               Blending;                // +0x04
    EMeshLocationPoint Location;                // +0x08
    CDefString         LocationName;            // +0x0c
    long               LocationIndex;           // +0x10
    long               ParticleIndex;           // +0x14
    long               SecondParticleIndex;     // +0x18
    float              StartAtMorphStrength;    // +0x1c
    float              EndAtMorphStrength;      // +0x20
};

template <class T1, class T2> struct pair {     // STLport pair: memberwise copy constructor
    T1 first;
    T2 second;
    __forceinline pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<EHeroMorphType, CParticleMorphEntry> SParticleMorphPair;   // 40 bytes

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

template SParticleMorphPair* vector<SParticleMorphPair>::_M_allocate_and_copy(size_t, const SParticleMorphPair*, const SParticleMorphPair*);

}