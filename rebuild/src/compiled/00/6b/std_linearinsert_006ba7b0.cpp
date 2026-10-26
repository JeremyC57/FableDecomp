// std::__linear_insert(pair<unsigned long, CCountedPointer<CFaction> >* first, pair<..>* last, pair<..> val, CKeyPairCompareLess comp) -- STLport insertion-sort step over the faction vector map: a new minimum shifts the whole prefix up, anything else takes the unguarded walk
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BA7B0, 111 bytes (+ int3 padding), `ret 10h`.
// Callers: one site, 0x006BAD97 (__insertion_sort 0x006BAD60 of the CFactionManager map sort).
// Callees: __copy_backward<pair*, pair*, int> @ 0x006B9100 (landed), pair<unsigned long, CCountedPointer<CFaction>
//   >::operator= @ 0x006B8E20 (out of line in this TU; catalog "CItem::operator=" is propagated),
//   __unguarded_linear_insert @ 0x006B8F40, operator delete @ 0x00BFE9BC (the by-value val's release).
// Types: pair<unsigned long, CCountedPointer<CFaction> > (12 bytes: key +0, Data +4, Info +8), the empty key-only
//   CKeyPairCompareLess (as the landed sort_heap 0x006BA830).
// Shape: STLport 4.x `__linear_insert(first, last, _Tp val, comp)` from a /Gr TU (`__fastcall`: first in ecx, last in
//   edx, the 12-byte val and the comparator slot on the stack, `ret 10h`): `if (comp(val, *first)) { copy_backward(first,
//   last, last + 1); *first = val; } else __unguarded_linear_insert(last, val, comp);` -- copy_backward is the inline
//   `__copy_backward_ptrs` forwarder (its `random_access_iterator_tag()` temporary homed in the dead comparator slot, a
//   null distance pointer), the second val copy is built in the argument area with the counted pointer's add-ref, and
//   val is released on the shared exit. The pair's memberwise operator= is a same-TU `__declspec(noinline)` body: VC7.1
//   then knows `*first = val` only reads val and keeps val.Info in esi from the copy to the release (a declared-only
//   operator= re-reads the slot after the call and in the release, 128v113).
// Label correction: the catalog `Vector_InsertOrShift` is descriptive.
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
    __declspec(noinline) pair& operator=(const pair& o)   // 0x006B8E20 (the memberwise assignment, out of line; its
    {                                                       // read-only body in the TU lets VC keep val.Info in esi)
        first = o.first;
        second = o.second;
        return *this;
    }
};
}

typedef std::pair<unsigned long, CCountedPointer<CFaction> > CFactionEntry;

template <class K, class V>
class CKeyPairCompareLess { public:                            // empty, passed by value
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
BidirectionalIter2 __fastcall __copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result,
                                              const random_access_iterator_tag&, Distance*);           // 0x006B9100

template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp);                 // 0x006B8F40

template <class RandomAccessIter, class T, class Compare>
void __fastcall __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

template void __fastcall __linear_insert<CFactionEntry*, CFactionEntry, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > >(
    CFactionEntry*, CFactionEntry*, CFactionEntry, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> >);
}