// std::fill(CIntelligentPointer<CThingAICreature>* first, CIntelligentPointer<CThingAICreature>* last, const CIntelligentPointer<CThingAICreature>& value) -- point every intelligent pointer of a range at the same creature
// Leaf: ??$fill@PAV?$CIntelligentPointer@VCThingAICreature@@@@V1@@std@@YIXPAV?$CIntelligentPointer@VCThingAICreature@@@@0ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AF980, 44 bytes, `__fastcall` (first ecx, last edx, value pointer
//   on the stack), `ret 4`.
// Identity: 8-byte stride; each element is assigned by `SetPItem(val.GetPItem())` (CBaseIntelligentPointer::GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90): the inline CIntelligentPointer<T>::operator=. T = CThingAICreature (sweep3, FINDINGS
//   chunk 478; was CThing): the only caller is the fill-insert 0x007AFE20, whose value copy is stamped with vtable
//   0x0123E7E0 (RTTI .?AV?$CIntelligentPointer@VCThingAICreature@@@@), as is the default value of its resize(n)
//   0x007B00C0; that resize is tail-called by 0x007AF710 `add ecx,0x30` = CTCBoss::AllocateHelperCreatures(long)
//   (Ego_r.pdb/FableWin.pdb), and Ego_r CTCBoss::HelperCreatures (+0x30) is vector<CIntelligentPointer<CThingAICreature> >.
//   (0x007AF6C0, formerly cited here as AddDependentThing, is CTCBoss::AddEffect: it pushes a CIntelligentPointer<CThingAICreature>,
//   vtable 0x01238C6C, onto Effects +0x3C.) Byte-identical to 0x007B83D0 (CThingObject), a separate instantiation.
//   One caller, 0x007AFF15.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: STLport fill with the inline operator= (the landed CIntelligentPointer model).

class CThingAICreature;
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

template void __fastcall std::fill<CIntelligentPointer<CThingAICreature>*, CIntelligentPointer<CThingAICreature> >(
    CIntelligentPointer<CThingAICreature>*, CIntelligentPointer<CThingAICreature>*, const CIntelligentPointer<CThingAICreature>&);