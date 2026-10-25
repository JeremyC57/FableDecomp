// std::fill(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, const CIntelligentPointer<CThing>& value) -- point every intelligent pointer of a range at the same thing
// Leaf: ??$fill@PAV?$CIntelligentPointer@VCThing@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AF980, 44 bytes, `__fastcall` (first ecx, last edx, value pointer
//   on the stack), `ret 4`.
// Identity: 8-byte stride; each element is assigned by `SetPItem(val.GetPItem())` (CBaseIntelligentPointer::GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90): the inline CIntelligentPointer<T>::operator=. The vector is the one grown by
//   the fill-insert 0x007AFE20, whose insert/resize 0x007B00C0 is reached from AddDependentThing (0x007AF6C0, in the
//   CTCBoss TU): Ego_r.pdb DependentThings is vector<CIntelligentPointer<CThing> > (CCreatureActionBase,
//   CTCBerserk), so T = CThing. The manifest's "pair<long, CIntelligentPointer<CThing>>" has the wrong element
//   (a pair would be 12 bytes). Byte-identical to 0x007B83D0, a separate instantiation (CThingObject). One caller,
//   0x007AFF15.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: STLport fill with the inline operator= (the landed CIntelligentPointer model).

class CThing;
class CBaseObject;

class CBaseIntelligentPointer {                          // 8 bytes
public:
    CBaseObject* PItem;                                  // +0x04
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

namespace std {
// STLport 4.6 fill; the algorithm templates are __fastcall here (first/last in ecx/edx, callee pops the value pointer).
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall std::fill<CIntelligentPointer<CThing>*, CIntelligentPointer<CThing> >(
    CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*, const CIntelligentPointer<CThing>&);