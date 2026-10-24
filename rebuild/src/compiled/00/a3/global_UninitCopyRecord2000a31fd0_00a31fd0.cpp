// _global::UninitCopy_Record20_00a31fd0(const SRecord20_00a31fd0* first, const SRecord20_00a31fd0* last, SRecord20_00a31fd0* result, const __false_type&) -- STLport __uninitialized_copy over 20-byte records: placement copy-construct [first, last), return the advanced result
// Leaf: UninitCopy_Record20_00a31fd0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A31FD0, length 69 bytes (+11 int3 pad), `ret 8`.
// Callers: 0x00A33DEA / 0x00A33E4B in the vector reallocation 0x00A33D90 (catalog "Vector_Reallocate_C3DAnimationStateAlpha";
//   Ego_r.pdb C3DAnimationStateAlpha is 20 bytes, four unnamed words then AlphaWeight +0x10 -- consistent, unproven).
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated; descriptive
//   record names.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack). A memberwise user copy constructor over a nested 3-word record with
//   its own user copy constructor at +0x04 keeps the second induction pair at &record + 8 (`lea edi,[eax+0Ch]` /
//   `lea esi,[ecx+0Ch]`, GOTCHAS 00776b60 / 00b31a30); the first word goes through ebx directly; placement-new null
//   test on the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRecord12_00a31fd0 {                     // 3-word record with its own user copy constructor
    long m_a, m_b, m_c;
    SRecord12_00a31fd0(const SRecord12_00a31fd0& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

struct SRecord20_00a31fd0 {                     // 20 bytes
    long               m_0;                     // +0x00
    SRecord12_00a31fd0 m_record;                // +0x04
    long               m_10;                    // +0x10
    SRecord20_00a31fd0(const SRecord20_00a31fd0& o) : m_0(o.m_0), m_record(o.m_record), m_10(o.m_10) { }
};

SRecord20_00a31fd0* __fastcall UninitCopy_Record20_00a31fd0(const SRecord20_00a31fd0* first, const SRecord20_00a31fd0* last, SRecord20_00a31fd0* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SRecord20_00a31fd0(*first);
    return result;
}