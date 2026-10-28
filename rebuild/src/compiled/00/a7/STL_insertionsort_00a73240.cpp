// std::__insertion_sort(pair<long,C3DMeshVolume>* first, pair<long,C3DMeshVolume>* last, CKeyPairCompareLess<long,C3DMeshVolume> comp) -- insertion-sort the navigation containment volumes by signed key, inserting each entry after the first through __linear_insert
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A73240, 109 bytes (+3 int3), /Gr `__fastcall` (first in ecx, last in
//   edx; the empty comparator on the stack, `ret 4`).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the by-value element copy's volume name),
//   std::__linear_insert 0x00A728F0 (same chunk).
// Identity: the manifest's "Vector_ReserveLog2_ComplexNumber_Impl" is descriptive. 16-byte pair<long, C3DMeshVolume>
//   entries of CPhysicsMesh::NavigationContainmentVolumes (CKeyPairCompareLess<long, C3DMeshVolume>).
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i,
//   comp);` -- first and last live in the two frame words; each by-value element is copied into the argument area
//   (key word, the name's copy constructor, the counted pointer's two words and AddRef).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (Info)
                ++Info->RefCount;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
};

class CContainmentVolume;

class C3DMeshVolume {                                   // 12 bytes (Ego_r.pdb)
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
    C3DMeshVolume& operator=(const C3DMeshVolume& other)   // out of line @ 0x00A32960 where not inlined
    {
        Name = other.Name;
        ConvexVolume = other.ConvexVolume;
        return *this;
    }
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

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A728F0

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

typedef pair<long, C3DMeshVolume> TNavEntry;
template void __fastcall __insertion_sort<TNavEntry*, CKeyPairCompareLess<long, C3DMeshVolume> >(TNavEntry*, TNavEntry*, CKeyPairCompareLess<long, C3DMeshVolume>);

}