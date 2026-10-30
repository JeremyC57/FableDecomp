// std::__adjust_heap(CSurfaceEntry* first, int holeIndex, int len, CSurfaceEntry value, CSurfaceEntryLess comp) -- STLport heap sift-down over the (long, CCompositeSurfaceInfo) vector-map entries: move the larger child up until the hole reaches a leaf, then push the value back up from there
// Flags: /O2 /Oy /W3 /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@HU12@V?$CKeyPairCompareLess@JVCCompositeSurfaceInfo@@@@@std@@YIXPAU?$pair@JVCCompositeSurfaceInfo@@@0@HHU10@V?$CKeyPairCompareLess@JVCCompositeSurfaceInfo@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEDD70, 300 bytes, `__fastcall` (first ecx, holeIndex edx), `ret 18h`
//   (len, the 16-byte value, the empty comparator).
// Identity: renamed (sweep1, 2026-10-25) from N00bedd70::__adjust_heap<Element,CKeyPairCompareLess> (its file was named
//   for the propagated catalog label _Adjust_heap<pair<long,CCountedPointer<CNavConnectorNode> >*,...>). Callers: the
//   landed std::__make_heap 0x00BEE140 (declares this callee as std::__adjust_heap<CSurfaceEntry*, int, CSurfaceEntry,
//   CSurfaceEntryLess>) and the __pop_heap<pair<long,CCompositeSurfaceInfo> > 0x00BEE070 (labels_chunk190_195), both in
//   the STLport sort of CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<...> > (Ego_r.pdb name set; FINDINGS
//   chunk 432; agent432 NOTES).
// Element: std::pair<long, CCompositeSurfaceInfo> (16 bytes): the key, then Ego_r.pdb CCompositeSurfaceInfo (RTTI vtable
//   0x012A8154, base CBaseClass vtable 0x01231710) {CCountedPointer<CCompositeSurfaceCache> Cache}.
// Callees: the entry's implicit operator= 0x00BED640 (out of line for the trailing `secondChild == len` move only),
//   __push_heap 0x00BED880, operator delete 0x00BFE9BC (count blocks), ~CBaseClass 0x0099A2E0 (the by-value value's base).
// Shape: STLport 4.6 __adjust_heap verbatim; the by-value value is copy-constructed in its outgoing slot (key, vptr
//   0x012A8154, counted-pointer copy) and destroyed after the call. The counted pointer's members are __forceinline (VC
//   otherwise keeps them out of line in this TU).
#include <new>

// CBaseClass: the polymorphic engine root (out-of-line virtual destructor 0x0099A2E0, vtable 0x01231710).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCPPointerInfo {                              // the shared count block of a CCountedPointer
public:
    unsigned int RefCount;                          // +0x00
    void (__fastcall* DeleteFunction)(void* data);  // +0x04
    void* Data;                                     // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                           // +0x00
    CCPPointerInfo* Info;                           // +0x04
    __forceinline CCountedPointer() : Data(0), Info(0) { }
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer() { DeleteData(); Data = 0; Info = 0; }
    __forceinline CCPPointerInfo* GetInfo() const { return Info; }
    __forceinline void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunction(Info->Data);
                ::operator delete(GetInfo());
            }
        }
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
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

namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};
}   // namespace std

typedef std::pair<long, CCompositeSurfaceInfo> CSurfaceEntry;   // 0x10 bytes

// The vector map's key order (an empty functor passed by value).
template <class K, class V> class CKeyPairCompareLess {
public:
    __forceinline bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<long, CCompositeSurfaceInfo> CSurfaceEntryLess;

namespace std {
template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __push_heap(RandomAccessIter first, Distance holeIndex, Distance topIndex, T value, Compare comp);   // 0x00BED880

template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value, Compare comp)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value, comp);
}
}   // namespace std

template void __fastcall std::__adjust_heap(CSurfaceEntry*, int, int, CSurfaceEntry, CSurfaceEntryLess);
