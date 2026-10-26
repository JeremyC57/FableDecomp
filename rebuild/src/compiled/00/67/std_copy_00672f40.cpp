// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (const std::pair<EExpressionType, CExpressionSet::CEntry>, std::pair<EExpressionType, CExpressionSet::CEntry>) STLport random-access forward copy of 12-byte records: n = last - first, then `*result = *first` n times; returns the advanced result
// Leaf: ??$__copy@PBU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@PAU12@H@std@@YIPAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@0@PBU10@0PAU10@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00672F40, 74 bytes (+ int3 padding), __fastcall (first ecx, last edx; result, the
//   tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy<const std::pair<EExpressionType, CExpressionSet::CEntry>*, std::pair<EExpressionType, CExpressionSet::CEntry>*, int>; evidence: its callers are vector<std::pair<EExpressionType, CExpressionSet::CEntry> >::operator= 0x00674400 (twice: the shrink and the extend arm) and the unreferenced __copy_ptrs 0x00673740; 0x00674400 is reached from the implicit CCreatureDef::operator= 0x00675C00 (called by CCreatureDef::Copy 0x00676A50, slot 19 of vtable 0x0125B43C, RTTI .?AVCCreatureDef@@) on [this+0x38] = Ego_r.pdb CCreatureDef::PExpressions (CCountedPointer<CExpressionSet>; +0x28 PWoundMorphs and +0x30 PRandomAppearanceMorph match the neighbouring calls), whose Expressions map stores this pair (Ego_r.pdb CEntry {Type, AnimIndex}, 8 bytes: the 12-byte stride); operator= passes const_pointer sources.
// Label: the manifest's "_Copy_impl<CTCSmokeGenerator::CGenerationPos*,CTCSmokeGenerator::CGenerationPos*>" is propagated (family J112: five byte-identical 12-byte __copy bodies; retail has no
//   identical-code folding).
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` verbatim (the
//   landed 0x0062FC20 body; the landed __copy_backward 0x00ADAB20 spelling): the signed count by the reciprocal divide, one
//   3-dword record copy per iteration (the implicit assignment); `__fastcall` on the template definition is the TU's /Gr.
enum EExpressionType { };                                // Ego_r.pdb enum (its enumerators are not needed here)
class CExpressionSet {
public:
    class CEntry {                                         // Ego_r.pdb 8 bytes
    public:
        EExpressionType Type;                              // +0x0
        long            AnimIndex;                         // +0x4
    };
};
namespace std {
template <class _T1, class _T2> struct pair {             // STLport pair: implicit assignment
    _T1 first;
    _T2 second;
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

template std::pair<EExpressionType, CExpressionSet::CEntry>* __fastcall __copy<const std::pair<EExpressionType, CExpressionSet::CEntry>*, std::pair<EExpressionType, CExpressionSet::CEntry>*, int>(const std::pair<EExpressionType, CExpressionSet::CEntry>*, const std::pair<EExpressionType, CExpressionSet::CEntry>*, std::pair<EExpressionType, CExpressionSet::CEntry>*, const random_access_iterator_tag&, int*);
}   // namespace std