// CIntelligentPointer<const CThingGameObject>::CIntelligentPointer() -- smart-pointer constructor: base CBaseIntelligentPointer(), this instantiation's vtable
// Leaf: ??0?$CIntelligentPointer@
// Renamed (fistp2, 2026-10-24) from CIntelligentPointer::CIntelligentPointer @ 00/69/CIntelligentPointer_CIntelligentPointer_0069b960.cpp: the constructor stores vtable 0x0125C29C, whose RTTI type descriptor (read from the exe) is `.?AV?$CIntelligentPointer@$$CBVCThingGameObject@@@@` = CIntelligentPointer<const CThingGameObject>; body = CBaseIntelligentPointer() @ 0x00A01B10 then the vtable; the manifest's rtti-vtables.tsv lists the vtable as CIntelligentPointer without the template argument
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069B960, 18 bytes.
#include <stddef.h>
class CBaseObject;
class CThingGameObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();                                  // this instantiation's own vtable
    CIntelligentPointer();
};
template <class T> CIntelligentPointer<T>::CIntelligentPointer() : CBaseIntelligentPointer() { }
template class CIntelligentPointer<const CThingGameObject>;
