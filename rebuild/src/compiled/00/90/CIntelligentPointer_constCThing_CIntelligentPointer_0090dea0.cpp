// CIntelligentPointer<const CThing>::CIntelligentPointer(const CIntelligentPointer<T>&) -- smart-pointer constructor: base CBaseIntelligentPointer(), this instantiation's vtable, then SetPItem
// Leaf: ??0?$CIntelligentPointer@
// Renamed (fistp2, 2026-10-24) from CIntelligentPointer::CIntelligentPointer @ 00/90/CIntelligentPointer_CIntelligentPointer_0090dea0.cpp: the constructor stores vtable 0x01237F1C, whose RTTI type descriptor (read from the exe) is `.?AV?$CIntelligentPointer@$$CBVCThing@@@@` = CIntelligentPointer<const CThing>; body = CBaseIntelligentPointer() @ 0x00A01B10 then the vtable then SetPItem(other.GetPItem()) @ 0x00A01B50/0x00A01B90; the manifest's rtti-vtables.tsv lists the vtable as CIntelligentPointer without the template argument
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090DEA0, 37 bytes.
#include <stddef.h>
struct CBaseObject;
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                                  // this instantiation's own vtable
    CIntelligentPointer(const CIntelligentPointer<T>& other);
};
template <class T> CIntelligentPointer<T>::CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
template struct CIntelligentPointer<const CThing>;
