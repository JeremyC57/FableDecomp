// _global::UninitFillN_TwoPointerRecord_008181e0(STwoPointerRecord_008181e0* first, unsigned int n, const STwoPointerRecord_008181e0& x, const __false_type&) -- STLport __uninitialized_fill_n over 28-byte {long, two intrusive counted pointers, long, long, bool, long} records: copy-construct n copies (add-ref both pointees), return first + n
// Leaf: UninitFillN_TwoPointerRecord_008181e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008181E0, length 84 bytes (+12 int3 pad), `ret 8`.
// Callers: one site, 0x00818D64 in the vector insert helper 0x00818CA0.
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>*, ...>` is
//   propagated. The record: a word (+0), two CIVCountedPointer-style intrusive pointers (+4, +8; count at +4 of the
//   pointee), two words (+0x0C, +0x10), a byte (+0x14) and a word (+0x18); not named by a landed relative -- descriptive.
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> eax as `cur`,
//   n in edx -> esi counted down, &x on the stack -> ecx loaded once). A flat record: the implicit memberwise copy goes
//   word by word through edx with the two pointers' user copy constructors (`if (P) ++P->RefCount`) inline -- no second
//   induction register; placement-new null test; /O2 loop-head pad.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRefCounted_008181e0 {                   // intrusively counted pointee
    void* __vftable;                            // +0x00
    long  RefCount;                             // +0x04
};

struct SIVCountedPointer_008181e0 {             // CIVCountedPointer<T>, 4 bytes
    SRefCounted_008181e0* P;
    SIVCountedPointer_008181e0(const SIVCountedPointer_008181e0& x) : P(x.P) { if (P) ++P->RefCount; }
};

struct STwoPointerRecord_008181e0 {             // 28 bytes
    long                       m_0;             // +0x00
    SIVCountedPointer_008181e0 m_first;         // +0x04
    SIVCountedPointer_008181e0 m_second;        // +0x08
    long                       m_c;             // +0x0C
    long                       m_10;            // +0x10
    bool                       m_14;            // +0x14
    long                       m_18;            // +0x18
};

STwoPointerRecord_008181e0* __fastcall UninitFillN_TwoPointerRecord_008181e0(STwoPointerRecord_008181e0* first, unsigned int n, const STwoPointerRecord_008181e0& x, const __false_type&)
{
    STwoPointerRecord_008181e0* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) STwoPointerRecord_008181e0(x);
    return cur;
}