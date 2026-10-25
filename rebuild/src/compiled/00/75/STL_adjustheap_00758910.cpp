// std::__adjust_heap(pair<EBodyLocation,CWoundMorphs::CEntry>* first, int holeIndex, int len, pair<EBodyLocation,CWoundMorphs::CEntry> val, CKeyPairCompareLess comp) -- sift a hole down to a leaf through the larger children, then push the value back up from there
// Leaf: __adjust_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00758910, 151 bytes (+9 int3), `ret 34h` (link-time __fastcall: first in ecx,
//   holeIndex in edx, then len, the 0x2C-byte value and the empty comparator on the stack: 4 + 0x2C + 4 = 0x34).
// Callees: __push_heap(first, holeIndex, topIndex, val, comp) @ 0x007583F0 (landed as `_Push_heap`, __fastcall, `ret 34h`).
// Callers: the CWoundMorphs map sort family (make_heap 0x00758C90, partial_sort 0x00759420, sort_heap 0x007590C0 --
//   the landed _Sort_heap cites this address as its sift-down).
// Label: the manifest's `_Adjust_heap<CDebugBox*,int,CDebugBox>` is a propagation (PDB CDebugBox is 84 bytes; the
//   stride here is 0x2C, the wound-morph map entry with its signed EBodyLocation key at +0).
// Shape: STLport `__adjust_heap`: topIndex is homed in the `push ecx` slot; `comp(first[second], first[second - 1])`
//   is the signed key compare (`jge` over a `dec eax`); each element move is the implicit assignment (`rep movsd` x11);
//   the by-value value argument for __push_heap is built in the argument area by the pair's memberwise user copy
//   constructor (key dword, then the 40-byte POD second as `rep movsd` x10).
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

void __fastcall __push_heap(TWoundPair* first, int holeIndex, int topIndex, TWoundPair val, CKeyPairCompareLess comp);   // 0x007583F0

void __fastcall __adjust_heap(TWoundPair* first, int holeIndex, int len, TWoundPair val, CKeyPairCompareLess comp)
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