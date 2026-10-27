// std::find(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CThing* const& val) -- STLport find: forward to __find with the random-access iterator tag
// Leaf: ??$find@PAV?$CIntelligentPointer@VCThing@@@@PAVCThing@@@std@@YIPAV?$CIntelligentPointer@VCThing@@@@PAV1@0ABQAVCThing@@@Z
// Retail: Fable.exe @ 0x006E6950, 18 bytes, /Gr `__fastcall` (first ecx, last edx, the value reference on the stack):
// `lea eax,[esp+4] ; push eax ; mov eax,[esp+8] ; push eax ; call 0x006E66E0 ; ret 4`.
// Identity (probable): STLport 4.6 `find(__first, __last, __val)` = `__find(__first, __last, __val,
// _STLP_ITERATOR_CATEGORY(__first, _InputIter))`; the callee is the landed std::__find<CIntelligentPointer<CThing>*,
// CThing*> 0x006E66E0 (CScriptConversation's PeopleInConversation search), and first/last pass through ecx/edx untouched,
// so this is the same instantiation. The row is unreferenced (the scan's ".rdata 0x012B9138" xref is a false hit inside
// a string); the manifest has no label for it.
// Shape: the empty `random_access_iterator_tag` temporary is homed in the value reference's own argument slot
// (`lea eax,[esp+4]`), as retail does; it needs a user constructor or VC7.1 zero-stores it.
class CBaseObject;
class CThing;
class CBaseIntelligentPointer { public:
    CBaseObject* PItem;                             // +0x00
    void*        PNext;                             // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&);       // 0x006E66E0

template <class _InputIter, class _Tp>
_InputIter __fastcall find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, random_access_iterator_tag());
}

template CIntelligentPointer<CThing>* __fastcall find(CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*,
                                                      CThing* const&);
}