// std::sort(CSurfaceEntry* first, CSurfaceEntry* last, CSurfaceEntryLess comp) -- STLport sort of the CVectorMap<long, CCompositeSurfaceInfo> entries: introsort loop to depth 2*lg(n), then the final insertion sort (inlined: threshold 16)
// Leaf: ??$sort@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@V?$CKeyPairCompareLess@JVCCompositeSurfaceInfo@@@@@std@@YIXPAU?$pair@JVCCompositeSurfaceInfo@@@0@0V?$CKeyPairCompareLess@JVCCompositeSurfaceInfo@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEF5C0, length 115 bytes (`__fastcall`: first in ecx, last in edx, the
//   empty comparator's 4-byte slot on the stack, `ret 4`).
// Identity: renamed (sweep1, 2026-10-25) from _global::Sort_SElement16_00bef5c0 (descriptive; manifest
//   std_vector_make_heap_Navigation propagated). Its callees are the landed STLport helpers of the same instantiation
//   (agent432): std::__introsort_loop<pair<long,CCompositeSurfaceInfo>*,...> 0x00BEF1D0, __insertion_sort 0x00BEECD0 and
//   __unguarded_insertion_sort_aux 0x00BEDD20; its callers are the CVectorMap<long, CCompositeSurfaceInfo,
//   CKeyPairCompareLess<...> > sort paths (0x00BECD95 in CEngineInternalPrimitiveMorphInfo::InitialiseCompositeSurfaces,
//   0x00BEF81E, 0x00BEF9B5) (FINDINGS chunk 432; agent432 NOTES).
// Element: std::pair<long, CCompositeSurfaceInfo> (16 bytes; Ego_r.pdb CCompositeSurfaceInfo, RTTI vtable 0x012A8154).
// Shape: STLport 4.6 `sort(first, last, comp) { if (first != last) { __introsort_loop(first, last, (T*)0, __lg(last -
//   first) * 2, comp); __final_insertion_sort(first, last, comp); } }` with __lg (the shift-count loop),
//   __final_insertion_sort and __unguarded_insertion_sort __forceinline (plain inline leaves them out of line): ranges longer than __stl_threshold (16) get
//   __insertion_sort(first, first + 16) then __unguarded_insertion_sort_aux(first + 16, last, (T*)0), shorter ones a single
//   __insertion_sort. `__fastcall` on the template definitions.
#include <new>

// CBaseClass: the polymorphic engine root (out-of-line virtual destructor 0x0099A2E0, vtable 0x01231710).
class CBaseClass {
public:
    virtual ~CBaseClass();                          // 0x0099A2E0
};

class CCPPointerInfo {                              // the shared count block of a CCountedPointer
public:
    long  RefCount;                                 // +0x00
    void (__fastcall* DeleteFunction)(void* data);  // +0x04
    void* Data;                                     // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                           // +0x00
    CCPPointerInfo* Info;                           // +0x04
};

class CCompositeSurfaceCache;
// Ego_r.pdb CCompositeSurfaceInfo (vtable 0x012A8154, 12 bytes): implicit copy, assignment and (virtual) destructor.
class CCompositeSurfaceInfo : public CBaseClass {
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;  // +0x04
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};
}   // namespace std

typedef std::pair<long, CCompositeSurfaceInfo> CSurfaceEntry;   // 0x10 bytes

// The vector map's key order (an empty functor passed by value).
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<long, CCompositeSurfaceInfo> CSurfaceEntryLess;

namespace std {
#define __stl_threshold 16

template <class RandomAccessIter, class T, class Size, class Compare>
void __fastcall __introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, Size depth_limit, Compare comp);   // 0x00BEF1D0
template <class RandomAccessIter, class Compare>
void __fastcall __insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp);   // 0x00BEECD0
template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp);   // 0x00BEDD20

template <class Size> __forceinline Size __lg(Size n)
{
    Size k;
    for (k = 0; n != 1; n >>= 1)
        ++k;
    return k;
}

template <class RandomAccessIter, class Compare>
__forceinline void __unguarded_insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __unguarded_insertion_sort_aux(first, last, (CSurfaceEntry*)0, comp);
}

template <class RandomAccessIter, class Compare>
__forceinline void __final_insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if (last - first > __stl_threshold) {
        __insertion_sort(first, first + __stl_threshold, comp);
        __unguarded_insertion_sort(first + __stl_threshold, last, comp);
    }
    else
        __insertion_sort(first, last, comp);
}

template <class RandomAccessIter, class Compare>
void __fastcall sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if (first != last) {
        __introsort_loop(first, last, (CSurfaceEntry*)0, __lg(last - first) * 2, comp);
        __final_insertion_sort(first, last, comp);
    }
}
}   // namespace std

template void __fastcall std::sort(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntryLess);
