// std::__copy_ptrs(first, last, result, const __false_type&) -- (std::pair<long, CRandomAppearanceMorph::CSkeletalMorph>) STLport forward copy of 16-byte records: n = last - first, then `*result = *first` n times front to back (the implicit assignment is four dword moves); returns the end of the destination
// Leaf: ??$__copy_ptrs@PBU?$pair@JVCSkeletalMorph@CRandomAppearanceMorph@@@std@@PAU12@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00673700, 57 bytes (+ int3 padding), __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`.
// Identity: element std::pair<long, CRandomAppearanceMorph::CSkeletalMorph> -- proven; evidence: its only caller 0x00674300 is the vector operator= that the landed CRandomAppearanceMorph::operator= 0x00675B60 runs on SkeletalMorphs (Ego_r.pdb CVectorMap<long, CRandomAppearanceMorph::CSkeletalMorph>, a vector of 16-byte pair<long, CSkeletalMorph>; the POD dword copy rules out the CCharString-bearing CSkeletalMorph of another class).
// Label: the manifest's "std::copy_range<16byte_element>" is descriptive / propagated across byte-identical bodies.
// Body: STLport 4.6 _algobase.h: `__copy_ptrs(first, last, result, const __false_type&)` = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above); `__fastcall` on the template definition (the
//   TU's /Gr convention). VC7.1 keeps __copy out of line under plain `inline`, so it is `__forceinline` here (as in G034).
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
class CDefString { public: long Index; };
class CRandomAppearanceMorph {
public:
    class CSkeletalMorph {                                   // 12 bytes (Ego_r.pdb)
    public:
        long GroupID;
        CDefString Name;
        bool AllowVariation;
    };
};
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

template std::pair<long, CRandomAppearanceMorph::CSkeletalMorph>* __fastcall __copy_ptrs<const std::pair<long, CRandomAppearanceMorph::CSkeletalMorph>*, std::pair<long, CRandomAppearanceMorph::CSkeletalMorph>*>(const std::pair<long, CRandomAppearanceMorph::CSkeletalMorph>*, const std::pair<long, CRandomAppearanceMorph::CSkeletalMorph>*, std::pair<long, CRandomAppearanceMorph::CSkeletalMorph>*, const __false_type&);
}