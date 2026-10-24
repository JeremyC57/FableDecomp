// Grade: DIFFER(103v103) -- prologue scheduling only: retail `push ebx ; lea eax,[edx-0Ch] ; mov ebx,eax ; push ebp`, ours pushes ebp one slot earlier; every other byte identical
// _global::__pop_heap_aux(PairULongTextureMorph* first, PairULongTextureMorph* last, PairULongTextureMorph*, CKeyPairCompareLess comp) -- SGI pop_heap front end for a heap of (unsigned long, CRandomAppearanceMorph::CTextureMorph) pairs: move the back element out, put the root there and sift the old back element down
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3C60, length 103 bytes (cut 112 with padding), `ret 8` (the unused T* and the empty
//   comparator on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Identity: the element is 12 bytes (`sub edx,0Ch ; imul 2AAAAAABh ; sar edx,1`); the callee 0x004C2D10 carries the manifest name
//   `_Adjust_heap<pair<unsigned long,CRandomAppearanceMorph::CTextureMorph>*,...,CKeyPairCompareLess<...>>` and Ego_r.pdb
//   CRandomAppearanceMorph::CTextureMorph is 8 bytes (NewTexture, GroupID), so the row's `_Pop_heap_0<pair<float,
//   CSubtitledMessage::CCameraTag>*,...>` label (CCameraTag is 0x18 bytes) is propagated. Byte twins (callee only): 0x004C3BF0, 0x00ADB4D0.
// Callee: __adjust_heap @ 0x004C2D10.
// Shape: STLport __pop_heap_aux with __pop_heap inlined: the T(*(last - 1)) temporary is held in esi/edi and the third word in
//   its 12-byte frame slot, the pair's implicit assignment writes the root into the back slot through ebp/ebx, the copy for
//   __adjust_heap is built in the argument area from the registers (STLport's explicit pair copy constructor).
//   The 12-byte frame appears only with the pair copy constructor written `{ *this = other; }` (STLport's memberwise form
//   scalarises the temporary and drops the frame: 86 bytes).
namespace CRandomAppearanceMorph {
struct CTextureMorph {                                  // Ego_r.pdb, 8 bytes
    unsigned long NewTexture;                           // +0x00
    unsigned long GroupID;                              // +0x04
};
}

struct PairULongTextureMorph {                          // std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>, 12 bytes
    unsigned long                         first;        // +0x00
    CRandomAppearanceMorph::CTextureMorph second;       // +0x04
    PairULongTextureMorph(const PairULongTextureMorph& other) { *this = other; }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<unsigned long, CTextureMorph>, empty
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(PairULongTextureMorph* first, long holeIndex, long len, PairULongTextureMorph value,
                              CKeyPairCompareLess comp);   // 0x004C2D10

inline void __fastcall __pop_heap(PairULongTextureMorph* first, PairULongTextureMorph* last, PairULongTextureMorph* result,
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
