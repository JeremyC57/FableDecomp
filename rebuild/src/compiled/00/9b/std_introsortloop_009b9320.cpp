// std::__introsort_loop(pair* first, pair* last, pair*, int depth_limit, CKeyPairCompareLess comp) -- (pair<unsigned long, CDefinitionManager::CDefClassInfo>) STLport introsort core of the definition manager's class-info vector map: while more than 16 entries remain, heapsort when the depth budget is spent, else partition around the median-of-three key (by-value pivot copy) and recurse on the right part
// Flags: /Gr
// Leaf: ??$__introsort_loop@PAU?$pair@KVCDefClassInfo@CDefinitionManager@@@std@@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B9320, 358 bytes, /Gr `__fastcall` (first ecx, last edx; the _Tp* tag,
//   depth_limit and the empty comparator on the stack), `ret 0Ch`.
// Identity (proven): called by the landed sort 0x009B9490 (call 0x009B94CE, the CVectorMap Sort of 0x009B94F0) and by
//   itself (0x009B9440); its helpers are the landed pair<unsigned long, CDefinitionManager::CDefClassInfo>
//   __unguarded_partition 0x009B8630 (whose notes name this caller) and __partial_sort 0x009B9030. The 24-byte element
//   (`imul 2AAAAAABh ; sar 2`) is that pair (Ego_r.pdb CDefClassInfo {CCharString Name; AllocFunc; CArray<unsigned long>
//   GlobalIndices}, retail 20 bytes), compared on the unsigned key (`jae`/`jb`) by CKeyPairCompareLess. The catalog's
//   "introsort_partial_24byte_quicksort" is descriptive; reloc_bindings holds only the stand-in ?IntrosortLoop_009b9320@@.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, malloc 0x00BFEA0E and memmove 0x00BFEAE6 (the pivot's
//   inline pair copy: key, Name, AllocFunc, the GlobalIndices vector copy), __unguarded_partition 0x009B8630, itself,
//   __partial_sort 0x009B9030 (behind the inline partial_sort).
// Model: this chunk's 0x0067F280 (STLport 4.6 _algo.c __introsort_loop verbatim) with the landed 0x009B9030 element model;
//   the pivot `_Tp(__median(..))` is built straight in the argument area through pair's user copy constructor.
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

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                        _Compare __comp);                                      // 0x009B8630

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                    _Compare __comp);                                                          // 0x009B9030

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

#define __stl_threshold 16

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __introsort_loop(CDefClassInfoPair*, CDefClassInfoPair*, CDefClassInfoPair*, int,
                               CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo>);

}