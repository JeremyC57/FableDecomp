// _global::UninitFillN_IVCountedPointer_005b149f(SIVCountedPointer_005b149f* first, unsigned int n, const SIVCountedPointer_005b149f& x, const __false_type&) -- STLport __uninitialized_fill_n<CIVCountedPointer<T>*> in a size-optimised TU: copy-construct n intrusive counted pointers (add-ref each), return first + n
// Leaf: UninitFillN_IVCountedPointer_005b149f
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B149F, length 36 bytes, `ret 8`.
// Callers: one site, 0x005B32EA in the vector reallocate-and-copy helper 0x005B3275 (catalog "Tattoo_ReallocateAndCopy";
//   `push &tag ; push &x ; mov ecx,first ; call`). Both functions start at unaligned addresses: a size-optimised TU.
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>*, ...>` is
//   propagated. The 4-byte element is a CIVCountedPointer<T> (intrusive count at +4 of the pointee); T is not named here.
//   Byte twin of 0x005F7A1D (another size-optimised TU).
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> eax as `cur`,
//   n in edx -> ecx counted down, &x and the tag reference on the stack). Under `#pragma optimize("s", on)` no callee-saved
//   register is opened, so &x is re-read from its argument slot every iteration (`mov edx,[esp+4] ; mov edx,[edx]`) and
//   the loop head is not aligned; the pointer's user copy constructor adds the reference; placement-new null test.
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRefCounted_005b149f {                   // intrusively counted pointee
    void* __vftable;                            // +0x00
    long  RefCount;                             // +0x04
};

struct SIVCountedPointer_005b149f {             // CIVCountedPointer<T>, 4 bytes
    SRefCounted_005b149f* P;
    SIVCountedPointer_005b149f(const SIVCountedPointer_005b149f& x) : P(x.P) { if (P) ++P->RefCount; }
};

SIVCountedPointer_005b149f* __fastcall UninitFillN_IVCountedPointer_005b149f(SIVCountedPointer_005b149f* first, unsigned int n, const SIVCountedPointer_005b149f& x, const __false_type&)
{
    SIVCountedPointer_005b149f* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SIVCountedPointer_005b149f(x);
    return cur;
}