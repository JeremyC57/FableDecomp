// CIntelligentPointer<const CThing>::CIntelligentPointer() -- smart-pointer constructor: base CBaseIntelligentPointer(), this instantiation's vtable
// Leaf: ??0?$CIntelligentPointer@
// Renamed (fistp2, 2026-10-24) from CIntelligentPointer::CIntelligentPointer @ 00/48/CIntelligentPointer_CIntelligentPointer_0048a400.cpp: the constructor stores vtable 0x01237F1C, whose RTTI type descriptor (read from the exe) is `.?AV?$CIntelligentPointer@$$CBVCThing@@@@` = CIntelligentPointer<const CThing>; body = CBaseIntelligentPointer() @ 0x00A01B10 then the vtable; the manifest's rtti-vtables.tsv lists the vtable as CIntelligentPointer without the template argument
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048A400, 18 bytes.
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
    CIntelligentPointer();
};
template <class T> CIntelligentPointer<T>::CIntelligentPointer() : CBaseIntelligentPointer() { }
template struct CIntelligentPointer<const CThing>;
