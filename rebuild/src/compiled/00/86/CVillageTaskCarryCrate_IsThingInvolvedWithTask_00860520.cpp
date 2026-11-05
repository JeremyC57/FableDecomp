// CVillageTaskCarryCrate::IsThingInvolvedWithTask(const CThing& thing) const -- true when CreatureAssignedToTask or PCrateStack resolves to the thing
// Leaf: ?IsThingInvolvedWithTask@CVillageTaskCarryCrate@@UBE_NABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860520, 49 bytes, thiscall, `ret 4`.
// Identity (proven): CVillageTaskCarryCrate vtable 0x0125A950 (RTTI .?AVCVillageTaskCarryCrate@@) slot 5; the manifest label (from FableWin.pdb) is
//   ?IsThingInvolvedWithTask@CVillageTaskCarryCrate@@UBE_NABVCThing@@@Z; Ego_r.pdb CVillageTaskCarryCrate: CreatureAssignedToTask (const CThing) +0x08, PCrateStack (CThing) +0x10.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (twice).
// Shape (family K029, representative 0x00860310): `return CreatureAssignedToTask.GetPItem() == &thing || PCrateStack.GetPItem() == &thing;` --
//   the typed GetPItem wrappers are inline, the two exits load 0 / 1. Header debt: engine/CVillageTaskCarryCrate.h flattens the vptr
//   and the intelligent pointers into bytes, so the class is modelled locally from the PDB layout.
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

#include "engine/CThing.h"
class CVillageTaskBase {                                  // RTTI base; 8 bytes (vptr + one word)
public:
    virtual ~CVillageTaskBase();
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const = 0;   // slot 5 in retail
    unsigned char _pad_0x04[4];                          // +0x04
};
class CVillageTaskCarryCrate : public CVillageTaskBase {
public:
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const;
    CIntelligentPointer<const CThing> CreatureAssignedToTask;   // +0x08
    CIntelligentPointer<CThing> PCrateStack;   // +0x10
    bool Pickup;                                         // +0x18
    long Priority;                                       // +0x1C
};
bool CVillageTaskCarryCrate::IsThingInvolvedWithTask(const CThing& thing) const
{
    return CreatureAssignedToTask.Get() == &thing || PCrateStack.Get() == &thing;
}