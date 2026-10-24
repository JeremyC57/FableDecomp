// _global::UninitFillN_ThreeCountedPointers_00513570(SThreeCountedPointers_00513570* first, unsigned int n, const SThreeCountedPointers_00513570& x, const __false_type&) -- STLport __uninitialized_fill_n over 28-byte {long, three CCountedPointer<T>} records: copy-construct n copies (add a reference to each count block), return first + n
// Leaf: UninitFillN_ThreeCountedPointers_00513570
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00513570, length 92 bytes (+4 int3 pad), `ret 8`.
// Callers: 0x0050121F (0x00501150), 0x00514D5A (0x00514C10), 0x00515911 (0x005158B0) and 0x0051A4FC (0x0051A430).
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>*, ...>` is
//   propagated (the stride is 28 bytes, not a multiple of 8, so no __int64 member). The element is a key word followed by
//   three CCountedPointer<T> {Data, Info} (+4/+8, +0x0C/+0x10, +0x14/+0x18; each copy bumps the count block's first word,
//   as CScriptThing::PImp 0x004ABE90); the types are not named here -- descriptive.
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> eax as `cur`,
//   n in edx -> ecx counted down, &x on the stack -> edx loaded once). The record is FLAT -- the key and the three counted
//   pointers are direct members, copied by the implicit memberwise copy constructor (each pointer through its user copy
//   constructor) -- and VC7.1 keeps a second induction register at the middle pointer's Info (`lea esi,[eax+10h]`).
//   Wrapping the three pointers in a nested record behind a pair-style copy constructor moves that register to +0x18,
//   and a {pointer, pair of pointers} split moves it to +8 (both probed). Placement-new null test.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SCountBlock_00513570 {
    long m_references;                          // +0 reference count
};

struct SCountedPointer_00513570 {               // CCountedPointer<T>, 8 bytes
    void*                 Data;                 // +0
    SCountBlock_00513570* Info;                 // +4
    SCountedPointer_00513570(const SCountedPointer_00513570& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
};

struct SThreeCountedPointers_00513570 {         // 28 bytes, implicit (memberwise) copy
    long                     first;             // +0x00
    SCountedPointer_00513570 m_a;               // +0x04
    SCountedPointer_00513570 m_b;               // +0x0C
    SCountedPointer_00513570 m_c;               // +0x14
};

SThreeCountedPointers_00513570* __fastcall UninitFillN_ThreeCountedPointers_00513570(SThreeCountedPointers_00513570* first, unsigned int n, const SThreeCountedPointers_00513570& x, const __false_type&)
{
    SThreeCountedPointers_00513570* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SThreeCountedPointers_00513570(x);
    return cur;
}