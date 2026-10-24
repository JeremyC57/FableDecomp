// _STL::__unguarded_partition(pair<unsigned long, C3DDummyObject>* first, pair<unsigned long, C3DDummyObject>* last, pair<unsigned long, C3DDummyObject> pivot, CKeyPairCompareLess comp) -- quicksort partition step of the dummy-object table around the pivot's key: advance from both ends, swap out-of-place pairs, return the split point
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A32A70, 109 bytes (+ int3 padding), `ret 3Ch` (__fastcall: first in ecx,
//   last in edx, the 0x38-byte pivot and the empty comparator on the stack). No callees.
// Identity: the __introsort_loop partition of the C3DMeshStats dummy-object sort (0x38-byte family of 0x00A31320 /
//   0x00A33800); the catalog's "partition_swap_14uint" is descriptive.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- the pivot key kept in ebx (unsigned
//   `jae`/`jb`), __iter_swap's `T tmp = *a` built by STLport's pair copy constructor (key, then `rep movsd` x13) into the
//   pivot's own argument slot (its second is never read), the two assignments implicit 14-dword copies.
namespace _STL {

struct CMatrix3x4 { float M[12]; };
struct C3DDummyObject {                                // 0x34 bytes
    CMatrix3x4 Transform;                              // +0x00
    long       BoneIndex;                              // +0x30
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<unsigned long, C3DDummyObject> TEntry;    // 0x38 bytes

struct CKeyPairCompareLess {                           // CKeyPairCompareLess<unsigned long, C3DDummyObject>
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

template <class ForwardIter1, class ForwardIter2, class T>
inline void __iter_swap(ForwardIter1 a, ForwardIter2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 a, ForwardIter2 b)
{
    __iter_swap(a, b, (TEntry*)0);
}

TEntry* __fastcall __unguarded_partition(TEntry* first, TEntry* last, TEntry pivot, CKeyPairCompareLess comp)
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

}