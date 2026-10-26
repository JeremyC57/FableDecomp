// std::__copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*) -- (std::pair<CIntelligentPointer<CThing>, long>*, std::pair<CIntelligentPointer<CThing>, long>*, int) STLport random-access copy: n = last - first assignments through the element's out-of-line operator=, return the result's end
// Flags: /Gr
// Leaf: ??$__copy@PAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@PAU12@H@std@@YIPAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00788F76, 50 bytes (size-optimised TU), __fastcall (/Gr: first ecx, last edx;
//   result, the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy<std::pair<CIntelligentPointer<CThing>, long>*, std::pair<CIntelligentPointer<CThing>, long>*, int>; evidence: its operator= callee 0x00788DF0 is agent952's landed std::pair<CIntelligentPointer<CThing>, long>::operator= (??4?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@QAEAAU01@ABU01@@Z) every 12 bytes; its callers are agent952's landed vector<pair<CIntelligentPointer<CThing>, long> >::erase 0x0080E4E0 (CTCDExplosion::Emitters), the HitThings vector's erase 0x00789085 and the __copy_ptrs forwarder 0x00788FE6
// Label: the manifest's "_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*,CArray<class_CRandomAppearanceMo" is a propagated / behavioural label (family J070: five 50-byte copy loops, each its own instantiation; retail has no identical-code folding).
// Callees: std::pair<CIntelligentPointer<CThing>, long>::operator=(const pair&) 0x00788DF0 (thiscall, `ret 4`), once per element.
// Shape: STLport 4.6 _algobase.h random-access __copy verbatim; the count is the exact pointer difference (`idiv 0xC`, the
//   divisor by push/pop under /Os) and the result pointer is advanced in its own stack slot.
// Element: std::pair<CIntelligentPointer<CThing>, long> (12 bytes: the 8-byte intelligent pointer and the long)
#pragma optimize("s", on)
class CThing;
class CBaseIntelligentPointer {                          // 8 bytes: vptr + PData
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair& operator=(const pair& other);                  // 0x00788DF0 for pair<CIntelligentPointer<CThing>, long> (implicit; out of line in retail)
};
}
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template std::pair<CIntelligentPointer<CThing>, long>* __copy<std::pair<CIntelligentPointer<CThing>, long>*, std::pair<CIntelligentPointer<CThing>, long>*, int>(std::pair<CIntelligentPointer<CThing>, long>*, std::pair<CIntelligentPointer<CThing>, long>*, std::pair<CIntelligentPointer<CThing>, long>*, const random_access_iterator_tag&, int*);
}