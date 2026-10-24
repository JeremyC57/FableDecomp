// _global::__pop_heap_aux(PairULongTextureMorph* first, PairULongTextureMorph* last, PairULongTextureMorph*, CKeyPairCompareLess comp) -- SGI pop_heap front end for a heap of (unsigned long, CRandomAppearanceMorph::CTextureMorph) pairs: move the back element out, put the root there and sift the old back element down
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471150, length 74 bytes, `ret 8` (the unused T* and the empty comparator on
//   the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). Frame pointer + `leave`, `idiv` by 12, `movsd` copies
//   and `push 0Ch ; pop ecx`: a size-optimised TU without frame-pointer omission.
// Identity: the element is 12 bytes; the callee 0x00471055 ("Heap_AdjustDown") is the matching __adjust_heap, which orders by
//   an UNSIGNED compare of the first word (`cmp ; jae`): CKeyPairCompareLess<unsigned long, CTextureMorph> over
//   pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> (Ego_r.pdb CTextureMorph: NewTexture, GroupID). The row's
//   `_Pop_heap_0<pair<float,CSubtitledMessage::CCameraTag>*,...>` label (CCameraTag is 0x18 bytes) is propagated.
// Callee: __adjust_heap @ 0x00471055.
// Shape: STLport __pop_heap_aux with __pop_heap inlined (`__forceinline`: the size TU would call it): the T(*(last - 1))
//   temporary is held in ebx/eax/ecx, the root is block-copied into the back slot (`movsd` x3, `first` spilled to [ebp-4]),
//   the copy for __adjust_heap is built in the argument area by STLport's explicit pair copy constructor, and the count is
//   ((last - 1) - first) by `idiv`.
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace CRandomAppearanceMorph {
struct CTextureMorph {                                  // Ego_r.pdb, 8 bytes
    unsigned long NewTexture;                           // +0x00
    unsigned long GroupID;                              // +0x04
};
}

struct PairULongTextureMorph {                          // std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>, 12 bytes
    unsigned long                         first;        // +0x00
    CRandomAppearanceMorph::CTextureMorph second;       // +0x04
    PairULongTextureMorph(const PairULongTextureMorph& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<unsigned long, CTextureMorph>, empty
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(PairULongTextureMorph* first, long holeIndex, long len, PairULongTextureMorph value,
                              CKeyPairCompareLess comp);   // 0x00471055

__forceinline void __fastcall __pop_heap(PairULongTextureMorph* first, PairULongTextureMorph* last, PairULongTextureMorph* result,
                                         PairULongTextureMorph value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

void __fastcall __pop_heap_aux(PairULongTextureMorph* first, PairULongTextureMorph* last, PairULongTextureMorph*,
                               CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, PairULongTextureMorph(*(last - 1)), comp, (long*)0);
}