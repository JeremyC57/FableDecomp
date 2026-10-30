// CVillageTaskPickCrops::CVillageTaskPickCrops(CThingObject& thing) -- a village task for the thing: remember it (Crop) with no creature assigned yet
// Leaf: ??0CVillageTaskPickCrops@@QAE@AAVCThingObject@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065EEFF, 50 bytes, thiscall, `ret 4`. Vtable 0x0125A928.
// Identity (proven): it stores vtable 0x0125A928 whose RTTI is .?AVCVillageTaskPickCrops@@; Ego_r.pdb ??0CVillageTaskPickCrops@@QAE@AAVCThingObject@@@Z and members
//   Crop (CIntelligentPointer<CThingObject>, +0x08) / CreatureAssignedToTask (CIntelligentPointer<const CThing>, +0x10); its caller is CTCVillage::AddThingToVillage 0x0065B854.
//   The manifest's "CVillageTaskHorn::CVillageTaskHorn" is propagated across byte-identical bodies.
// Callees: CBaseObject::CBaseObject() 0x0099A380, CIntelligentPointer<CThingObject>(CThingObject*) 0x0065E133, CBaseIntelligentPointer::
//   CBaseIntelligentPointer() 0x00A01B10 (then the CIntelligentPointer<const CThing> vtable 0x01237F1C, inline).
// Shape (family M181, representative 0x0065ED08): base, vptr, then the members in declaration order. Size TU (`push [esp+0Ch]`).
#pragma optimize("s", on)
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                            // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item);                         // out of line
    virtual ~CIntelligentPointer();
};
class CThing;
class CThingObject;
class CVillageTask : public CBaseObject {
public:
    virtual ~CVillageTask();
};
class CVillageTaskPickCrops : public CVillageTask {
public:
    CVillageTaskPickCrops(CThingObject& thing);
    virtual ~CVillageTaskPickCrops();
    CIntelligentPointer<CThingObject> Crop;                     // +0x08
    CIntelligentPointer<const CThing> CreatureAssignedToTask;   // +0x10
};
CVillageTaskPickCrops::CVillageTaskPickCrops(CThingObject& thing)
    : Crop(&thing)
{
}