// std::__unguarded_partition(pair<CCharString,C3DMeshVolume>* first, pair<CCharString,C3DMeshVolume>* last, pair<CCharString,C3DMeshVolume> pivot, CKeyPairCompareLess<CCharString,C3DMeshVolume> comp) -- quicksort partition of the named containment volumes around a by-value pivot (name order); returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71910, 110 bytes, /Gr `__fastcall` (first in ecx, last in edx; the
//   16-byte pivot and the empty comparator on the stack, `ret 14h`).
// Callees: CBasicString<char>::operator< 0x00429950 (behind the inline CCharString::operator<), std::swap<pair<
//   CCharString,C3DMeshVolume> > 0x00A70490 (landed, out of line), pair<CCharString,C3DMeshVolume>::~pair 0x00A6F390
//   (landed; the by-value pivot's destructor, out of line here).
// Identity: the manifest's "Introsort_QuickPartition_Str" is descriptive. The element is the 16-byte
//   pair<CCharString, C3DMeshVolume> of CPhysicsMesh::ContainmentVolumes ordered by the CCharString key.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- the pivot's string data reloaded into
//   esi at the top of each pass, comp the inline CCharString::operator< (equal -> false, null left -> true, null right
//   -> false, else the data compare), iter_swap reaching the out-of-line swap(*a, *b). The by-value pivot dies through
//   the pair destructor's out-of-line COMDAT (VC7.1 inlines the implicit ~pair here, 182 v 110, so the model declares
//   it, as the same-TU swap/linear-insert callees are declared).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

// Same-TU body of CBasicString<char>::operator< 0x00429950 (landed) under auto_inline(off), so VC7.1 keeps the
//   pivot's data pointer in esi across the calls (EXTRA_RULES / GOTCHAS: STLport-over-CCharString algorithms).
#pragma auto_inline(off)
template <class T> class CBasicString {
public:
    T* m_pData;
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicString& other) const { return Compare(other) < 0; }
};
#pragma auto_inline(on)

class CCharString {
public:
    CBasicString<char>* PData;                          // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
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
    ~pair();                                            // 0x00A6F390: the implicit destructor's out-of-line COMDAT
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {

template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b);              // 0x00A70490

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

typedef pair<CCharString, C3DMeshVolume> TVolumeEntry;
template TVolumeEntry* __fastcall __unguarded_partition<TVolumeEntry*, TVolumeEntry, CKeyPairCompareLess<CCharString, C3DMeshVolume> >(TVolumeEntry*, TVolumeEntry*, TVolumeEntry, CKeyPairCompareLess<CCharString, C3DMeshVolume>);

}