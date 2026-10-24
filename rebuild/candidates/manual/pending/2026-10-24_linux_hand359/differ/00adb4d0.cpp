// Grade: DIFFER(103v103) -- prologue scheduling only: retail `push ebx ; lea eax,[edx-0Ch] ; mov ebx,eax ; push ebp`, ours pushes ebp one slot earlier; every other byte identical
// _global::__pop_heap_aux(SEntry12* first, SEntry12* last, SEntry12*, CCompare comp) -- SGI pop_heap front end for a heap of 12-byte records keyed by the signed word at +4: move the back element out, put the root there and sift the old back element down
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADB4D0, length 103 bytes, `ret 8` (the unused T* and the empty comparator on
//   the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). Byte twins (callee only): 0x004C3BF0, 0x004C3C60.
// Identity: the element is 12 bytes (`sub edx,0Ch ; imul 2AAAAAABh ; sar edx,1`); its __adjust_heap 0x00ADB000 orders by a
//   SIGNED compare of the word at +4 (`cmp ebp,[..+4-0Ch] ; jge`). The manifest names 0x00ADB000
//   `_Adjust_heap<pair<EExpressionType,CExpressionSet::CEntry>*,...>`, whose key would sit at +0, so the record is modelled
//   from the bytes. The row's `_Pop_heap_0<pair<float,CSubtitledMessage::CCameraTag>*,...>` label (0x1C bytes) is propagated.
// Callee: __adjust_heap @ 0x00ADB000.
// Shape: as 0x004C3BF0: __pop_heap inlined, the temporary in esi/edi plus the 12-byte frame slot, the root copied through
//   ebp/ebx, the argument copy built from registers; the frame needs the `{ *this = other; }` copy constructor.
//   (a named `back` local instead of the temporary is worse: 104 bytes.)
struct SEntry12 {                                       // 12 bytes; the adjust heap compares the signed word at +4
    long m_0;
    long m_key;
    long m_8;
    SEntry12(const SEntry12& other) { *this = other; }
};

struct CCompare {
    CCompare() { }
};

void __fastcall __adjust_heap(SEntry12* first, long holeIndex, long len, SEntry12 value, CCompare comp);   // 0x00ADB000

__forceinline void __fastcall __pop_heap(SEntry12* first, SEntry12* last, SEntry12* result, SEntry12 value, CCompare comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

void __fastcall __pop_heap_aux(SEntry12* first, SEntry12* last, SEntry12*, CCompare comp)
{
    __pop_heap(first, last - 1, last - 1, SEntry12(*(last - 1)), comp, (long*)0);
}
