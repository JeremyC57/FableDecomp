// _global::UninitCopy_SRecord1C_00b8eed0(const SRecord1C* first, const SRecord1C* last, SRecord1C* result, const __false_type&) -- STLport __uninitialized_copy over 28-byte trivially-copyable records: placement-copy [first, last) to result, return the advanced result
// Leaf: UninitCopy_SRecord1C_00b8eed0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8EED0, length 74 bytes, `ret 8`.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` (`__fastcall`: first in ecx, last in
// edx, result + the tag reference on the stack; callers push `lea` of a dead slot, then result). Each element is built
// with placement new (hence the null test on the destination, `test eax,eax ; je`) through a memberwise copy
// constructor: 7 direct dword moves through esi (an implicit copy of a POD this wide is a block move through
// esi/edi, GOTCHAS wave 13). Both pointers stepped by 0x1c; result (eax throughout) is returned.
// Label correction: the catalog's `_Copy_impl<std::pair<float,CSubtitledMessage::CCameraTag>*,...>` is a Dinkumware name; the catalog element pair<float, CCameraTag> is propagated and unverified (the neighbours at 0x00B8F070 are pair<unsigned long, CCountedPointer<CFaction>> helpers); descriptive name.
inline void* operator new(unsigned int, void* p) { return p; }

struct SRecord1C {                               // 28 bytes (0x1c), dword members
    long m_00;
    long m_04;
    long m_08;
    long m_0c;
    long m_10;
    long m_14;
    long m_18;
    SRecord1C(const SRecord1C& o) : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18) { }
};

struct __false_type { __false_type() { } };

SRecord1C* __fastcall UninitCopy_SRecord1C_00b8eed0(const SRecord1C* first, const SRecord1C* last, SRecord1C* result, const __false_type&)
{
    SRecord1C* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SRecord1C(*first);
    return cur;
}