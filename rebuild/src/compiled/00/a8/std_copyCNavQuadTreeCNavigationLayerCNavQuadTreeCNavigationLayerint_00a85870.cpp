// _global::Copy_Record40_00a85870(const SRecord40* first, const SRecord40* last, SRecord40* result, const random_access_iterator_tag&, int*) -- assign [first, last) of 0x28-byte {three vectors, empty member, flag} records onto result, return the advanced result
// Leaf: Copy_Record40_00a85870
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A85870, length 97 bytes (`ret 0Ch`).
// Identity: catalog `_Move<std::pair<CCharString,...CPersistInfo>*>` is propagated. The body strides 0x28 and calls three
//   different out-of-line vector assignments (0x00A80EE0 at +0, 0x00A83D70 at +0x0c, 0x00A84940 at +0x18), then copies the
//   byte at +0x25 only -- the byte at +0x24 is never touched, an empty member. Callers: CNavQuadTree::Initialise 0x00A7A8D0,
//   CNavQuadTree::LoadFromFile 0x00A7AF60, CTCVillage::OnInitialActivate 0x00A858F0 and vector helpers.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   An empty struct member reproduces the skipped +0x24 byte (the implicit operator= emits nothing for it).
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

struct SVectorA { void* m_first; void* m_last; void* m_end; SVectorA& operator=(const SVectorA& other); };   // 0x00A80EE0
struct SVectorB { void* m_first; void* m_last; void* m_end; SVectorB& operator=(const SVectorB& other); };   // 0x00A83D70
struct SVectorC { void* m_first; void* m_last; void* m_end; SVectorC& operator=(const SVectorC& other); };   // 0x00A84940
struct SEmptyMember { };                        // 1-byte empty member: its implicit operator= copies nothing

struct SRecord40 {                              // 0x28 bytes
    SVectorA     m_a;                           // +0x00
    SVectorB     m_b;                           // +0x0c
    SVectorC     m_c;                           // +0x18
    SEmptyMember m_empty;                       // +0x24 (never copied)
    bool         m_flag;                        // +0x25
};

SRecord40* __fastcall Copy_Record40_00a85870(const SRecord40* first, const SRecord40* last, SRecord40* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}