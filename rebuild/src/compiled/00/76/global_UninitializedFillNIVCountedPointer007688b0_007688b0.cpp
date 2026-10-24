// _global::UninitializedFillN_IVCountedPointer_007688b0(SIVCountedPointer_007688b0* first, unsigned int n, const SIVCountedPointer_007688b0& x) -- STLport uninitialized_fill_n<CIVCountedPointer<T>*> (three-argument front end, __false_type helper inlined): copy-construct n intrusive counted pointers (add-ref each), return first + n
// Leaf: UninitializedFillN_IVCountedPointer_007688b0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007688B0, length 41 bytes (+7 int3 pad), `ret 4`.
// Callers: one site, 0x0076B9E7 in the vector resize/insert helper 0x0076B950 (catalog "Vector_Resize_IVCountedPointer";
//   `push &x ; mov edx,n ; mov ecx,first` -- one stack argument).
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<..>>*, ...>` is propagated. The 4-byte
//   element is a CIVCountedPointer<T> (intrusive count at +4 of the pointee, as CDataBankTextEntry::PText 0x00A43630); T is
//   not named here -- descriptive. `ret 4` = the top-level `uninitialized_fill_n(first, n, x)` with
//   `__uninitialized_fill_n(..., __false_type())` inlined (same front end as 0x00440180).
// Shape: `__fastcall` (first in ecx -> eax as `cur`, n in edx -> esi counted down, &x on the stack -> ecx, loaded once).
//   The pointer's user copy constructor `P(x.P) { if (P) ++P->RefCount; }`; placement-new null test; /O2 loop-head pad.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport _IS_POD_ITER result for a class element

struct SRefCounted_007688b0 {                   // intrusively counted pointee
    void* __vftable;                            // +0x00
    long  RefCount;                             // +0x04
};

struct SIVCountedPointer_007688b0 {             // CIVCountedPointer<T>, 4 bytes
    SRefCounted_007688b0* P;
    SIVCountedPointer_007688b0(const SIVCountedPointer_007688b0& x) : P(x.P) { if (P) ++P->RefCount; }
};

__forceinline SIVCountedPointer_007688b0* __uninitialized_fill_n(SIVCountedPointer_007688b0* first, unsigned int n, const SIVCountedPointer_007688b0& x, const __false_type&)
{
    SIVCountedPointer_007688b0* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SIVCountedPointer_007688b0(x);
    return cur;
}

SIVCountedPointer_007688b0* __fastcall UninitializedFillN_IVCountedPointer_007688b0(SIVCountedPointer_007688b0* first, unsigned int n, const SIVCountedPointer_007688b0& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}