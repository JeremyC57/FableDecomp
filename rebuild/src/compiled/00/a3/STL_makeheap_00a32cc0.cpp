// _STL::__make_heap(pair<unsigned long, C3DDummyObject>* first, pair<unsigned long, C3DDummyObject>* last, CKeyPairCompareLess comp, pair<unsigned long, C3DDummyObject>*, int*) -- SGI make_heap of the dummy-object table: sift every parent down from (len-2)/2 to the root with __adjust_heap, each fed a by-value copy of the parent
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A32CC0, 127 bytes (+ int3 padding), `ret 0Ch` (__fastcall: first in ecx,
//   last in edx, the empty comparator's slot, the value-type pointer and the Distance* on the stack).
// Identity: the 0x38-byte twin of the landed __make_heap 0x00A32C20 (20-byte helper points); called by __partial_sort
//   0x00A34360. The catalog's "make_heap_generic" is descriptive. Callee: __adjust_heap(first, holeIndex, len, T value,
//   comp) @ 0x00A32500.
// Shape: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first,
//   parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the count divides by 56 (reciprocal
//   0x92492493, `sar edx,5`), first and len spilled to the frame, the by-value copy built in the argument area by
//   STLport's pair copy constructor (the key, then the 52-byte second as `rep movsd` x13).
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

void __fastcall __adjust_heap(TEntry* first, int holeIndex, int len, TEntry val, CKeyPairCompareLess comp);   // 0x00A32500

void __fastcall __make_heap(TEntry* first, TEntry* last, CKeyPairCompareLess comp, TEntry*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, TEntry(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}

}