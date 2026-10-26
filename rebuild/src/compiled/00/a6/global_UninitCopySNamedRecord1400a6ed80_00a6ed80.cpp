// _global::UninitCopy_SNamedRecord14_00a6ed80(const SNamedRecord14* first, const SNamedRecord14* last, SNamedRecord14* result, const __false_type&) -- STLport __uninitialized_copy over 20-byte {CCharString, 12-byte record, word} records: placement copy-construct, return the advanced result
// Leaf: UninitCopy_SNamedRecord14_00a6ed80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6ED80, length 96 bytes, `ret 8`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` (`__fastcall`: first in ecx, last in
// edx -- spilled to the frame word and reloaded after the call -- result + the tag reference on the stack). Placement new
// (null test on the destination) of the pair's implicit copy constructor: the out-of-line CCharString copy
// (`push ebx ; mov ecx,ebp ; call`), the 12-byte value at +4 through its memberwise USER copy constructor -- the
// strength-reduced src+0xC/dst+0xC pointers address that record's last word -- then the word at +0x10. Same data as
// 0x0088ABC0 but the record sits before the word, which moves the pointers from +0x10 to +0xC. Advanced result returned.
// Label correction: the catalog's `_Copy_impl<NPlayerTargeting::CTargetPoint*,...>` is propagated; the 20-byte element
// is not named by a landed relative -- descriptive names.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);       // 0x0099EC30
};

struct SValue0C {                                // 12-byte value with a memberwise user copy constructor
    long m_word0, m_word4, m_word8;
    SValue0C(const SValue0C& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8) { }
};

struct SNamedRecord14 {                          // 20 bytes
    CCharString m_name;                          // +0x00
    SValue0C    m_record;                        // +0x04
    long        m_value;                         // +0x10
};

struct __false_type { __false_type() { } };

SNamedRecord14* __fastcall UninitCopy_SNamedRecord14_00a6ed80(const SNamedRecord14* first, const SNamedRecord14* last, SNamedRecord14* result, const __false_type&)
{
    SNamedRecord14* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SNamedRecord14(*first);
    return cur;
}