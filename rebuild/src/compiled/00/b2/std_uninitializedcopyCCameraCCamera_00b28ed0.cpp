// _global::UninitCopy_Record56_00b28ed0(const SRecord56* first, const SRecord56* last, SRecord56* result, const __false_type&) -- placement copy-construct [first, last) of 0x38-byte records (two flag bytes at +0x24) into raw storage, return the advanced result
// Leaf: UninitCopy_Record56_00b28ed0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B28ED0, length 131 bytes (`ret 8`).
// Identity: catalog `_Move<CTCCameraPointScriptedSpline::CKeyCamera*>`; the 0x38 stride equals sizeof(CKeyCamera) in Ego_r.pdb and
//   the caller is labelled CTCCameraPointScriptedSpline_InsertKeyCamera (0x00B29840), but the PDB layout (float ShuttleSpeed
//   +0x24, CCharString EventName +0x30) disagrees with the body (two bytes at +0x24/+0x25, no string copy), so the record
//   stays descriptive; the body is an uninitialized copy either way.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack, `ret 8`); the placement new keeps the `test eax,eax` null test on the
//   destination and the advanced result is returned.
//   The byte members force a byte-addressable temporary: VC7.1 copies through edx and keeps `last` in ebp
//   (`mov ebp,edx`); the 3-long record at +0x18 sets the induction pair at +0x20 (= &record + 8).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRecord12 {                              // 3-long record with its own user copy constructor
    long m_a, m_b, m_c;
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

struct SRecord56 {                              // 0x38 bytes
    long      m_0, m_4, m_8, m_c, m_10, m_14;   // +0x00 .. +0x14
    SRecord12 m_record;                         // +0x18
    bool      m_flag24;                         // +0x24
    bool      m_flag25;                         // +0x25
    long      m_28, m_2c, m_30, m_34;           // +0x28 .. +0x34
    SRecord56(const SRecord56& o)
        : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_c(o.m_c), m_10(o.m_10), m_14(o.m_14), m_record(o.m_record),
          m_flag24(o.m_flag24), m_flag25(o.m_flag25), m_28(o.m_28), m_2c(o.m_2c), m_30(o.m_30), m_34(o.m_34) { }
};

SRecord56* __fastcall UninitCopy_Record56_00b28ed0(const SRecord56* first, const SRecord56* last, SRecord56* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SRecord56(*first);
    return result;
}