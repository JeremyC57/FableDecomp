// std::__unguarded_partition(pair<unsigned long,CDefClassInfo>* first, pair<unsigned long,CDefClassInfo>* last, pair<unsigned long,CDefClassInfo> pivot, CKeyPairCompareLess<unsigned long,CDefClassInfo> comp) -- STLport quicksort partition of (index, class info) pairs around a by-value pivot key; returns the split point
// Flags: /Gr
// Leaf: ??$__unguarded_partition@PAU?$pair@KVCDefClassInfo@CDefinitionManager@@@std@@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B8630, 97 bytes, `ret 1Ch` (/Gr TU: first in ecx, last in edx; the
//   24-byte pivot and the empty comparator on the stack, the pivot destroyed by the callee).
// Identity: the 24-byte pair<unsigned long, CDefClassInfo> of 0x009B7320, unsigned-key compare; single caller 0x009B9429
//   (the introsort loop 0x009B9320 of the landed sort 0x009B9490).  The catalog label
//   `Quicksort_Partition_CEngineTextureDecal` is propagated.
// Callees: swap(pair&, pair&) @ 0x009B7650 (iter_swap inlined down to the out-of-line swap), free @ 0x00BFEA14 (the
//   pivot's vector storage), ~CCharString @ 0x0099EAE0.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }`.
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
template <class _Tp> void swap(_Tp& __a, _Tp& __b);   // 0x009B7650 (out of line)

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template CDefClassInfoPair* __unguarded_partition(CDefClassInfoPair*,
    CDefClassInfoPair*, CDefClassInfoPair, CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo>);
}