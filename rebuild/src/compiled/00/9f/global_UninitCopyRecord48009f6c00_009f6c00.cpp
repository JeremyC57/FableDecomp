// _global::UninitCopy_Record48_009f6c00(const SRecord48* first, const SRecord48* last, SRecord48* result, const __false_type&) -- placement copy-construct [first, last) of 0x30-byte records (array member copied out of line), return the advanced result
// Leaf: UninitCopy_Record48_009f6c00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F6C00, length 130 bytes (`ret 8`).
// Identity: catalog `_Move<CTCCameraPointScriptedSpline::CKeyCamera*>` is propagated (placement copy, 0x30 stride; CKeyCamera
//   is 0x38). The element is the 0x30-byte record of the landed `__uninitialized_fill_n` 0x009F6C90 in the same TU.
// Callees: the array member's copy constructor 0x009F66A0. Caller: the vector insert-range 0x009F6ED0 (two calls).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack, `ret 8`); the placement new keeps the `test eax,eax` null test on the
//   destination and the advanced result is returned.
//   `T* cur = result` local: with the out-of-line copy constructor in the loop VC7.1 loads result into ebp BEFORE the
//   empty test and returns it from the shared epilogue; the `++result` form gives a separate early-out reading the
//   argument slot (tried). The 3-long record at +0x24 sets the induction pair at +0x2c (= &record + 8).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct CArray12 {                               // 12-byte array member with an out-of-line copy constructor
    void* m_first;
    void* m_last;
    void* m_end;
    CArray12(const CArray12& other);            // 0x009F66A0
};

struct SQuad16 { long m_a, m_b, m_c, m_d; };   // 16-byte POD, implicit copy (the `lea ecx ; lea edx` block copy)

struct SRecord12 {                              // 3-long record with its own user copy constructor
    long m_a, m_b, m_c;
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

struct SRecord48 {                              // 0x30 bytes
    CArray12  m_array;                          // +0x00
    long      m_c;                              // +0x0c
    SQuad16   m_quad;                           // +0x10
    long      m_20;                             // +0x20
    SRecord12 m_record;                         // +0x24
    SRecord48(const SRecord48& o) : m_array(o.m_array), m_c(o.m_c), m_quad(o.m_quad), m_20(o.m_20), m_record(o.m_record) { }
};

SRecord48* __fastcall UninitCopy_Record48_009f6c00(const SRecord48* first, const SRecord48* last, SRecord48* result, const __false_type&)
{
    SRecord48* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SRecord48(*first);
    return cur;
}