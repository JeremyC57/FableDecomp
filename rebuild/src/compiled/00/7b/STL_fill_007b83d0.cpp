// std::fill(CIntelligentPointer<CThingObject>* first, CIntelligentPointer<CThingObject>* last, const CIntelligentPointer<CThingObject>& value) -- point every intelligent pointer of a range at the same object
// Leaf: ??$fill@PAV?$CIntelligentPointer@VCThingObject@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B83D0, 44 bytes, `__fastcall` (first ecx, last edx, value pointer
//   on the stack), `ret 4`.
// Identity: 8-byte stride; each element is assigned by `SetPItem(val.GetPItem())` (the inline
//   CIntelligentPointer<T>::operator=). The vector's resize 0x007B9EC0 is called twice from the tavern-table
//   OnInitialActivate (0x007B90F0, between CTCTavernTable's GetSeat/GetServingPoint and CTCTavernBar): Ego_r.pdb
//   CTCTavernTable::Tankards and ::Food (and CTCTavernBar::Jugs) are vector<CIntelligentPointer<CThingObject> >, so
//   T = CThingObject. The manifest's "pair<long, CIntelligentPointer<CThing>>" is propagated. Byte-identical to
//   0x007AF980 (CThing), a separate instantiation. One caller, 0x007B9C35.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: STLport fill with the inline operator= (the landed CIntelligentPointer model).

class CThingObject;
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

template void __fastcall std::fill<CIntelligentPointer<CThingObject>*, CIntelligentPointer<CThingObject> >(
    CIntelligentPointer<CThingObject>*, CIntelligentPointer<CThingObject>*, const CIntelligentPointer<CThingObject>&);