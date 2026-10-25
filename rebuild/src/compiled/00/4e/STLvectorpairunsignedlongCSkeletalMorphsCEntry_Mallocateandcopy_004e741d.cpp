// std::vector<pair<unsigned long,CSkeletalMorphs::CEntry>>::_M_allocate_and_copy(size_t n, const pair* first, const pair* last) -- STLport vector regrowth helper for CSkeletalMorphs' CVectorMap<unsigned long, CSkeletalMorphs::CEntry> (12-byte pairs): malloc n elements (none for zero) and copy-construct [first, last) into them
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E741D, 80 bytes, `ret 0Ch` (size-optimised TU: unaligned entry,
//   `pop ecx` after malloc). `this` (the vector) is never read: the allocator is empty.
// Caller: the vector's operator= at 0x004E7310 (catalogued as the propagated `CActiveFile::operator_assign`).
// Callee: CRT malloc 0x00BFEA0E.
// Label correction: the catalog's `AllocateAndCopyActiveFileStride12` is descriptive; the element is Ego_r.pdb's
//   pair<unsigned long, CSkeletalMorphs::CEntry> (12 bytes; CEntry = CDefString MorphName, short Priority,
//   bool SupressesLowerMorphs).
// Shape: the 0x004E6088 model (STLport `_M_allocate_and_copy`, `allocate(n)` = `n != 0 ? malloc(n * 12) : 0`, the
//   `__false_type` copy loop, `__forceinline` helpers under `optimize("s")`); the element copy is STLport pair's
//   memberwise copy constructor: the key dword, then the 8-byte CEntry by its implicit copy (two dwords through esi).
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

namespace std {

struct __false_type { __false_type() { } };

struct CDefString {                             // 4 bytes, trivially copied
    unsigned long m_index;
};

struct CSkeletalMorphEntry {                    // CSkeletalMorphs::CEntry (Ego_r.pdb, 8 bytes)
    CDefString MorphName;                       // +0x00
    short      Priority;                        // +0x04
    bool       SupressesLowerMorphs;            // +0x06
};

template <class T1, class T2> struct pair {     // STLport pair: memberwise copy constructor
    T1 first;
    T2 second;
    __forceinline pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<unsigned long, CSkeletalMorphEntry> SSkeletalMorphPair;   // 12 bytes

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

template SSkeletalMorphPair* vector<SSkeletalMorphPair>::_M_allocate_and_copy(size_t, const SSkeletalMorphPair*, const SSkeletalMorphPair*);

}