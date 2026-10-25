// _global::UninitFillN_LongIVCountedPointer_005e0ca0(SLongIVCountedPointer_005e0ca0* first, unsigned int n, const SLongIVCountedPointer_005e0ca0& x, const __false_type&) -- STLport __uninitialized_fill_n over 8-byte {long, intrusive counted pointer} records: copy-construct n copies (add-ref the pointee), return first + n
// Leaf: UninitFillN_LongIVCountedPointer_005e0ca0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0CA0, length 47 bytes, `ret 8`.
// Callers: one site, 0x005E2A57 in the vector reallocation 0x005E29D0.
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>*, ...>` is
//   propagated. The element is a word (+0) and a CIVCountedPointer-style intrusive pointer (+4, count at +4 of the
//   pointee) with STLport's memberwise pair copy -- descriptive names.
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> eax as `cur`,
//   n in edx -> esi counted down, &x on the stack -> edx loaded once, the tag reference unused). The key word through ecx,
//   then the pointer copy `P(x.P) { if (P) ++P->RefCount; }`; placement-new null test; /O2 loop-head pad.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRefCounted_005e0ca0 {                   // intrusively counted pointee
    void* __vftable;                            // +0x00
    long  RefCount;                             // +0x04
};

struct SIVCountedPointer_005e0ca0 {             // CIVCountedPointer<T>, 4 bytes
    SRefCounted_005e0ca0* P;
    SIVCountedPointer_005e0ca0(const SIVCountedPointer_005e0ca0& x) : P(x.P) { if (P) ++P->RefCount; }
};

struct SLongIVCountedPointer_005e0ca0 {         // std::pair<long, CIVCountedPointer<T> >, 8 bytes
    long                       first;           // +0x00
    SIVCountedPointer_005e0ca0 second;          // +0x04
    SLongIVCountedPointer_005e0ca0(const SLongIVCountedPointer_005e0ca0& o) : first(o.first), second(o.second) { }
};

SLongIVCountedPointer_005e0ca0* __fastcall UninitFillN_LongIVCountedPointer_005e0ca0(SLongIVCountedPointer_005e0ca0* first, unsigned int n, const SLongIVCountedPointer_005e0ca0& x, const __false_type&)
{
    SLongIVCountedPointer_005e0ca0* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SLongIVCountedPointer_005e0ca0(x);
    return cur;
}