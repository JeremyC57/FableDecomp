// CVillageTaskTorch::RemoveThingFromTask(const CThing& thing, bool& removed) -- if the thing is the task's Torch, clear it and report true; otherwise clear the assigned creature and report false
// Leaf: ?RemoveThingFromTask@CVillageTaskTorch@@UAEXABVCThing@@AA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008605b0, 158 bytes, thiscall, `ret 8`.
// Identity (proven): vtable 0x0125A8AC (RTTI .?AVCVillageTaskTorch@@) slot 6; Ego_r.pdb ?RemoveThingFromTask@CVillageTaskTorch@@UAEXABVCThing@@AA_N@Z; the members it clears are Ego_r.pdb Torch (CIntelligentPointer<CThing>, vtable 0x01238C6C) and CreatureAssignedToTask (CIntelligentPointer<const CThing>, vtable 0x01237F1C).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, GetPItem() const 0x00A01B50, SetPItem 0x00A01B90,
//   ~CBaseIntelligentPointer 0x00A01C10 (each `member = 0` builds a CIntelligentPointer temporary: base ctor, vptr, SetPItem(0),
//   then the member's SetPItem(temp.GetPItem()) and the temporary's base destructor).
// Header debt: engine/CVillageTaskTorch.h is a flat byte layout without its CVillageTaskBase base or methods; modelled locally.
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"

#include "engine/CThing.h"
class CThingObject : public CThing { };
class CVillageTaskBase {                               // retail: vptr + one dword
public:
    virtual ~CVillageTaskBase();
    unsigned char _pad04[4];                           // +0x04
};
class CVillageTaskTorch : public CVillageTaskBase {
public:
    CIntelligentPointer<CThing>       Torch;                 // +0x08
    CIntelligentPointer<const CThing> CreatureAssignedToTask;   // +0x10
    virtual void RemoveThingFromTask(const CThing& thing, bool& removed);
};
void CVillageTaskTorch::RemoveThingFromTask(const CThing& thing, bool& removed)
{
    if (Torch.Get() == &thing) {
        Torch = 0;
        removed = true;
    } else {
        CreatureAssignedToTask = 0;
        removed = false;
    }
}