// CVillageTaskHorn::IsThingInvolvedWithTask(const CThing& thing) const -- true when PHorn or CreatureAssignedToTask resolves to the thing
// Leaf: ?IsThingInvolvedWithTask@CVillageTaskHorn@@UBE_NABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860350, 49 bytes, thiscall, `ret 4`.
// Identity (proven): CVillageTaskHorn vtable 0x0125A8D8 (RTTI .?AVCVillageTaskHorn@@) slot 5; the manifest label (from FableWin.pdb) is
//   ?IsThingInvolvedWithTask@CVillageTaskHorn@@UBE_NABVCThing@@@Z; Ego_r.pdb CVillageTaskHorn: PHorn (CThing) +0x08, CreatureAssignedToTask (const CThing) +0x10.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (twice).
// Shape (family K029, representative 0x00860310): `return PHorn.GetPItem() == &thing || CreatureAssignedToTask.GetPItem() == &thing;` --
//   the typed GetPItem wrappers are inline, the two exits load 0 / 1. Header debt: engine/CVillageTaskHorn.h flattens the vptr
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
class CVillageTaskHorn : public CVillageTaskBase {
public:
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const;
    CIntelligentPointer<CThing> PHorn;   // +0x08
    CIntelligentPointer<const CThing> CreatureAssignedToTask;   // +0x10
};
bool CVillageTaskHorn::IsThingInvolvedWithTask(const CThing& thing) const
{
    return PHorn.Get() == &thing || CreatureAssignedToTask.Get() == &thing;
}