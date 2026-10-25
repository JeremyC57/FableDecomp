// std::__introsort_loop(pair<CSkeletalMorphs::CEntry,float>* first, pair<CSkeletalMorphs::CEntry,float>* last, pair<CSkeletalMorphs::CEntry,float>*, int depth_limit, CIsSkeletalMorphLowerPriorityThan comp) -- STLport introsort recursion over the weighted skeletal morphs by priority
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00721C10, 251 bytes, __fastcall (first ecx, last edx), `ret 0Ch`.
// Label correction: catalog "TextureMorph_QuickSort" -> STLport 4.x __introsort_loop over the CTCHeroMorph TU's
//   vector<pair<CSkeletalMorphs::CEntry, float> > (the landed __unguarded_partition 0x0071FC20 and __partial_sort
//   0x00721400 name this caller; 12-byte element).
// Callees: __unguarded_partition 0x0071FC20 (pivot by value), __partial_sort 0x00721400, itself.
// Shape: as the 0x006FD1E0 loop: `while (last - first > 16)`, the depth-limit partial_sort fallback, `--depth_limit` in
//   its own slot, __median inlined over the signed `short` Priority keys (`jge`/`jl`), the pivot copied into the
//   by-value argument area through pair's memberwise copy constructor (three dword moves via eax), `last = cut`.
class CDefString {
public:
    long TablePos;                              // +0x00
};

class CSkeletalMorphs {
public:
    class CEntry {                              // Ego_r.pdb, 8 bytes
    public:
        CDefString MorphName;                   // +0x00
        short Priority;                         // +0x04
        bool SupressesLowerMorphs;              // +0x06
    };
};

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

typedef std::pair<CSkeletalMorphs::CEntry, float> SMorphWeight;

class CIsSkeletalMorphLowerPriorityThan {
public:
    bool operator()(const SMorphWeight& a, const SMorphWeight& b) const { return a.first.Priority < b.first.Priority; }
};

namespace std {

const int __stl_threshold = 16;

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
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x0071FC20

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x00721400

template <class _RandomAccessIterator, class _Compare>
inline void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (SMorphWeight*)0, __comp);
}

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

template void __fastcall __introsort_loop<SMorphWeight*, SMorphWeight, int, CIsSkeletalMorphLowerPriorityThan>(SMorphWeight*, SMorphWeight*, SMorphWeight*, int, CIsSkeletalMorphLowerPriorityThan);

}