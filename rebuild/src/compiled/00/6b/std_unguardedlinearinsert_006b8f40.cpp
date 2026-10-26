// std::__unguarded_linear_insert(pair<unsigned long, CCountedPointer<CFaction> >* last, pair<unsigned long, CCountedPointer<CFaction> > val, CKeyPairCompareLess comp) -- STLport insertion-sort step over the faction vector map: shift larger keys up one slot, drop val into the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B8F40, 186 bytes (+ int3 padding), `ret 10h`.
// Callers: 0x006B9EFE (__unguarded_insertion_sort_aux 0x006B9ED0) and 0x006BA801 (__linear_insert 0x006BA7B0).
// Callees: operator delete @ 0x00BFE9BC; the count block's DeleteFunc through its slot +4 (object at +8 in ecx).
// Types: the CFactionManager map is a CVectorMap<unsigned long, CCountedPointer<CFaction> > (manifest LowerBound
//   0x006B8AE0), so the 12-byte element is pair<unsigned long, CCountedPointer<CFaction> > (CCountedPointer = Data +0,
//   Info +4; CCPPointerInfo = RefCount +0, DeleteFunc +4, Data +8, Ego_r.pdb) and the comparator the empty key-only
//   CKeyPairCompareLess (`cmp ecx,eax ; jae` = unsigned key <), as in the landed sort_heap 0x006BA830.
// Shape: STLport 4.x `__unguarded_linear_insert(last, _Tp val, comp)` from a /Gr TU (`__fastcall`: last in ecx, the
//   12-byte val and the 4-byte comparator slot on the stack, `ret 10h`): `next = last; --next; while (comp(val, *next))
//   { *last = *next; last = next; --next; } *last = val;` then val's destructor. The pair's implicit operator= runs the
//   inline CCountedPointer::operator= (the source's Info and Data read first, the old block released through DeleteFunc
//   + operator delete, both words adopted, `inc [Info]`).
// Label correction: the catalog `Vector_CopyWithRefCount` is descriptive.
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
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
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
template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
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

template void __fastcall __unguarded_linear_insert<CFactionEntry*, CFactionEntry, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > >(
    CFactionEntry*, CFactionEntry, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> >);
}