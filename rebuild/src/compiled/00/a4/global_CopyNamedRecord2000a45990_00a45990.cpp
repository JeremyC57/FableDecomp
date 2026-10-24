// _global::Copy_NamedRecord20_00a45990(const SNamedRecord20* first, const SNamedRecord20* last, SNamedRecord20* result, const random_access_iterator_tag&, int*) -- assign [first, last) of 20-byte {name, index, 12-byte record} entries onto result, return the advanced result
// Leaf: Copy_NamedRecord20_00a45990
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A45990, length 122 bytes (`ret 0Ch`).
// Identity: catalog `_Move<CTCInventoryMap::CUIMiniMapMarker*>` is propagated (that marker is 0x1c bytes; this body strides
//   0x14: string assignment 0x0099EFB0 at +0, a word, a 12-byte block). Callers: the fill-n 0x00A45EA0 (landed) and the
//   CCombatWheel assignment 0x00A46790; the element stays descriptive.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   The nested POD record is assigned as a block through `mov ecx,esi ; mov edx,edi` (induction pair at +8).
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

struct CCharString {                            // 4-byte counted string handle
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct SRecord12 { long m_a, m_b, m_c; };      // 12-byte POD (implicit assignment: the pointer-register block copy)

struct SNamedRecord20 {                         // 0x14 bytes
    CCharString m_name;                         // +0x00
    long        m_index;                        // +0x04
    SRecord12   m_record;                       // +0x08
};

SNamedRecord20* __fastcall Copy_NamedRecord20_00a45990(const SNamedRecord20* first, const SNamedRecord20* last, SNamedRecord20* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}