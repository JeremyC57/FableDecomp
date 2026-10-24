// Grade: DIFFER(103v103) -- prologue scheduling only: retail `push ebx ; lea eax,[edx-0Ch] ; mov ebx,eax ; push ebp`, ours pushes ebp one slot earlier; every other byte identical
// _global::__pop_heap_aux(SFlagged* first, SFlagged* last, SFlagged*, CCompare comp) -- SGI pop_heap front end for a heap of 12-byte records ordered by a flag bit: move the back element out, put the root there and sift the old back element down
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3BF0, length 103 bytes (cut 112 with padding), `ret 8` (the unused T* and the
//   empty comparator on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). Byte twins (callee only):
//   0x004C3C60 and 0x00ADB4D0.
// Identity: the element is 12 bytes (`sub edx,0Ch ; imul 2AAAAAABh ; sar edx,1`); its __adjust_heap 0x004C2C70 orders by
//   `test byte ptr [..+8],1` (bit 0 of the third word), so the record carries a flag bit at +8. The manifest names 0x004C2C70
//   `_Adjust_heap<pair<unsigned long,CTextureMorph>*,...,CKeyPairCompareLess<...>>`, but a key-pair compare would read +0:
//   that label is propagated too, so the record is modelled from the bytes. The row's `_Pop_heap_0<pair<float,
//   CSubtitledMessage::CCameraTag>*,...>` label (CCameraTag is 0x18 bytes) is propagated.
// Callee: __adjust_heap @ 0x004C2C70.
// Shape: STLport __pop_heap_aux with __pop_heap inlined; the T(*(last - 1)) temporary is held in esi/edi and the third word in
//   the 12-byte frame slot, the root is copied into the back slot through ebp/ebx, the argument copy is built from registers.
//   The 12-byte frame appears only with a `{ *this = other; }` copy constructor (a memberwise one scalarises the temporary and
//   drops the frame: 86 bytes).
struct SFlagged {                                       // 12 bytes; the comparator tests bit 0 of the word at +8
    long          m_0;
    long          m_4;
    unsigned long m_bFlag : 1;
    unsigned long m_rest  : 31;
    SFlagged(const SFlagged& other) { *this = other; }
};

struct CCompare {
    CCompare() { }
};

void __fastcall __adjust_heap(SFlagged* first, long holeIndex, long len, SFlagged value, CCompare comp);   // 0x004C2C70

inline void __fastcall __pop_heap(SFlagged* first, SFlagged* last, SFlagged* result, SFlagged value, CCompare comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

void __fastcall __pop_heap_aux(SFlagged* first, SFlagged* last, SFlagged*, CCompare comp)
{
    __pop_heap(first, last - 1, last - 1, SFlagged(*(last - 1)), comp, (long*)0);
}
