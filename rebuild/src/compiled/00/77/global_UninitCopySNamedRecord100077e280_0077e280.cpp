// _global::UninitCopy_SNamedRecord10_0077e280(const SNamedRecord10* first, const SNamedRecord10* last, SNamedRecord10* result, const __false_type&) -- STLport __uninitialized_copy over 16-byte {CCharString, 12-byte record} pairs: placement copy-construct, return the advanced result
// Leaf: UninitCopy_SNamedRecord10_0077e280
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E280, length 90 bytes (+ int3 padding), `ret 8`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` (`__fastcall`: first in ecx, last in
// edx -- spilled to the frame word and reloaded after the call -- result + the tag reference on the stack). Placement new
// (null test on the destination) of the pair's implicit copy constructor: the out-of-line CCharString copy
// (`push esi ; mov ecx,ebp ; call`), then the 12-byte value through its memberwise USER copy constructor (three direct
// dword moves through the strength-reduced src+0xC/dst+0xC pointers). The advanced result is returned.
// Label correction: the catalog's `_Copy_impl<NPlayerTargeting::CTargetPoint*,...>` is propagated; the element is a
// {name, 12-byte value} pair like the landed CVectorMap<CCharString, STargetPoint> @ 0x0077F270 -- descriptive names.
inline void* operator new(unsigned int, void* p) { return p; }

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);       // 0x0099EC30
};

struct SValue0C {                                // 12-byte value with a memberwise user copy constructor
    long m_word0, m_word4, m_word8;
    SValue0C(const SValue0C& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8) { }
};

struct SNamedRecord10 {                          // 16 bytes
    CCharString first;                           // +0x00
    SValue0C    second;                          // +0x04
};

struct __false_type { __false_type() { } };

SNamedRecord10* __fastcall UninitCopy_SNamedRecord10_0077e280(const SNamedRecord10* first, const SNamedRecord10* last, SNamedRecord10* result, const __false_type&)
{
    SNamedRecord10* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SNamedRecord10(*first);
    return cur;
}