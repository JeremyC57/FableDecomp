// std::__make_heap(pair<EBodyLocation,CWoundMorphs::CEntry>* first, pair<EBodyLocation,CWoundMorphs::CEntry>* last, CKeyPairCompareLess comp, pair<EBodyLocation,CWoundMorphs::CEntry>*, int*) -- heapify the wound-morph entries: sift every parent down from the last one to the root
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00758C90, 127 bytes (+1 int3), `ret 0Ch` (link-time __fastcall: first in ecx, last in
//   edx, then the empty comparator and the value-type / distance-type tag pointers on the stack).
// Callees: __adjust_heap(first, parent, len, T(*(first + parent)), comp) @ 0x00758910 (__fastcall, `ret 34h`).
// Callers: 0x00758E59 (make_heap front end) and 0x00759438 (__partial_sort 0x00759420).
// Label: the manifest's "Heap_BuildHeap_From" is descriptive; the body is STLport `__make_heap`.
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- len (the
//   0x2E8BA2E9 divide by 44) is computed once; len and first are homed in the 8-byte frame; the cursor first + parent is
//   strength-reduced into ebx; the by-value element is built in the argument area by the pair's memberwise user copy
//   constructor (key dword, then `rep movsd` x10).
enum EBodyLocation { BODY_LOCATION_NONE = 0 };

struct CWoundMorphsEntry {                       // CWoundMorphs::CEntry, 40 bytes of POD
    long Words[10];
};

namespace std {
template <class A, class B> struct pair {
    A first;                                     // +0x00
    B second;                                    // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<EBodyLocation, CWoundMorphsEntry> TWoundPair;

struct CKeyPairCompareLess {                     // empty, passed by value
    bool operator()(const TWoundPair& a, const TWoundPair& b) const { return a.first < b.first; }
};

void __fastcall __adjust_heap(TWoundPair* first, int holeIndex, int len, TWoundPair val, CKeyPairCompareLess comp);   // 0x00758910

void __fastcall __make_heap(TWoundPair* first, TWoundPair* last, CKeyPairCompareLess comp, TWoundPair*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, TWoundPair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
}