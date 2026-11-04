// std::__make_heap(pair<unsigned long,CDefClassInfo>* first, pair<unsigned long,CDefClassInfo>* last, CKeyPairCompareLess<unsigned long,CDefClassInfo> comp, pair<unsigned long,CDefClassInfo>*, int*) -- STLport heapify of (index, class info) pairs: sift every parent from (len - 2) / 2 down to the root with __adjust_heap
// Flags: /Gr
// Leaf: ??$__make_heap@PAU?$pair@KVCDefClassInfo@CDefinitionManager@@@std@@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B8820, 218 bytes, `ret 0Ch` (/Gr TU: first in ecx, last in edx; the
//   empty comparator, the unused T* and distance* on the stack).
// Identity: the 24-byte pair<unsigned long, CDefClassInfo> of 0x009B7320 (len = bytes / 24 via 0x2AAAAAAB); callers
//   0x009B8A09 / 0x009B9046 (make_heap in its partial_sort / sort paths).  Catalogued as `MakeHeap_Pair24byte_Build`.
// Callees: CCharString(const CCharString&) @ 0x0099EC30, malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6 (the inlined pair /
//   CDefClassInfo / vector copy constructors building the value in the argument area), __adjust_heap @ 0x009B8220.
// Shape: STLport _heap.c __make_heap, as the CCharString-pair row 0x009B6D20 (len and first spilled to the frame).
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
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x009B8220

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

template void __make_heap(CDefClassInfoPair*, CDefClassInfoPair*,
                          CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo>, CDefClassInfoPair*, int*);
}