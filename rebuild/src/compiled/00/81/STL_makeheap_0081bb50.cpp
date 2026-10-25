// std::__make_heap(SKeyedBlock40* first, SKeyedBlock40* last, CKeyPairCompareLess comp, SKeyedBlock40*, int*) -- heapify a vector of (long key, 36-byte POD) pairs: sift every parent down from the last one to the root
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081BB50, 119 bytes (+int3 pad), `ret 0Ch` (link-time __fastcall /Gr TU: first in
//   ecx, last in edx, then the empty comparator and the value-type / distance-type tag pointers on the stack).
// Callee: __adjust_heap(first, parent, len, T(*(first + parent)), comp) @ 0x0081AB40 (__fastcall, `ret 30h`; the manifest's
//   `_Adjust_heap<CDebugCone*,int,CDebugCone>` is propagated -- it sifts on the signed first dword).
// Caller: 0x0081D4E8, the __partial_sort of the same 40-byte pairs (0x0081D4D0).
// Label: the catalog's "std_make_heap_40ByteElement" is descriptive; the body is STLport `__make_heap`.
// Identity: the 0x66666667 / sar 4 divide is the 40-byte stride, the element the (long key, 36-byte payload) pair of the
//   landed _Sort_heap 0x0081C700 / sort 0x0081E090 family.
// Shape: the landed 0x00758C90 (44-byte) form with a 9-dword payload: `if (last - first < 2) return; len = last - first;
//   parent = (len - 2) / 2; for (;;) { __adjust_heap(first, parent, len, T(*(first + parent)), comp); if (parent == 0)
//   return; parent--; }` -- len and first homed in the 8-byte frame, the cursor first + parent strength-reduced into ebp,
//   the by-value pair built in the argument area by STLport pair's memberwise user copy constructor (key dword, then the
//   POD payload as `rep movsd` x9).
struct SPayload36 {                         // 36-byte POD payload (copied as one block)
    unsigned long m_words[9];
};

namespace std {
template <class A, class B> struct pair {
    A first;                                // +0x00
    B second;                               // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<long, SPayload36> SKeyedBlock40;

struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SKeyedBlock40& a, const SKeyedBlock40& b) const { return a.first < b.first; }
};

void __fastcall __adjust_heap(SKeyedBlock40* first, int holeIndex, int len, SKeyedBlock40 val, CKeyPairCompareLess comp);   // 0x0081AB40

void __fastcall __make_heap(SKeyedBlock40* first, SKeyedBlock40* last, CKeyPairCompareLess comp, SKeyedBlock40*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, SKeyedBlock40(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
}