// std::__unguarded_partition(pair<unsigned long, CCountedPointer<CFaction> >* first, pair<..>* last, pair<..> pivot, CKeyPairCompareLess comp) -- STLport quicksort partition of the faction vector map by key; returns the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BA100, 103 bytes, `ret 10h`.
// Callers: one site, 0x006BB0E6 (__introsort_loop of the CFactionManager map sort).
// Callees: iter_swap<pair*, pair*> @ 0x006B8E70 (out of line, `__fastcall`; catalog "swap<CCountedPointer<..> >"),
//   operator delete @ 0x00BFE9BC (the by-value pivot's CCountedPointer release, DeleteFunc through slot +4).
// Types: pair<unsigned long, CCountedPointer<CFaction> > (12 bytes: key +0, Data +4, Info +8) and the empty key-only
//   CKeyPairCompareLess, as the landed sort_heap 0x006BA830 / __unguarded_linear_insert 0x006B8F40.
// Shape: STLport 4.x `__unguarded_partition(first, last, _Tp pivot, comp)` from a /Gr TU (`__fastcall`: first in ecx,
//   last in edx, the 12-byte pivot and the 4-byte comparator slot on the stack, `ret 10h`): `for (;;) { while (comp(*first,
//   pivot)) ++first; --last; while (comp(pivot, *last)) --last; if (!(first < last)) return first; iter_swap(first, last);
//   ++first; }` -- the unsigned key compares are `cmp [esi],ebx ; jae` and `cmp ebx,eax ; jae`; the by-value pivot's
//   CCountedPointer is released on the way out.
// Label correction: the catalog `Heap_PartitionByKey` is descriptive.
void operator delete(void* p);                          // 0x00BFE9BC

class CCPPointerInfo { public:                                 // 12 bytes (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void*);               // +0x04
    void*         Data;                                 // +0x08
};

class CFaction;

template <class T>
class CCountedPointer { public:                                // 8 bytes
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}

typedef std::pair<unsigned long, CCountedPointer<CFaction> > CFactionEntry;

template <class K, class V>
class CKeyPairCompareLess { public:                            // empty, passed by value
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class ForwardIter1, class ForwardIter2>
void __fastcall iter_swap(ForwardIter1 a, ForwardIter2 b);     // 0x006B8E70

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

template CFactionEntry* __fastcall __unguarded_partition<CFactionEntry*, CFactionEntry, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > >(
    CFactionEntry*, CFactionEntry*, CFactionEntry, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> >);
}