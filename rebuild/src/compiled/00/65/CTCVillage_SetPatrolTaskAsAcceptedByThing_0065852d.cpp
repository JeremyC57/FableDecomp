// CTCVillage::SetPatrolTaskAsAcceptedByThing(CVillageTaskPatrol& task, const CThing& thing) -- give the patrol task to the creature when the task is free or the creature suits it better; true when assigned
// Leaf: ?SetPatrolTaskAsAcceptedByThing@CTCVillage@@QAE_NAAVCVillageTaskPatrol@@ABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065852D, 52 bytes (size-optimised TU, no padding), thiscall, `ret 8`.
// Identity (proven): CTCVillage::SetPatrolTaskAsAcceptedByThing(CVillageTaskPatrol&, const CThing&); evidence: Ego_r.pdb holds the six
//   CTCVillage::Set*TaskAsAcceptedByThing at 0x0069A446.. in the order Torch, Horn, Patrol, Crop (37 bytes), SecuritySweep,
//   Crate with the same spacing as retail's 0x006584C5 / 0x006584F9 / 0x0065852D / (0x00658561) / 0x00658586 / 0x006585BA
//   (this one is Ego_r 0x0069A4B2); it calls the task's IsTaskAvailable (slot 4) and AssignCreatureToTask (slot 8) and
//   0x00860AD0, which sits after CVillageTaskPatrol's own virtuals (AssignCreatureToTask 0x00860A00, vtable 0x0125A900); its caller is
//   CAIStateGroup_VillageTaskPatrol::Update 0x0093F3C0 and 0x0093F020, the creature's task AI for this task type.
// Label: the manifest's "SetCrateTaskAsAcceptedByThing" is propagated (family J105: five byte-identical task-acceptance bodies; retail has no
//   identical-code folding).
// Callees: CVillageTaskPatrol::IsCreatureMoreAppropriateForTask(const CThing&) const 0x00860AD0 (Ego_r.pdb / FableWin.pdb
//   ?IsCreatureMoreAppropriateForTask@CVillageTaskPatrol@@QBE_NABVCThing@@@Z), the task's virtual IsTaskAvailable() const (slot 4, +0x10)
//   and AssignCreatureToTask(const CThing&) (slot 8, +0x20) (vtable slot names from labels_rtti_port.tsv / the 2026-10-22
//   naming review, the PDB signatures).
// Shape: two separate tests, each assigning and returning true (`if (task.IsTaskAvailable()) { assign; return true; } if
//   (task.IsCreatureMoreAppropriateForTask(thing)) { assign; return true; } return false;`): the size optimiser merges the two
//   assign tails and hoists their common `push thing; mov ecx, task` above the first branch, and keeps the explicit `xor al,
//   al` false block (one `||` condition schedules the vptr load first and folds the false return: grade 1 DIFFER(48v52)).
//   A size-optimised TU (odd addresses, no 16-byte alignment): `#pragma optimize("s", on)` (as the landed CVillageTaskHorn
//   destructor 0x0065EE0C; without it grade 2 DIFFER(53v52)).
// Layout: `this` is engine/CTCVillage.h under a layout name (unused); the task chain is a local polymorphic partial view
//   (RTTI CVillageTaskPatrol <- CVillageTaskBase <- CBaseObject <- CBaseClass; engine/CVillageTaskPatrol.h models the vptr as a data member).
#pragma optimize("s", on)
#define CTCVillage CTCVillagePdb
#include "engine/CTCVillage.h"
#undef CTCVillage

class CThing;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
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
class CVillageTaskPatrol : public CVillageTaskBase {
public:
    virtual EVillageTaskType GetTaskType() const;
    virtual bool IsValid() const;
    virtual bool IsTaskAvailable() const;
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const;
    virtual void RemoveThingFromTask(const CThing& thing, bool& removed);
    virtual bool IsCreatureAssignedToTask(const CThing& thing) const;
    virtual void AssignCreatureToTask(const CThing& thing);
    virtual bool IsAnyCreatureAssignedToTask() const;
    bool IsCreatureMoreAppropriateForTask(const CThing& thing) const;   // 0x00860AD0
};

class CTCVillage : public CTCVillagePdb {
public:
    bool SetPatrolTaskAsAcceptedByThing(CVillageTaskPatrol& task, const CThing& thing);
};

bool CTCVillage::SetPatrolTaskAsAcceptedByThing(CVillageTaskPatrol& task, const CThing& thing)
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