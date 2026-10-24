// _global::UninitCopy_Record28_00bf5b50(const SRecord28* first, const SRecord28* last, SRecord28* result, const __false_type&) -- placement copy-construct [first, last) of 0x1c-byte records into raw storage, return the advanced result
// Leaf: UninitCopy_Record28_00bf5b50
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF5B50, length 81 bytes (`ret 8`).
// Identity: catalog `_Move<CTCInventoryMap::CUIMiniMapMarker*>` is propagated: CUIMiniMapMarker is 0x1c bytes but holds a
//   CCharString at +4 (its copy would call the string copy constructor); this body copies seven plain words. Caller: the
//   reallocating fill-insert 0x00BF5EE0 (two calls; the landed push_back 0x00BF5FE0 is its sibling).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack, `ret 8`); the placement new keeps the `test eax,eax` null test on the
//   destination and the advanced result is returned.
//   Memberwise user copy constructor around a 3-long record at +0x0c: induction pair at +0x14 (= &record + 8).
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

SRecord28* __fastcall UninitCopy_Record28_00bf5b50(const SRecord28* first, const SRecord28* last, SRecord28* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SRecord28(*first);
    return result;
}