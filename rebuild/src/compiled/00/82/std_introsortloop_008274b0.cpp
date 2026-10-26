// std::__introsort_loop(pair* first, pair* last, pair*, int depth_limit, CKeyPairCompareLess comp) -- (pair<long, CCountedPointer<CContainmentVolume> >) quicksort with median-of-three pivots down to runs of 16, falling back to partial_sort at the depth limit
// Leaf: ??$__introsort_loop@PAU?$pair@JV?$CCountedPointer@VCContainmentVolume@@@@@std@@U12@HV?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@std@@YIXPAU?$pair@JV?$CCountedPointer@VCContainmentVolume@@@@@0@00HV?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008274B0, 250 bytes (+ int3 padding), `__fastcall` (first in ecx, last in edx; the null value-type pointer, the depth limit and the empty comparator on the stack, `ret 0Ch`).
// Identity (proven): std::__introsort_loop<pair<long, CCountedPointer<CContainmentVolume> >*, pair<...>, int, CKeyPairCompareLess<...> > (STLport 4.6 _algo.c; the PDB builds are Dinkumware); evidence: its call sites (0x008219D3, 0x00827569, 0x0082774D, 0x00A13307) are sort 0x00827710 (and CMap::InitCachedVolumes 0x00821850 directly) and its own recursion; that sort is the sort of CCachedVolumes::NavigationContainmentVolumes (CMap::InitCachedVolumes 0x00821850): CVectorMap<long, CCountedPointer<CContainmentVolume> > (J050 proves the element type from that sort's owner); its callees are that type's J052 __unguarded_partition 0x00826180 and J053 __partial_sort 0x00827130
// Label: the manifest's "Navigation_BalanceVolumeTree_Quickselect" is invented (family J055: five byte-identical bodies, one per element type).
// Callees: std::__unguarded_partition 0x00826180 (J052; a counted copy of the median as the by-value pivot), std::__partial_sort 0x00827130 (J053, via the inline partial_sort), itself (the right part).
// Body: `while (__last - __first > __stl_threshold) { if (__depth_limit == 0) { partial_sort(__first, __last, __last, __comp); return; } --__depth_limit;
//   _RandomAccessIter __cut = __unguarded_partition(__first, __last, _Tp(__median(*__first, *(__first + (__last - __first)/2), *(__last - 1), __comp)), __comp);
//   __introsort_loop(__cut, __last, (_Tp*) 0, __depth_limit, __comp); __last = __cut; }` with __stl_threshold 16 and STLport's inline __median.
// Layout: pair<K, CCountedPointer<V> > is 12 bytes {first +0, second.Data +4, second.Info +8}; CCPPointerInfo {RefCount +0, DeleteFunc +4, Data +8}.
// Model: J050 (agent950's landed CCountedPointer, key-only CKeyPairCompareLess).
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0
    void (__fastcall* DeleteFunc)(void* data);              // +4, Data in ecx
    void* Data;                                             // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0
    CCPPointerInfo* Info;                                   // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
};
class CContainmentVolume;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}   // namespace std
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const std::pair<K, V>& a, const K& key) const { return a.first < key; }
};
namespace std {
template <class _Iterator> struct iterator_traits { };
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
    typedef int difference_type;
};
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x00826180
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x00827130
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __comp);
}
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
enum { __stl_threshold = 16 };
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
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
typedef pair<long, CCountedPointer<CContainmentVolume> > _Entry;
template void __fastcall __introsort_loop<_Entry*, _Entry, int, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> > >(_Entry*, _Entry*, _Entry*, int, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> >);
}   // namespace std