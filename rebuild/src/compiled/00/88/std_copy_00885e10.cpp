// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight) STLport random-access forward copy of 12-byte records: n = last - first, then `*result = *first` n times; returns the advanced result
// Leaf: ??$__copy@PAUCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAU123@H@std@@YIPAUCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAU123@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00885E10, 74 bytes (+ int3 padding), __fastcall (first ecx, last edx; result, the
//   tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, int>; evidence: its callers are vector<CThemeWeight>::resize 0x00889440 (the shrink arm's erase), the unreferenced erase(iterator) 0x00888D70, erase(first, last) 0x00888E20 and __copy_ptrs 0x00888C30; 0x00889440 runs on this+8 in NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::RestoreDefaultTheme 0x00887380 (PDB-spelled in reloc_bindings; `Themes.resize(0)` then ThemeTransitionActive = false at +0x1C) and 0x00887330 -- Ego_r.pdb Themes CArray<CThemeWeight> at +8 (retail 0xC vector); FableWin.pdb spells the element UCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@ (struct); erase passes pointer sources.
// Label: the manifest's "_Copy_impl<CTCSmokeGenerator::CGenerationPos*,CTCSmokeGenerator::CGenerationPos*>" is propagated (family J112: five byte-identical 12-byte __copy bodies; retail has no
//   identical-code folding).
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` verbatim (the
//   landed 0x0062FC20 body; the landed __copy_backward 0x00ADAB20 spelling): the signed count by the reciprocal divide, one
//   3-dword record copy per iteration (the implicit assignment); `__fastcall` on the template definition is the TU's /Gr.
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
public:
    struct CThemeWeight {                                  // Ego_r.pdb 12 bytes
        long  ThemeID;                                     // +0x0
        float Weight;                                      // +0x4
        bool  LockWeight;                                  // +0x8
    };
};
}

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight* __fastcall __copy<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, int>(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, const random_access_iterator_tag&, int*);
}   // namespace std