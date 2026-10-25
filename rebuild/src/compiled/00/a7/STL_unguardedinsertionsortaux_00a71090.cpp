// std::__unguarded_insertion_sort_aux(pair<long,C3DMeshVolume>* first, pair<long,C3DMeshVolume>* last, pair<long,C3DMeshVolume>*, CKeyPairCompareLess<long,C3DMeshVolume> comp) -- insertion-sort pass with no lower-bound test over the navigation containment volumes, each entry handed by value to __unguarded_linear_insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71090, 79 bytes, /Gr `__fastcall` (first in ecx, last in edx; the
//   value-type tag pointer and the empty comparator on the stack, `ret 8`).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, std::__unguarded_linear_insert 0x00A703C0 (same
//   chunk; it owns and destroys val).
// Label correction: catalogued as `_Copy_impl<CComplexNumber*,CComplexNumber*>` (propagated). The element is the
//   16-byte pair<long, C3DMeshVolume> of CPhysicsMesh::NavigationContainmentVolumes; byte twin (signed key) of the
//   landed pair<unsigned long, C3DMeshVolume> 0x00A33870, whose model this follows.
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);` -- comp pushed first,
//   then T(*i) built in the argument area by STLport's pair copy constructor (key, the string copy call, the counted
//   pointer's two words and add-ref).
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

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A703C0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

typedef pair<long, C3DMeshVolume> TNavEntry;
template void __fastcall __unguarded_insertion_sort_aux<TNavEntry*, TNavEntry, CKeyPairCompareLess<long, C3DMeshVolume> >(TNavEntry*, TNavEntry*, TNavEntry*, CKeyPairCompareLess<long, C3DMeshVolume>);

}