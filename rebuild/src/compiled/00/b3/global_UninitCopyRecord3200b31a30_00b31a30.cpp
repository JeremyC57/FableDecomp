// _global::UninitCopy_Record32_00b31a30(const SRecord32* first, const SRecord32* last, SRecord32* result, const __false_type&) -- placement copy-construct [first, last) of 0x20-byte records into raw storage, return the advanced result
// Leaf: UninitCopy_Record32_00b31a30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B31A30, length 87 bytes (`ret 8`).
// Identity: catalog `_Move<CTCCameraPointScriptedSpline::CKeyCamera*>` is propagated (placement copy, 0x20 stride; CKeyCamera
//   is 0x38). The element is a descriptive 0x20-byte record. Caller: the vector insert/resize 0x00B31B10 (two calls).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack, `ret 8`); the placement new keeps the `test eax,eax` null test on the
//   destination and the advanced result is returned.
//   Memberwise user copy constructor around a 3-long record with its own user copy constructor at +0x0c: the
//   induction pair sits at +0x14 (= &record + 8), the first word is copied through ecx/eax directly.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRecord12 {                              // 3-long record with its own user copy constructor
    long m_a, m_b, m_c;
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

struct SRecord32 {                              // 0x20 bytes
    long      m_0, m_4, m_8;                    // +0x00 .. +0x08
    SRecord12 m_record;                         // +0x0c
    long      m_18, m_1c;                       // +0x18, +0x1c
    SRecord32(const SRecord32& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_record(o.m_record), m_18(o.m_18), m_1c(o.m_1c) { }
};

SRecord32* __fastcall UninitCopy_Record32_00b31a30(const SRecord32* first, const SRecord32* last, SRecord32* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SRecord32(*first);
    return result;
}