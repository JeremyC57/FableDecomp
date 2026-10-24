// _global::UninitCopy_CountedPointerLong_009ca4c0(const SCountedPointerLong_009ca4c0* first, const SCountedPointerLong_009ca4c0* last, SCountedPointerLong_009ca4c0* result, const __false_type&) -- STLport __uninitialized_copy over 8-byte {intrusive counted pointer, long} records: placement copy-construct (add-ref the pointee), return the advanced result
// Leaf: UninitCopy_CountedPointerLong_009ca4c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CA4C0, length 51 bytes (+13 int3 pad), `ret 8`.
// Callers: 0x009CA5F9 (inside the landed 0x009CA530 neighbourhood), 0x009CB23B (reserve helper 0x009CB1F0) and
//   0x009CB52F / 0x009CB583 (reserve-and-fill helper 0x009CB4E0); all 8-byte element vectors.
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. The element is
//   a CIVCountedPointer-style intrusive pointer (count at +0x04 of the pointee) followed by a long -- the same shape as
//   NGameText::CDataBankTextEntry in the byte twin 0x00A43630, here in another TU; the class is not named by a landed
//   relative, so the record keeps a descriptive name.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack). The implicit memberwise copy constructor runs the pointer's user copy
//   constructor (`P(x.P) { if (P) ++P->RefCount; }`) then copies the long; placement-new null test on the destination;
//   the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRefCounted_009ca4c0 {                   // intrusively counted pointee
    void* __vftable;                            // +0x00
    long  RefCount;                             // +0x04
};

struct SIVCountedPointer_009ca4c0 {             // CIVCountedPointer<T>
    SRefCounted_009ca4c0* P;
    SIVCountedPointer_009ca4c0(const SIVCountedPointer_009ca4c0& x) : P(x.P) { if (P) ++P->RefCount; }
};

struct SCountedPointerLong_009ca4c0 {           // 8 bytes
    SIVCountedPointer_009ca4c0 m_pointer;       // +0x00
    long                       m_value;         // +0x04
};

SCountedPointerLong_009ca4c0* __fastcall UninitCopy_CountedPointerLong_009ca4c0(const SCountedPointerLong_009ca4c0* first, const SCountedPointerLong_009ca4c0* last, SCountedPointerLong_009ca4c0* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SCountedPointerLong_009ca4c0(*first);
    return result;
}