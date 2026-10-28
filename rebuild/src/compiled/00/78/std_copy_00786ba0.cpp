// std::__copy(const _E* first, const _E* last, _E* result, const random_access_iterator_tag&, int*) -- (_E = CTCSkeletalMorph::CSkeletalMorph) assign [first, last) onto result
// Leaf: ??$__copy@PBVCSkeletalMorph@CTCSkeletalMorph@@PAV12@H@std@@YIPAVCSkeletalMorph@CTCSkeletalMorph@@PBV12@0PAV12@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00786BA0, 80 bytes, `__fastcall` (first ecx, last edx), `ret 0Ch`.
// Identity (proven): called twice by the landed vector<CTCSkeletalMorph::CSkeletalMorph>::operator= 0x007871D0 (const source) and by the __copy_ptrs thunk 0x00786E20; Ego_r.pdb CTCSkeletalMorph::CSkeletalMorph is {long ID, CCharString Name, float Strength}; the mutable-source instantiation is 0x00786EC0.
// Callees: CCharString::operator=(const CCharString&) 0x0099EFB0.
// Shape (family M067, representative 0x0047B8E0): STLport `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result =
//   *__first; ++__first; ++__result; } return __result;` -- 12-byte elements (the 0x2AAAAAAB divide); the implicit element
//   operator= copies the first field, calls CCharString::operator= for the second and copies the third (dword move).
class CCharString {
public:
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
    void* PStringData;
};
class CTCSkeletalMorph {
public:
    class CSkeletalMorph {                                // Ego_r.pdb, 12 bytes
    public:
        long ID;                                          // +0x0
        CCharString Name;                                 // +0x4
        float Strength;                                   // +0x8
    };
};
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
typedef ::CTCSkeletalMorph::CSkeletalMorph _E;
template _E* __fastcall __copy<const _E*, _E*, int>(const _E*, const _E*, _E*, const random_access_iterator_tag&, int*);
}