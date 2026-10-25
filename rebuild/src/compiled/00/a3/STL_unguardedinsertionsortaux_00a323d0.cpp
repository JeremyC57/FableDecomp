// std::__unguarded_insertion_sort_aux(pair<unsigned long, C3DDummyObject>* first, pair<unsigned long, C3DDummyObject>* last, pair<unsigned long, C3DDummyObject>*, CKeyPairCompareLess comp) -- insertion-sort pass with no lower-bound test over the dummy-object table, each entry sifted down by its unsigned key
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A323D0, 98 bytes (+ int3 padding), `ret 8` (__fastcall: first in ecx, last
//   in edx, the value-type tag pointer and the empty comparator on the stack). No callees.
// Identity: the __final_insertion_sort tail of the C3DMeshStats dummy-object sort (the 0x38-byte family of
//   __unguarded_linear_insert 0x00A31320 / __linear_insert 0x00A33800); the catalog's "_Fill<CHeroMarriageInfo*,...>" is
//   propagated.
// Shape: the landed 0x007588A0 template: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp)` with
//   the linear insert inlined -- the by-value T(*i) built in the frame by STLport's pair copy constructor (key dword, then
//   the 52-byte C3DDummyObject as `rep movsd` x13), the shifts and the final store the implicit 14-dword assignment, the
//   key compare unsigned (`jae`/`jb`) with the key in ebp.
namespace std {

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

template <class RandomAccessIter, class T, class Compare>
inline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
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

void __fastcall __unguarded_insertion_sort_aux(TEntry* first, TEntry* last, TEntry*, CKeyPairCompareLess comp)
{
    for (TEntry* i = first; i != last; ++i)
        __unguarded_linear_insert(i, TEntry(*i), comp);
}

}