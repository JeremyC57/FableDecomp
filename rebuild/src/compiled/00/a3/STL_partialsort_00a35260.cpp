// std::__partial_sort(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* middle, pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume>*, CKeyPairCompareLess comp) -- heap-based partial sort of the mesh-volume table: heapify [first, middle), pop every smaller later entry into it, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A35260, 123 bytes, `ret 0Ch` (__fastcall: first in ecx, middle in edx, last,
//   the value-type pointer and the empty comparator on the stack).
// Identity: called by __introsort_loop 0x00A35490 when the depth limit runs out; the catalog's "Heap_PopHeap" is
//   descriptive. Callees: __make_heap @ 0x00A344E0, CCharString::CCharString(const CCharString&) @ 0x0099EC30, __pop_heap
//   @ 0x00A34420 (landed), sort_heap @ 0x00A34D80 (landed).
// Element: the 16-byte C3DMeshStats volume-table entry pair<unsigned long, C3DMeshVolume> (Ego_r.pdb C3DMeshVolume, 12 bytes:
//   CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the landed swap 0x00A331B0 names it) -- the
//   model of the landed 0x00A333A0 / 0x00A34420 (copy = key, string copy call, the pointer's two words and add-ref; the
//   by-value parameter's destruction = the pointer's release, its zero stores, the string's destructor); ordered by the
//   unsigned key (`jae`/`jb`).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first,
//   middle, i, T(*i), comp, (int*)0); sort_heap(first, middle, comp);` -- make_heap inlined to __make_heap with null
//   value-type and distance pointers, comp in edi (reloaded after the copy borrows edi).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CCPPointerInfo { public:
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> class CCountedPointer { public:
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

class C3DMeshVolume { public:                                        // 12 bytes (Ego_r.pdb)
    CCharString                         Name;                   // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;           // +0x04
    C3DMeshVolume& operator=(const C3DMeshVolume& other)        // out of line @ 0x00A32960 where not inlined
    {
        Name = other.Name;
        ConvexVolume = other.ConvexVolume;
        return *this;
    }
};

namespace std {

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

namespace std {

template <class RandomAccessIter, class Compare, class T, class Distance>
void __fastcall __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*);   // 0x00A344E0

template <class RandomAccessIter, class T, class Compare, class Distance>
void __fastcall __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T value, Compare comp,
                           Distance*);                                                        // 0x00A34420

template <class RandomAccessIter, class Compare>
void __fastcall sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);       // 0x00A34D80

template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (TEntry*)0, (int*)0);
}

template <class RandomAccessIter, class T, class Compare>
void __fastcall __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

template void __fastcall __partial_sort<TEntry*, TEntry, CKeyPairCompareLess>(TEntry*, TEntry*, TEntry*, TEntry*, CKeyPairCompareLess);

}