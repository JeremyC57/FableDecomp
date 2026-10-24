// std::__uninitialized_fill_n(SRecord44* first, unsigned int n, const SRecord44& x, const __false_type&) -- copy-construct n copies of a 44-byte record into raw storage at first and return first + n
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x00F37AD0, 96 bytes, `ret 8`: __fastcall (first ecx -> eax, n edx; x and the empty tag by reference
// on the stack), returning the end pointer in eax. Call sites 0x00F37D7A, 0x00F38022, 0x00F382A7.
// Identity: the catalog's pair<float, CSubtitledMessage::CCameraTag> is propagated -- that pair is 28 bytes and this body
// strides 44 (0x2C). The neighbour 0x00F37D30 is catalogued _Copy_backward<pair<unsigned long, C3DDummyObject>*>, but
// Ego_r.pdb's C3DDummyObject (52 bytes) does not fit either; the element is kept as an anonymous 11-word record.
// Shape: the 0x0040EAC0 shape with eleven words: a FLAT record whose user copy constructor copies each scalar straight
// through esi (a nested record with its own user copy constructor would add a second induction register).
// Placement-new null check kept; n counts down in edx.
#include <new>
struct SRecord44 {                              // 44 bytes, eleven scalar words
    unsigned long m_00, m_04, m_08, m_0c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28;
    SRecord44(const SRecord44& o)
        : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18),
          m_1c(o.m_1c), m_20(o.m_20), m_24(o.m_24), m_28(o.m_28) {}
};
struct __false_type { __false_type() {} };
namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template SRecord44* __fastcall __uninitialized_fill_n(SRecord44*, unsigned int, const SRecord44&, const __false_type&);
}