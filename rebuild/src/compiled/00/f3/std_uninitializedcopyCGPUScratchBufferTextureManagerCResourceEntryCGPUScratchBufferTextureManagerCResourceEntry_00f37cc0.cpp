// _global::UninitCopy_SRecord2C_00f37cc0(const SRecord2C* first, const SRecord2C* last, SRecord2C* result, const __false_type&) -- STLport __uninitialized_copy over 44-byte trivially-copyable records: placement-copy [first, last) to result, return the advanced result
// Leaf: UninitCopy_SRecord2C_00f37cc0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F37CC0, length 98 bytes (+ int3 padding), `ret 8`.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` (`__fastcall`: first in ecx, last in
// edx, result + the tag reference on the stack; callers push `lea` of a dead slot, then result). Each element is built
// with placement new (hence the null test on the destination, `test eax,eax ; je`) through a memberwise copy
// constructor: 11 direct dword moves through esi (an implicit copy of a POD this wide is a block move through
// esi/edi, GOTCHAS wave 13). Both pointers stepped by 0x2c; result (eax throughout) is returned.
// Label correction: the catalog's `_Copy_impl<std::pair<float,CSubtitledMessage::CCameraTag>*,...>` is a Dinkumware name; the catalog element pair<float, CCameraTag> is propagated (the element is 44 bytes); descriptive name.
inline void* operator new(unsigned int, void* p) { return p; }

struct SRecord2C {                               // 44 bytes (0x2c), dword members
    long m_00;
    long m_04;
    long m_08;
    long m_0c;
    long m_10;
    long m_14;
    long m_18;
    long m_1c;
    long m_20;
    long m_24;
    long m_28;
    SRecord2C(const SRecord2C& o) : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18), m_1c(o.m_1c), m_20(o.m_20), m_24(o.m_24), m_28(o.m_28) { }
};

struct __false_type { __false_type() { } };

SRecord2C* __fastcall UninitCopy_SRecord2C_00f37cc0(const SRecord2C* first, const SRecord2C* last, SRecord2C* result, const __false_type&)
{
    SRecord2C* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SRecord2C(*first);
    return cur;
}