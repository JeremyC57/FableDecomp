// _global::UninitFillN_IVCountedPointer_005f7a1d(SIVCountedPointer_005f7a1d* first, unsigned int n, const SIVCountedPointer_005f7a1d& x, const __false_type&) -- STLport __uninitialized_fill_n<CIVCountedPointer<T>*> in a size-optimised TU: copy-construct n intrusive counted pointers (add-ref each), return first + n
// Leaf: UninitFillN_IVCountedPointer_005f7a1d
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F7A1D, length 36 bytes, `ret 8`.
// Callers: one site, 0x005FA17F in the vector resize helper 0x005FA10A (catalog "std_vector_resize_grow"). Both functions
//   start at unaligned addresses: a size-optimised TU.
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>*, ...>` is
//   propagated. The 4-byte element is a CIVCountedPointer<T> (intrusive count at +4 of the pointee); T is not named here.
//   Byte twin of 0x005B149F (another size-optimised TU); kept as a distinct element type.
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> eax as `cur`,
//   n in edx -> ecx counted down, &x and the tag reference on the stack). Under `#pragma optimize("s", on)` no callee-saved
//   register is opened, so &x is re-read from its argument slot every iteration (`mov edx,[esp+4] ; mov edx,[edx]`) and
//   the loop head is not aligned; the pointer's user copy constructor adds the reference; placement-new null test.
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRefCounted_005f7a1d {                   // intrusively counted pointee
    void* __vftable;                            // +0x00
    long  RefCount;                             // +0x04
};

struct SIVCountedPointer_005f7a1d {             // CIVCountedPointer<T>, 4 bytes
    SRefCounted_005f7a1d* P;
    SIVCountedPointer_005f7a1d(const SIVCountedPointer_005f7a1d& x) : P(x.P) { if (P) ++P->RefCount; }
};

SIVCountedPointer_005f7a1d* __fastcall UninitFillN_IVCountedPointer_005f7a1d(SIVCountedPointer_005f7a1d* first, unsigned int n, const SIVCountedPointer_005f7a1d& x, const __false_type&)
{
    SIVCountedPointer_005f7a1d* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SIVCountedPointer_005f7a1d(x);
    return cur;
}