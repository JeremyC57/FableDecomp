// CIntelligentPointer<CVillageTaskPickCrops>::CIntelligentPointer(T* item) -- smart-pointer constructor: base CBaseIntelligentPointer(), this instantiation's vtable, then SetPItem
// Leaf: ??0?$CIntelligentPointer@
// Renamed (fistp2, 2026-10-24) from CIntelligentPointer::CIntelligentPointer @ 00/94/CIntelligentPointer_CIntelligentPointer_00940530.cpp: the constructor stores vtable 0x0128B0A0, whose RTTI type descriptor (read from the exe) is `.?AV?$CIntelligentPointer@VCVillageTaskPickCrops@@@@` = CIntelligentPointer<CVillageTaskPickCrops>; body = CBaseIntelligentPointer() @ 0x00A01B10 then the vtable then SetPItem(argument) @ 0x00A01B90; the manifest's rtti-vtables.tsv lists the vtable as CIntelligentPointer without the template argument
// Retail: Fable.exe (TLC, VC7.1) @ 0x00940530, 32 bytes.
#include <stddef.h>
struct CBaseObject;
struct CVillageTaskPickCrops;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                                  // this instantiation's own vtable
    CIntelligentPointer(T* item);
};
template <class T> CIntelligentPointer<T>::CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
template struct CIntelligentPointer<CVillageTaskPickCrops>;
