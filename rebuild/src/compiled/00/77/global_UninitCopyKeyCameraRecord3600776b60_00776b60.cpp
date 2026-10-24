// _global::UninitCopy_KeyCameraRecord36_00776b60(const SKeyCameraRecord36* first, const SKeyCameraRecord36* last, SKeyCameraRecord36* result, const __false_type&) -- placement copy-construct [first, last) of 0x24-byte key-camera records into raw storage, return the advanced result
// Leaf: UninitCopy_KeyCameraRecord36_00776b60
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00776B60, length 93 bytes (`ret 8`).
// Identity: catalog `_Move<CTCCameraPointScriptedSpline::CKeyCamera*>` is propagated -- the body is a null-tested
//   placement copy (an uninitialized copy, not an assignment) and CKeyCamera is 0x38 bytes in Ego_r.pdb. The element
//   is the 0x24-byte record of the landed `__uninitialized_fill_n` 0x00776BC0 in the same TU (same model reused).
// Caller: the vector fill-insert 0x00777FE0 (two calls).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack, `ret 8`); the placement new keeps the `test eax,eax` null test on the
//   destination and the advanced result is returned.
//   The element has a memberwise USER copy constructor holding a 3-long record with its own user copy constructor:
//   VC7.1 then keeps a second induction pair (`lea edi,[eax+20h] ; lea esi,[ecx+20h]`, = &record + 8) and copies
//   words +4..+0x20 relative to it; the first word goes through ecx/eax directly. A flat POD gives `rep movsd`,
//   a flat memberwise copy constructor gives plain [ecx+N] moves (both tried).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SKeyRecord {                             // 3-long record with its own user copy constructor
    long m_a, m_b, m_c;
    SKeyRecord(const SKeyRecord& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

struct SKeyCameraRecord36 {                     // 0x24 bytes
    long       m_0, m_4, m_8, m_c, m_10, m_14;  // +0x00 .. +0x14
    SKeyRecord m_record;                        // +0x18
    SKeyCameraRecord36(const SKeyCameraRecord36& o)
        : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_c(o.m_c), m_10(o.m_10), m_14(o.m_14), m_record(o.m_record) { }
};

SKeyCameraRecord36* __fastcall UninitCopy_KeyCameraRecord36_00776b60(const SKeyCameraRecord36* first, const SKeyCameraRecord36* last, SKeyCameraRecord36* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SKeyCameraRecord36(*first);
    return result;
}