// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (std::pair<EExpressionType, CExpressionSet::CEntry>, std::pair<EExpressionType, CExpressionSet::CEntry>) STLport random-access forward copy of 12-byte records: n = last - first, then `*result = *first` n times; returns the advanced result
// Leaf: ??$__copy@PAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@PAU12@H@std@@YIPAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086EFA0, 74 bytes (+ int3 padding), __fastcall (first ecx, last edx; result, the
//   tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy<std::pair<EExpressionType, CExpressionSet::CEntry>*, std::pair<EExpressionType, CExpressionSet::CEntry>*, int>; evidence: its callers are vector<std::pair<EExpressionType, CExpressionSet::CEntry> >::resize 0x00870040 (the shrink arm's erase), the unreferenced erase(first, last) 0x0086FA90 and __copy_ptrs 0x0086F300; 0x00870040 is called by CExpressionSet::TransferBinaryIn 0x0086F680, which CCreatureDef::Transfer 0x00675E20 (slot 18 of vtable 0x0125B43C, RTTI .?AVCCreatureDef@@) runs on [this+0x38] = PExpressions; erase passes pointer sources.
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

template std::pair<EExpressionType, CExpressionSet::CEntry>* __fastcall __copy<std::pair<EExpressionType, CExpressionSet::CEntry>*, std::pair<EExpressionType, CExpressionSet::CEntry>*, int>(std::pair<EExpressionType, CExpressionSet::CEntry>*, std::pair<EExpressionType, CExpressionSet::CEntry>*, std::pair<EExpressionType, CExpressionSet::CEntry>*, const random_access_iterator_tag&, int*);
}   // namespace std