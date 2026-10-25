// Behaviour test for 0x00BEDD70 std::__adjust_heap over the (long, CCompositeSurfaceInfo) vector-map entries: the
// sift-down moves the larger child up, the trailing single child is moved, the value is pushed back up, and every
// counted-pointer reference is balanced (the by-value value is released, the base destructor runs).
#include <new>

// CBaseClass: the polymorphic engine root (out-of-line virtual destructor 0x0099A2E0, vtable 0x01231710).
class CBaseClass {
public:
    virtual ~CBaseClass();                          // 0x0099A2E0
};

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

class CCompositeSurfaceCache { public: int ID; };
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
void __fastcall __push_heap(RandomAccessIter first, Distance holeIndex, Distance topIndex, T value, Compare comp)
{
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

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

#include <cstdio>
static int liveControls, livePayloads, destroyCalls, baseDestructs;
static CCPPointerInfo* controls[20];
static int controlCount;
CBaseClass::~CBaseClass() { ++baseDestructs; }
void __fastcall destroyCache(void* p) { CCompositeSurfaceCache* c = (CCompositeSurfaceCache*)p; if (c) { delete c; --livePayloads; ++destroyCalls; } }
static void init(CSurfaceEntry& e, int key, int id)
{
    e.first = key; CCPPointerInfo* b = new CCPPointerInfo; b->RefCount = 1; b->DeleteFunction = destroyCache;
    b->Data = new CCompositeSurfaceCache; ((CCompositeSurfaceCache*)b->Data)->ID = id;
    e.second.Cache.Data = (CCompositeSurfaceCache*)b->Data; e.second.Cache.Info = b; controls[controlCount++] = b; ++liveControls; ++livePayloads;
}
static int id(const CSurfaceEntry& e) { return e.second.Cache.Data ? e.second.Cache.Data->ID : 0; }
static bool owns(const CSurfaceEntry* p, int n, const CSurfaceEntry& x, const CSurfaceEntry& y, const CSurfaceEntry& z)
{
    for (int i = 0; i < controlCount; ++i) {
        CCPPointerInfo* b = controls[i]; unsigned int expected = 0;
        for (int j = 0; j < n; ++j) if (p[j].second.Cache.Info == b) ++expected;
        if (x.second.Cache.Info == b) ++expected; if (y.second.Cache.Info == b) ++expected; if (z.second.Cache.Info == b) ++expected;
        if (expected && b->RefCount != expected) return false;
    }
    return true;
}
static int run()
{
    if (sizeof(CCompositeSurfaceInfo) != 12 || sizeof(CSurfaceEntry) != 16) return 1;
    {
        CSurfaceEntry a[7]; for (int i = 0; i < 7; ++i) init(a[i], 7 - i, 100 + i);
        CSurfaceEntryLess c;
        CSurfaceEntry x = a[0]; x.first = 1; CSurfaceEntry y = a[0]; CSurfaceEntry z = a[0];
        std::__adjust_heap(a, 0, 7, x, c);
        if (a[0].first != 6 || id(a[0]) != 101 || a[1].first != 4 || id(a[1]) != 103 || a[3].first != 1 || id(a[3]) != 100 || !owns(a, 7, x, y, z)) return 2;
        y = a[0]; y.first = 10; std::__push_heap(a, 6, 0, y, c);
        if (a[0].first != 10 || id(a[0]) != 101 || !owns(a, 7, x, y, z)) return 3;
        z = a[0]; z.first = 20; std::__adjust_heap(a, 0, 1, z, c);
        if (a[0].first != 20 || id(a[0]) != 101 || !owns(a, 7, x, y, z)) return 4;
        CCPPointerInfo* shared = a[0].second.Cache.Info; unsigned int before = shared->RefCount; a[0] = a[0]; if (shared->RefCount != before) return 5;
        CSurfaceEntry alias = a[0]; before = shared->RefCount; alias = a[0]; if (shared->RefCount != before) return 6;
    }
    if (livePayloads || destroyCalls != 7 || baseDestructs < 10) return 7;
    {
        CSurfaceEntry one[2]; init(one[0], 1, 201); init(one[1], 9, 202); CSurfaceEntryLess c;
        CSurfaceEntry candidate = one[0]; candidate.first = 4; std::__adjust_heap(one, 0, 2, candidate, c);
        if (one[0].first != 9 || id(one[0]) != 202 || one[1].first != 4 || id(one[1]) != 201) return 8;
    }
    if (livePayloads || destroyCalls != 9) return 9;
    return 0;
}
int main()
{
    int r = run();
    if (r) { std::printf("00bedd70_TEST FAIL %d\n", r); return 1; }
    std::printf("00bedd70_TEST PASS\n");
    return 0;
}
