// std::__copy_ptrs(first, last, result, const __false_type&) -- (CQuaternion) STLport forward copy of 16-byte records: n = last - first, then `*result = *first` n times front to back (the implicit assignment is four dword moves); returns the end of the destination
// Leaf: ??$__copy_ptrs@PBVCQuaternion@@PAV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A535F0, 57 bytes (+ int3 padding), __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`.
// Identity: element CQuaternion -- probable; evidence: its only caller 0x00A54BB0 is a vector operator= of 16-byte elements that C3DAnimationSequenceData::InitialiseAsConstant 0x00A4EFC0 (FableWin.pdb name; it fills the rotation track, Ego_r.pdb CQuaternion* RotationTrack) runs on two locals, also wrapped by the CArray operator= 0x00A54DC0; FableWin.pdb has vector<CQuaternion>::operator= and no other 16-byte array in that function.
// Label: the manifest's "Memory_Copy_16ByteAligned" is descriptive / propagated across byte-identical bodies.
// Body: STLport 4.6 _algobase.h: `__copy_ptrs(first, last, result, const __false_type&)` = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above); `__fastcall` on the template definition (the
//   TU's /Gr convention). VC7.1 keeps __copy out of line under plain `inline`, so it is `__forceinline` here (as in G034).
class CQuaternion { public: float X, Y, Z, W; };
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>   // inline in every retail body
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
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
inline _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CQuaternion* __fastcall __copy_ptrs<const CQuaternion*, CQuaternion*>(const CQuaternion*, const CQuaternion*, CQuaternion*, const __false_type&);
}