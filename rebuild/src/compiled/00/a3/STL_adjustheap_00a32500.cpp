// _STL::__adjust_heap(pair<unsigned long, C3DDummyObject>* first, int holeIndex, int len, pair<unsigned long, C3DDummyObject> val, CKeyPairCompareLess comp) -- heap sift-down for the dummy-object table: move the larger child up until the hole reaches the bottom, then push val back up from there
// Leaf: __adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A32500, 151 bytes (+ int3 padding), `ret 40h` (__fastcall: first in ecx,
//   holeIndex in edx, then len, the 0x38-byte value and the empty comparator on the stack).
// Identity: called by __make_heap 0x00A32CC0 and __partial_sort 0x00A34360 (the inlined __pop_heap) of the C3DMeshStats
//   dummy-object sort; the catalog's "_Adjust_heap<CDebugBox*,int,CDebugBox>" is propagated. Callee: __push_heap(first,
//   holeIndex, topIndex, val, comp) @ 0x00A31410. The 20-byte twin is the landed 0x00A32440.
// Element: pair<unsigned long, C3DDummyObject> (0x38 bytes; Ego_r.pdb C3DDummyObject = CMatrix3x4 Transform + long
//   BoneIndex), ordered by the unsigned key (`jae`).
// Shape: SGI `topIndex = holeIndex; secondChild = 2 * holeIndex + 2; while (secondChild < len) { if (comp(first[sc],
//   first[sc - 1])) sc--; first[holeIndex] = first[sc]; holeIndex = sc; sc = 2 * (sc + 1); } if (sc == len) { ... }
//   __push_heap(first, holeIndex, topIndex, val, comp);` -- the implicit 14-dword assignments, topIndex spilled to the
//   push-ecx slot, and val re-built in the argument area by STLport's pair copy constructor (key, then `rep movsd` x13).
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

void __fastcall __push_heap(TEntry* first, int holeIndex, int topIndex, TEntry val, CKeyPairCompareLess comp);   // 0x00A31410

void __fastcall __adjust_heap(TEntry* first, int holeIndex, int len, TEntry val, CKeyPairCompareLess comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
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
    __push_heap(first, holeIndex, topIndex, val, comp);
}

}