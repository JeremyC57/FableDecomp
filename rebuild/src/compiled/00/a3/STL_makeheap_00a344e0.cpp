// _STL::__make_heap(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, CKeyPairCompareLess comp, pair<unsigned long, C3DMeshVolume>*, int*) -- SGI make_heap of the mesh-volume table: sift every parent down from (len-2)/2 to the root with __adjust_heap, each fed a by-value copy of the parent
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A344E0, 116 bytes, `ret 0Ch` (__fastcall: first in ecx, last in edx, the
//   empty comparator's slot, the value-type pointer and the Distance* on the stack).
// Identity: called by __partial_sort 0x00A35260; the 16-byte twin of the landed 0x00A32C20; the catalog's
//   "Heap_MakeHeap_CFaction" is propagated. Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30,
//   __adjust_heap(first, holeIndex, len, T value, comp) @ 0x00A33A70 (it owns and destroys value).
// Element: the 16-byte C3DMeshStats volume-table entry pair<unsigned long, C3DMeshVolume> (Ego_r.pdb C3DMeshVolume, 12 bytes:
//   CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the landed swap 0x00A331B0 names it) -- the
//   model of the landed 0x00A333A0 / 0x00A34420 (copy = key, string copy call, the pointer's two words and add-ref; the
//   by-value parameter's destruction = the pointer's release, its zero stores, the string's destructor); ordered by the
//   unsigned key (`jae`/`jb`).
// Shape: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first,
//   parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the count is `sar 4`, first spilled
//   to the push-ecx slot, the copy read through a strength-reduced `first + parent*16 + 8` pointer.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CCPPointerInfo {
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> struct CCountedPointer {
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
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

struct C3DMeshVolume {                                        // 12 bytes (Ego_r.pdb)
    CCharString                         Name;                   // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;           // +0x04
    C3DMeshVolume& operator=(const C3DMeshVolume& other)        // out of line @ 0x00A32960 where not inlined
    {
        Name = other.Name;
        ConvexVolume = other.ConvexVolume;
        return *this;
    }
};

namespace _STL {

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<unsigned long, C3DMeshVolume> TEntry;      // 0x10 bytes

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<unsigned long, ...>
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

}

namespace _STL {

template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value, Compare comp);   // 0x00A33A70

template <class RandomAccessIter, class Compare, class T, class Distance>
void __fastcall __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*)
{
    if (last - first < 2)
        return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}

template void __fastcall __make_heap<TEntry*, CKeyPairCompareLess, TEntry, int>(TEntry*, TEntry*, CKeyPairCompareLess, TEntry*, int*);

}