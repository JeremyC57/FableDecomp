// _global::UninitCopy_SNamedRecord14_00a458b0(const SNamedRecord14* first, const SNamedRecord14* last, SNamedRecord14* result, const __false_type&) -- STLport __uninitialized_copy over 20-byte {CCharString, word, 12-byte record} records: placement copy-construct, return the advanced result
// Leaf: UninitCopy_SNamedRecord14_00a458b0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A458B0, length 96 bytes, `ret 8`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` (`__fastcall`: first in ecx, last in
// edx -- spilled to the frame word and reloaded after the call -- result + the tag reference on the stack). Placement new
// (null test on the destination) of the pair's implicit copy constructor: the out-of-line CCharString copy
// (`push ebx ; mov ecx,ebp ; call`), the word at +4, then the 12-byte value at +8 through its memberwise USER copy
// constructor -- the strength-reduced src+0x10/dst+0x10 pointers address that record's last word (the same placement
// as CTargetPoint's C3DVector in 0x00806C00). The advanced result is returned.
// Same element as this chunk's __insertion_sort 0x00A46680 (same TU) and byte-identical to 0x0088ABC0 (another COMDAT copy).
// Label correction: the catalog's `_Copy_impl<NPlayerTargeting::CTargetPoint*,...>` is propagated; the 20-byte element
// is not named by a landed relative -- descriptive names.
inline void* operator new(unsigned int, void* p) { return p; }

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);       // 0x0099EC30
};

struct SValue0C {                                // 12-byte value with a memberwise user copy constructor
    long m_word0, m_word4, m_word8;
    SValue0C(const SValue0C& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8) { }
};

struct SNamedRecord14 {                          // 20 bytes
    CCharString m_name;                          // +0x00
    long        m_value;                         // +0x04
    SValue0C    m_record;                        // +0x08
};

struct __false_type { __false_type() { } };

SNamedRecord14* __fastcall UninitCopy_SNamedRecord14_00a458b0(const SNamedRecord14* first, const SNamedRecord14* last, SNamedRecord14* result, const __false_type&)
{
    SNamedRecord14* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SNamedRecord14(*first);
    return cur;
}