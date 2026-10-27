// std::__copy_ptrs(const CTCHeroAttachableAppearanceModifiers::CModifierEntry* first, const CTCHeroAttachableAppearanceModifiers::CModifierEntry* last, CTCHeroAttachableAppearanceModifiers::CModifierEntry* result, const __false_type&) -- STLport copy over non-trivially-assigned pointers: assign last - first elements forward (the random-access __copy inlined), return the end of the output
// Leaf: ??$__copy_ptrs@PBVCModifierEntry@CTCHeroAttachableAppearanceModifiers@@PAV12@@std@@YIPAVCModifierEntry@CTCHeroAttachableAppearanceModifiers@@PBV12@0PAV12@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00708590, 38 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (proven): both calls come from vector<CTCHeroAttachableAppearanceModifiers::CModifierEntry>::operator= 0x00708CA0 (family L046: the vector whose own vector's __copy 0x00709090 is bound ??$__copy@PBV?$vector@VCModifierEntry@CTCHeroAttachableAppearanceModifiers@@@std@@...); 8-byte stride = sizeof(CModifierEntry).
// Callees: none.
// Shape (family L045): n = (last - first) >> 3; the element's trivial assignment is a two-dword block copy through esi
//   (MSVC copies trivially-assignable classes bitwise, padding included); the source walks as result + (first - result).
class CTCHeroAttachableAppearanceModifiers {
public:
    class CModifierEntry {                                 // Ego_r.pdb, 8 bytes
    public:
        long DefIndex;                                    // +0x00
        bool Rendered;                                    // +0x04
    };
};
namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
}
template CTCHeroAttachableAppearanceModifiers::CModifierEntry* __fastcall std::__copy_ptrs(const CTCHeroAttachableAppearanceModifiers::CModifierEntry*, const CTCHeroAttachableAppearanceModifiers::CModifierEntry*, CTCHeroAttachableAppearanceModifiers::CModifierEntry*, const std::__false_type&);