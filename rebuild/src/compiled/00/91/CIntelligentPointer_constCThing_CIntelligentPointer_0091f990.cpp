// CIntelligentPointer<const CThing>::CIntelligentPointer<CThingCreatureBase>(CIntelligentPointer<CThingCreatureBase>&) -- converting smart-pointer constructor: base CBaseIntelligentPointer(), this instantiation's vtable, then SetPItem
// Leaf: ??$?0VCThingCreatureBase@@@?$CIntelligentPointer@$$CBVCThing@@@@QAE@AAV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z
// Name fix (NF382, QUEUE q2): was CIntelligentPointer<const CThing>'s copy constructor ??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@ABV0@@Z,
//   which the table binds at 0x00654D58. This address is the member-template converting constructor from a
//   CIntelligentPointer<CThingCreatureBase>&: the landed binder 0x0094E7B0 calls it under this spelling, and the FableWin callers of
//   ??$?0VCThingCreatureBase@@@?$CIntelligentPointer@$$CBVCThing@@@@QAE@AAV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z
//   (HoverHornetQueen::Attack, CreatureTakeCover::AttackTargetThing, ...) are the retail callers of 0x0091F990. Same bytes.
// Renamed (fistp2, 2026-10-24) from CIntelligentPointer::CIntelligentPointer @ 00/91/CIntelligentPointer_CIntelligentPointer_0091f990.cpp: the constructor stores vtable 0x01237F1C, whose RTTI type descriptor (read from the exe) is `.?AV?$CIntelligentPointer@$$CBVCThing@@@@` = CIntelligentPointer<const CThing>; body = CBaseIntelligentPointer() @ 0x00A01B10 then the vtable then SetPItem(other.GetPItem()) @ 0x00A01B50/0x00A01B90; the manifest's rtti-vtables.tsv lists the vtable as CIntelligentPointer without the template argument
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091F990, 37 bytes.
#include <stddef.h>
class CBaseObject;
class CThing;
class CThingCreatureBase;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();                                  // this instantiation's own vtable
    template <class U> CIntelligentPointer(CIntelligentPointer<U>& other);
};
template <class T> template <class U> CIntelligentPointer<T>::CIntelligentPointer(CIntelligentPointer<U>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
template CIntelligentPointer<const CThing>::CIntelligentPointer(CIntelligentPointer<CThingCreatureBase>&);
