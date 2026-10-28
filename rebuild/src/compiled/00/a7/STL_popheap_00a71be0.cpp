// std::__pop_heap(pair<CCharString,C3DMeshVolume>* first, pair<CCharString,C3DMeshVolume>* last, pair<CCharString,C3DMeshVolume>* result, pair<CCharString,C3DMeshVolume> val, CKeyPairCompareLess<CCharString,C3DMeshVolume> comp, int*) -- heap-sort pop over the named containment volumes: move the root to result, then sift val down from the root of the shortened heap
// Leaf: ??$__pop_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71BE0, 178 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx; result, the 16-byte value, the empty comparator and the distance-type tag pointer on the stack,
//   `ret 1Ch`).
// Callees: CCharString::operator= 0x0099EFB0 and C3DMeshVolume::operator= 0x00A32960 (the `*result = *first` pair
//   assignment), CCharString::CCharString(const CCharString&) 0x0099EC30 (x2, the by-value copy for __adjust_heap),
//   std::__adjust_heap 0x00A71370 (__fastcall first/hole, then len, the 16-byte value, the comparator), the count
//   block's DeleteFunc, operator delete 0x00BFE9BC, CCharString::~CCharString 0x0099EAE0 (x2).
// Identity: the manifest's "std_Adjust_Heap_Pair" is descriptive. The element is the 16-byte
//   pair<CCharString, C3DMeshVolume> of CPhysicsMesh::ContainmentVolumes ordered by the CCharString key.
// Shape: STLport `*result = *first; __adjust_heap(first, _Distance(0), _Distance(last - first), val, comp);` -- the
//   by-value val is copied into the callee's argument area (pair copy constructor: two string copies, the counted
//   pointer's AddRef), then val itself dies here (counted-pointer release, zero stores, the two string destructors).
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

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A71370

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

typedef pair<CCharString, C3DMeshVolume> TVolumeEntry;
template void __fastcall __pop_heap<TVolumeEntry*, TVolumeEntry, CKeyPairCompareLess<CCharString, C3DMeshVolume>, int>(TVolumeEntry*, TVolumeEntry*, TVolumeEntry*, TVolumeEntry, CKeyPairCompareLess<CCharString, C3DMeshVolume>, int*);

}