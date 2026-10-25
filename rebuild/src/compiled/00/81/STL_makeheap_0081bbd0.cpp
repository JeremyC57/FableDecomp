// std::__make_heap(STextureMorphKeyPair* first, STextureMorphKeyPair* last, CKeyPairCompareLess comp, STextureMorphKeyPair*, int*) -- heapify a vector of (unsigned long, CRandomAppearanceMorph::CTextureMorph) pairs: sift every parent down from the last one to the root
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081BBD0, 101 bytes, `ret 0Ch` (link-time __fastcall /Gr TU: first in ecx, last in
//   edx, then the empty comparator and the value-type / distance-type tag pointers on the stack).
// Callee: __adjust_heap(first, parent, len, T(*(first + parent)), comp) @ 0x0081AC20 (manifest `_Adjust_heap<pair<unsigned
//   long,CRandomAppearanceMorph::CTextureMorph>*,...>`; it sifts with an UNSIGNED key compare, `jae`).
// Callers: 0x0081BF29 (make_heap front end) and 0x0081D598 (the 12-byte __partial_sort).
// Label: the catalog's "std_make_heap_12ByteElement" is descriptive; the body is STLport `__make_heap`.
// Identity: the 0x2AAAAAAB / sar 1 divide is the 12-byte stride: a 4-byte key and an 8-byte payload.
// Shape: the landed 0x00758C90 form: `if (last - first < 2) return; len; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` with the cursor
//   strength-reduced into edi; the by-value pair is built in the argument area by STLport pair's memberwise user copy
//   constructor over an 8-byte trivially copyable payload (three direct dword moves, no `add eax,4`).
struct CTextureMorph {                      // CRandomAppearanceMorph::CTextureMorph as seen by the sort: 8 bytes of POD
    long m_a;                               // +0x00
    long m_b;                               // +0x04
};

namespace std {
template <class A, class B> struct pair {
    A first;                                // +0x00
    B second;                               // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<unsigned long, CTextureMorph> STextureMorphKeyPair;

struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const STextureMorphKeyPair& a, const STextureMorphKeyPair& b) const { return a.first < b.first; }
};

void __fastcall __adjust_heap(STextureMorphKeyPair* first, int holeIndex, int len, STextureMorphKeyPair val, CKeyPairCompareLess comp);   // 0x0081AC20

void __fastcall __make_heap(STextureMorphKeyPair* first, STextureMorphKeyPair* last, CKeyPairCompareLess comp, STextureMorphKeyPair*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, STextureMorphKeyPair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
}