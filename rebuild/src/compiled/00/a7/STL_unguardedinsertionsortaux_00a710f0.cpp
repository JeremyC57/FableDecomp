// std::__unguarded_insertion_sort_aux(pair<CCharString, C3DMeshVolume>* first, pair<CCharString, C3DMeshVolume>* last, pair<CCharString, C3DMeshVolume>*, CKeyPairCompareLess<CCharString, C3DMeshVolume> comp) -- unguarded insertion-sort pass over a physics mesh's named containment volumes: insert a copy of every entry of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A710F0, 83 bytes, `ret 8` (link-time __fastcall /Gr algorithm TU, written as
//   explicit __fastcall: first in ecx, last in edx, the value-type pointer and the empty comparator's 4-byte slot on the
//   stack).
// Identity: the `__final_insertion_sort` tail of the sort over CPhysicsMesh::ContainmentVolumes (CVectorMap<CCharString,
//   C3DMeshVolume>): callers the landed sort 0x00A73BA0 and the sibling COMDATs 0x00A71910 / 0x00A73600; same
//   instantiation as the landed __unguarded_linear_insert 0x00A70540.
// Callees: CCharString copy constructor 0x0099EC30 (the key, then the volume's name), __unguarded_linear_insert
//   0x00A70540 (landed; it destroys the by-value entry).
// Layout: Ego_r.pdb C3DMeshVolume (12 bytes): Name CCharString +0, ConvexVolume CCountedPointer<CContainmentVolume> +4
//   (Data, then the CCPPointerInfo count block whose RefCount is bumped on copy).
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`: the 16-byte by-value
//   pair is built in its outgoing slot (`sub esp,10h ; mov edi,esp`) by STLport pair's memberwise copy constructor; the
//   volume's IMPLICIT copy constructor re-bases edi (`add edi,4`): name copy, Data word, Info word with the add-ref.
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the body is STLport
//   `__unguarded_insertion_sort_aux`.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const;
};

class CCPPointerInfo {                                  // Ego_r.pdb, 12 bytes
public:
    long  RefCount;                                     // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
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
    ~CCountedPointer();
};

class CContainmentVolume;
class C3DMeshVolume {                                   // 12 bytes (Ego_r.pdb), implicit copy constructor
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {  // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<CCharString, C3DMeshVolume> TVolumeEntry;                  // 16 bytes
typedef CKeyPairCompareLess<CCharString, C3DMeshVolume> TVolumeLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A70540

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<TVolumeEntry*, TVolumeEntry, TVolumeLess>(TVolumeEntry*, TVolumeEntry*, TVolumeEntry*, TVolumeLess);
}