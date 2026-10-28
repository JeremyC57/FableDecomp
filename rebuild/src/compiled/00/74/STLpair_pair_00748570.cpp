// std::pair::pair(const pair& other) -- copy constructor of pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>: copy the pointer, then the danger status and its timer
// Leaf: ??0?$pair@V?$CIntelligentPointer@$$CBVCThing@@@@VCDangerStatus@NStealth@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00748570, 53 bytes, __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<CIntelligentPointer<CThing>,long><CIntelligentPointer<CThing>,int>" is
//   propagated. The pointer is a CIntelligentPointer<const CThing> (vtable 0x01237F1C, RTTI
//   .?AV?$CIntelligentPointer@$$CBVCThing@@@@) followed by two dwords: the 16-byte element of CTCStealth's
//   danger-thing vector, whose placement copy is the landed _Construct 0x00748A00 and whose remove_if is 0x00748D80 (same
//   TU). Ego_r.pdb NStealth::CDangerStatus: DangerStatus +0 (EDangerStatus), Timer +4 (long). No direct callers.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90.
// Shape: STLport's memberwise pair copy; CDangerStatus has a user constructor but an implicit copy (two dword moves).
class CBaseObject;
class CThing;
class CThingAICreature;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PData;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();                                 // each instantiation has its own vtable
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE };
class CDangerStatus {
public:
    EDangerStatus DangerStatus;                         // +0x00
    long          Timer;                                // +0x04
    CDangerStatus(EDangerStatus status);
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x08
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>::pair(
    const pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>&);
}