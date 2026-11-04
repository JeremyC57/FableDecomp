// std::__unguarded_insertion_sort_aux(pair<unsigned long,CDefClassInfo>* first, pair<unsigned long,CDefClassInfo>* last, pair<unsigned long,CDefClassInfo>*, CKeyPairCompareLess<unsigned long,CDefClassInfo> comp) -- STLport unguarded insertion sort: sink a by-value copy of every element of [first, last) with __unguarded_linear_insert
// Flags: /Gr
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@KVCDefClassInfo@CDefinitionManager@@@std@@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B80C0, 171 bytes (+ int3 padding), `ret 8` (/Gr TU: first in ecx, last
//   in edx; the unused T* and the empty comparator on the stack).
// Identity: the 24-byte pair<unsigned long, CDefClassInfo> of 0x009B7320; callers 0x009B86A7 / 0x009B9226 (the final
//   insertion sorts of that vector, e.g. the landed __final_insertion_sort 0x009B91F0).  Catalogued as
//   `MakeHeap_Pair16byte_Build`.
// Callees: CCharString(const CCharString&) @ 0x0099EC30, malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6 (the inlined
//   pair / CDefClassInfo / vector copy constructors building the value in the argument area),
//   __unguarded_linear_insert(i, value, comp) @ 0x009B76F0.
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`.
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                           // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
private:
    void* PStringData;
};

#include "engine/CArray.h"


class CDefClassBase;
class CDefinitionManager {
public:
    class CDefClassInfo {                               // Ego_r.pdb (24 bytes; retail 20: CArray 16 -> 12)
    public:
        CCharString            Name;                    // +0x00
        CDefClassBase* (__cdecl* AllocFunc)();          // +0x04
        CArray<unsigned long>  GlobalIndices;           // +0x08
    };
};
typedef std::pair<unsigned long, CDefinitionManager::CDefClassInfo> CDefClassInfoPair;

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x009B76F0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __unguarded_insertion_sort_aux(CDefClassInfoPair*, CDefClassInfoPair*,
    CDefClassInfoPair*, CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo>);
}