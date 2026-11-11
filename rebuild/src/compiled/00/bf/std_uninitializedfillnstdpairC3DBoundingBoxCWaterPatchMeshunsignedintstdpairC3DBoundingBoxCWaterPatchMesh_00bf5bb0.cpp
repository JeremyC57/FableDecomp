// _global::UninitFillN_Record28_00bf5bb0(SRecord28* first, unsigned int n, const SRecord28& x, const __false_type&) -- placement copy-construct n copies of a 0x1c-byte record into raw storage, return the advanced cursor
// Leaf: UninitFillN_Record28_00bf5bb0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF5BB0, 74 bytes (`ret 8`).
// Identity: the catalog label `fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha>` is propagated (Ego_r C3DAnimationStateAlpha is
//   0x14 bytes; this element is 0x1c, `add eax,1Ch`). The only caller is the reallocating fill-insert 0x00BF5EE0 (the n != 1
//   path, right after the landed __uninitialized_copy sibling 0x00BF5B50 of the same 28-byte record vector; push_back 0x00BF5FE0).
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx, n in edx, the value
//   and the tag reference on the stack, `ret 8`); `for (; n > 0; --n, ++cur) _Construct(cur, x)` with an unsigned count
//   (`test edx,edx ; jbe`, then `dec ecx ; jne`), the placement new keeping its null test, the cursor returned in eax.
//   Same record model as the landed 0x00BF5B50 (memberwise user copy constructor around a 3-long record at +0x0c: the
//   induction pointer at +0x14 next to the cursor).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRecord12 {                              // 3-long record with its own user copy constructor
    long m_a, m_b, m_c;
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

struct SRecord28 {                              // 0x1c bytes
    long      m_0, m_4, m_8;                    // +0x00 .. +0x08
    SRecord12 m_record;                         // +0x0c
    long      m_18;                             // +0x18
    SRecord28(const SRecord28& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_record(o.m_record), m_18(o.m_18) { }
};

SRecord28* __fastcall UninitFillN_Record28_00bf5bb0(SRecord28* first, unsigned int n, const SRecord28& x, const __false_type&)
{
    SRecord28* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SRecord28(x);
    return cur;
}