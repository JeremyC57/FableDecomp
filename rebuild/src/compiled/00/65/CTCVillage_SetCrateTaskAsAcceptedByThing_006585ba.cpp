// CTCVillage::SetCrateTaskAsAcceptedByThing(CVillageTaskCarryCrate& task, const CThing& thing) -- give the crate-carrying task to the creature when the task is free or the creature suits it better; true when assigned
// Leaf: ?SetCrateTaskAsAcceptedByThing@CTCVillage@@QAE_NAAVCVillageTaskCarryCrate@@ABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006585BA, 52 bytes (size-optimised TU, no padding), thiscall, `ret 8`.
// Identity (proven): CTCVillage::SetCrateTaskAsAcceptedByThing(CVillageTaskCarryCrate&, const CThing&); evidence: Ego_r.pdb holds the six
//   CTCVillage::Set*TaskAsAcceptedByThing at 0x0069A446.. in the order Torch, Horn, Patrol, Crop (37 bytes), SecuritySweep,
//   Crate with the same spacing as retail's 0x006584C5 / 0x006584F9 / 0x0065852D / (0x00658561) / 0x00658586 / 0x006585BA
//   (this one is Ego_r 0x0069A543); it calls the task's IsTaskAvailable (slot 4) and AssignCreatureToTask (slot 8) and
//   0x00861530, which sits after CVillageTaskCarryCrate's own virtuals (AssignCreatureToTask 0x00860FD0, vtable 0x0125A950); its caller is
//   CAIStateGroup_WorkInWarehouse::Update, the creature's task AI for this task type.
// Label: the manifest's "SetCrateTaskAsAcceptedByThing" is propagated (family J105: five byte-identical task-acceptance bodies; retail has no
//   identical-code folding).
// Callees: CVillageTaskCarryCrate::IsCreatureMoreAppropriateForTask(const CThing&) const 0x00861530 (Ego_r.pdb / FableWin.pdb
//   ?IsCreatureMoreAppropriateForTask@CVillageTaskCarryCrate@@QBE_NABVCThing@@@Z), the task's virtual IsTaskAvailable() const (slot 4, +0x10)
//   and AssignCreatureToTask(const CThing&) (slot 8, +0x20) (vtable slot names from labels_rtti_port.tsv / the 2026-10-22
//   naming review, the PDB signatures).
// Shape: two separate tests, each assigning and returning true (`if (task.IsTaskAvailable()) { assign; return true; } if
//   (task.IsCreatureMoreAppropriateForTask(thing)) { assign; return true; } return false;`): the size optimiser merges the two
//   assign tails and hoists their common `push thing; mov ecx, task` above the first branch, and keeps the explicit `xor al,
//   al` false block (one `||` condition schedules the vptr load first and folds the false return: grade 1 DIFFER(48v52)).
//   A size-optimised TU (odd addresses, no 16-byte alignment): `#pragma optimize("s", on)` (as the landed CVillageTaskHorn
//   destructor 0x0065EE0C; without it grade 2 DIFFER(53v52)).
// Layout: `this` is engine/CTCVillage.h under a layout name (unused); the task chain is a local polymorphic partial view
//   (RTTI CVillageTaskCarryCrate <- CVillageTaskBase <- CBaseObject <- CBaseClass; engine/CVillageTaskCarryCrate.h models the vptr as a data member).
#pragma optimize("s", on)
#define CTCVillage CTCVillagePdb
#include "engine/CTCVillage.h"
#undef CTCVillage

class CThing;
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseObject : public CBaseClass {
public:
    virtual void Validate() const;                          // slot 1
};
enum EVillageTaskType { EVillageTaskType_Force32 = 0x7fffffff };   // enumerators not needed
class CVillageTaskBase : public CBaseObject {
public:
    virtual EVillageTaskType GetTaskType() const = 0;       // slot 2
    virtual bool IsValid() const = 0;                       // slot 3
    virtual bool IsTaskAvailable() const = 0;               // slot 4 (+0x10)
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const = 0;          // slot 5
    virtual void RemoveThingFromTask(const CThing& thing, bool& removed) = 0;     // slot 6
    virtual bool IsCreatureAssignedToTask(const CThing& thing) const = 0;         // slot 7
    virtual void AssignCreatureToTask(const CThing& thing) = 0;                   // slot 8 (+0x20)
    virtual bool IsAnyCreatureAssignedToTask() const = 0;  // slot 9
};
class CVillageTaskCarryCrate : public CVillageTaskBase {
public:
    virtual EVillageTaskType GetTaskType() const;
    virtual bool IsValid() const;
    virtual bool IsTaskAvailable() const;
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const;
    virtual void RemoveThingFromTask(const CThing& thing, bool& removed);
    virtual bool IsCreatureAssignedToTask(const CThing& thing) const;
    virtual void AssignCreatureToTask(const CThing& thing);
    virtual bool IsAnyCreatureAssignedToTask() const;
    bool IsCreatureMoreAppropriateForTask(const CThing& thing) const;   // 0x00861530
};

class CTCVillage : public CTCVillagePdb {
public:
    bool SetCrateTaskAsAcceptedByThing(CVillageTaskCarryCrate& task, const CThing& thing);
};

bool CTCVillage::SetCrateTaskAsAcceptedByThing(CVillageTaskCarryCrate& task, const CThing& thing)
{
    if (task.IsTaskAvailable()) {
        task.AssignCreatureToTask(thing);
        return true;
    }
    if (task.IsCreatureMoreAppropriateForTask(thing)) {
        task.AssignCreatureToTask(thing);
        return true;
    }
    return false;
}