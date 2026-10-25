// std::__unguarded_linear_insert(pair<CCharString,C3DMeshVolume>* last, pair<CCharString,C3DMeshVolume> val, CKeyPairCompareLess<CCharString,C3DMeshVolume> comp) -- insertion-sort inner step over the named containment volumes: shift every entry whose name sorts after val's up one slot, drop the value into the gap, destroy the by-value copy
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70540, 239 bytes (+1 int3), /Gr `__fastcall` (last in ecx; the
//   16-byte value and the empty comparator on the stack, `ret 14h`).
// Callees: CBasicString<char>::operator< 0x00429950 (behind the inline CCharString::operator<), CCharString::operator=
//   0x0099EFB0, C3DMeshVolume::operator= 0x00A32960 (the record assignment's out-of-line COMDAT), the count block's
//   DeleteFunc, operator delete 0x00BFE9BC, CCharString::~CCharString 0x0099EAE0 (x2).
// Identity: the manifest's "std_partition_PairWithOrdering" is descriptive. The element is the 16-byte
//   pair<CCharString, C3DMeshVolume> of CPhysicsMesh::ContainmentVolumes (CVectorMap<CCharString, C3DMeshVolume,
//   CKeyPairCompareLess<CCharString, C3DMeshVolume> >), ordered by the CCharString key (the landed LowerBound
//   0x00A6FC40's inline operator<); companion of the long-key 0x00A703C0.
// Shape: STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`
//   -- comp is `val.first < next->first` (equal data -> false, null left -> true, null right -> false, else the data
//   compare); the loop's pair assignment inlines the key's string operator= and the record assignment (string
//   operator= + the counted pointer's ShareData), the final `*last = val` calls the record's out-of-line COMDAT; then
//   val dies (counted-pointer release, zero stores, the two string destructors).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;    // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;                          // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

typedef pair<CCharString, C3DMeshVolume> TVolumeEntry;
template void __fastcall __unguarded_linear_insert<TVolumeEntry*, TVolumeEntry, CKeyPairCompareLess<CCharString, C3DMeshVolume> >(TVolumeEntry*, TVolumeEntry, CKeyPairCompareLess<CCharString, C3DMeshVolume>);

}