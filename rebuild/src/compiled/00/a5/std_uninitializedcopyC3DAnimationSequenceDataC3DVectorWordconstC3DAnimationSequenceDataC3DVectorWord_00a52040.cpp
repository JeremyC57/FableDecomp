// _global::UninitCopy_C3DVectorWord_00a52040(const C3DVectorWord* first, const C3DVectorWord* last, C3DVectorWord* result, const __false_type&) -- STLport __uninitialized_copy<C3DVectorWord*>: placement copy-construct 6-byte short vectors, return the advanced result
// Leaf: UninitCopy_C3DVectorWord_00a52040
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A52040, length 52 bytes (+12 int3 pad), `ret 8`.
// Callers: one site, 0x00A54DA3 in the vector<6-byte>::operator= at 0x00A54CB0 (catalog "Array_C3DVectorWord_Assign";
//   `imul 2AAAAAABh` = divide by 6 after the shift).
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. The 6-byte
//   element is taken as Ego_r.pdb C3DVectorWord (short X, Y, Z) after the assigning caller's label; C3DTriangle2 has
//   the same size and copy, so the element name rests on that one label. Byte twins: 0x00A51C60, 0x00A6E350.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last
//   in edx, result + the tag reference on the stack). The implicit (trivial) 6-byte copy is VC7.1's inline block copy
//   through esi/edi (dword + word); placement-new null test on the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct C3DVectorWord {                          // 6 bytes (Ego_r.pdb)
    short X;                                    // +0x00
    short Y;                                    // +0x02
    short Z;                                    // +0x04
};

C3DVectorWord* __fastcall UninitCopy_C3DVectorWord_00a52040(const C3DVectorWord* first, const C3DVectorWord* last, C3DVectorWord* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) C3DVectorWord(*first);
    return result;
}