// std::__unguarded_partition(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume> pivot, CKeyPairCompareLess comp) -- quicksort partition step of the mesh-volume table around the pivot's key: advance from both ends, swap out-of-place entries, return the split point, destroy the by-value pivot
// Leaf: __unguarded_partition
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34260, 127 bytes, `ret 14h` (__fastcall: first in ecx, last in edx, the
//   16-byte pivot and the empty comparator on the stack).
// Identity: called by __introsort_loop 0x00A35490; the catalog's "Introsort_Partition_Pair" is descriptive. Callees:
//   swap(pair&, pair&) @ 0x00A331B0 (landed, out of line, a in ecx / b in edx), the count block's DeleteFunc, operator
//   delete @ 0x00BFE9BC, CCharString::~CCharString @ 0x0099EAE0.
// Element: the 16-byte C3DMeshStats volume-table entry pair<unsigned long, C3DMeshVolume> (Ego_r.pdb C3DMeshVolume, 12 bytes:
//   CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the landed swap 0x00A331B0 names it) -- the
//   model of the landed 0x00A333A0 / 0x00A34420 (copy = key, string copy call, the pointer's two words and add-ref; the
//   by-value parameter's destruction = the pointer's release, its zero stores, the string's destructor); ordered by the
//   unsigned key (`jae`/`jb`).
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` with iter_swap = `swap(*a, *b)` (the out-of-line
//   COMDAT), the pivot key in eax; on return the pivot's destructor runs (`xor esi,esi` zero, release, zero stores).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

#include "engine/CCountedPointer.h"


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

template <class T> void __fastcall swap(T& a, T& b);    // 0x00A331B0

template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 a, ForwardIter2 b)
{
    swap(*a, *b);
}

template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __fastcall __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

template TEntry* __fastcall __unguarded_partition<TEntry*, TEntry, CKeyPairCompareLess>(TEntry*, TEntry*, TEntry, CKeyPairCompareLess);

}