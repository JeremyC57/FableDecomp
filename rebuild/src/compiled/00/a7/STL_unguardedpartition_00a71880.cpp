// std::__unguarded_partition(pair<long,C3DMeshVolume>* first, pair<long,C3DMeshVolume>* last, pair<long,C3DMeshVolume> pivot, CKeyPairCompareLess<long,C3DMeshVolume> comp) -- quicksort partition of the navigation containment volumes around a by-value pivot (signed key); returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71880, 127 bytes, /Gr `__fastcall` (first in ecx, last in edx; the
//   16-byte pivot and the empty comparator on the stack, `ret 14h`).
// Callees: std::swap<pair<long,C3DMeshVolume> > 0x00A70320 (landed, out of line), the count block's DeleteFunc,
//   operator delete 0x00BFE9BC, CCharString::~CCharString 0x0099EAE0 (the by-value pivot).
// Identity: the manifest's "QuickPartition_WithHeapPhase" is descriptive. The element is the 16-byte pair<long,
//   C3DMeshVolume> of CPhysicsMesh::NavigationContainmentVolumes, ordered by the signed key (`jge`/`jl`).
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- the pivot key reloaded into eax at the
//   top of each pass, iter_swap reaching the out-of-line swap(*a, *b); the callee destroys its by-value pivot
//   (counted-pointer release with the block re-read, zero stores, the string destructor).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CCPPointerInfo {                                  // Ego_r.pdb, 12 bytes
public:
    long  RefCount;                                     // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04, Data in ecx
    void* Data;                                         // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            operator delete(Info);
        }
        Data = 0;
        Info = 0;
    }
};

class CContainmentVolume;

class C3DMeshVolume {                                   // 12 bytes (Ego_r.pdb)
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {

template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b);              // 0x00A70320

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
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

typedef pair<long, C3DMeshVolume> TNavEntry;
template TNavEntry* __fastcall __unguarded_partition<TNavEntry*, TNavEntry, CKeyPairCompareLess<long, C3DMeshVolume> >(TNavEntry*, TNavEntry*, TNavEntry, CKeyPairCompareLess<long, C3DMeshVolume>);

}