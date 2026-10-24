// _STL::__unguarded_linear_insert(CSurfaceEntry* last, CSurfaceEntry val, CSurfaceEntryLess comp) -- shift larger-keyed (key, CCompositeSurfaceInfo) entries up one slot until val's place is found (no lower bound check), store val there
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@JVCCompositeSurfaceInfo@@@_STL@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BED790, 225 bytes, `ret 14h` (the 16-byte value + the empty comparator).
// Callees: operator delete @ 0x00BFE9BC (count blocks), ~CBaseClass @ 0x0099A2E0 (the by-value value's base).
// Label: the manifest's "CNavigationPosition_CopyRange" is propagated; this is the STLport sort instantiation of
// CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> > (Ego_r.pdb name set).
// Element: _STL::pair<long, CCompositeSurfaceInfo> (16 bytes): key, then CCompositeSurfaceInfo {vptr 0x012A8154,
// CCountedPointer<CCompositeSurfaceCache> Cache}; the implicit operator= is the key store plus the counted-pointer
// assignment (vptr untouched), the by-value value's implicit destructor releases Cache and calls ~CBaseClass.
// Shape: STLport 4.6 __unguarded_linear_insert; signed key compare inline (`jge`/`jl`).
#include <new>
#include <stddef.h>

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
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { DeleteData(); Data = 0; Info = 0; }
    void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunction(Info->Data);
                operator delete(Info);
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            DeleteData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CCompositeSurfaceCache;
// Ego_r.pdb CCompositeSurfaceInfo (vtable 0x012A8154, 12 bytes): implicit copy, assignment and (virtual) destructor.
class CCompositeSurfaceInfo : public CBaseClass {
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;  // +0x04
};

namespace _STL {
struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};
}   // namespace _STL

typedef _STL::pair<long, CCompositeSurfaceInfo> CSurfaceEntry;   // 0x10 bytes

// The vector map's key order (an empty functor passed by value).
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<long, CCompositeSurfaceInfo> CSurfaceEntryLess;

namespace _STL {
template <class RandomAccessIter, class T, class Compare>
void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
}   // namespace _STL

template void _STL::__unguarded_linear_insert(CSurfaceEntry*, CSurfaceEntry, CSurfaceEntryLess);