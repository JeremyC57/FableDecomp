// _STL::__make_heap(CSurfaceEntry* first, CSurfaceEntry* last, CSurfaceEntryLess comp, CSurfaceEntry*, int*) -- heapify the (key, CCompositeSurfaceInfo) entries: sift every parent down from the last one to the root
// Leaf: ??$__make_heap@PAU?$pair@JVCCompositeSurfaceInfo@@@_STL@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEE140, 103 bytes, `ret 0Ch` (comparator, value-type and distance pointers).
// Callees: __adjust_heap @ 0x00BEDD70 (landed).
// Label: the manifest's "Std_Make_Heap_SiftDown" is descriptive; this is the STLport heap build used by partial_sort
// @ 0x00BEED30 (this chunk) for the CVectorMap<long, CCompositeSurfaceInfo> entries.
// Element: see 0x00BED790. Shape: STLport 4.6 __make_heap (`(len - 2) / 2` as cdq/sub/sar); the by-value element is
// copy-constructed in its outgoing slot (key, vptr 0x012A8154, counted-pointer copy).
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
template <class RandomAccessIter, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value, Compare comp);   // 0x00BEDD70

template <class RandomAccessIter, class Compare, class T, class Distance>
void __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*)
{
    if (last - first < 2) return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
}   // namespace _STL

template void _STL::__make_heap(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntryLess, CSurfaceEntry*, int*);