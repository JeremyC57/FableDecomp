#ifndef FABLE_ENGINE_CVILLAGETASKBASE_H
#define FABLE_ENGINE_CVILLAGETASKBASE_H
#include "engine/CBaseObject.h"
class CThing;
// Both raw donors: EVillageTaskType, records 1c28d / 1ceb9.
enum EVillageTaskType { VILLAGE_TASK_TORCH, VILLAGE_TASK_PATROL, VILLAGE_TASK_PICK_CROPS, VILLAGE_TASK_SECURITY_SWEEP, VILLAGE_TASK_HORN, VILLAGE_TASK_CARRY_CRATES };
// Both raw donors: CVillageTaskBase, size 8, CBaseObject primary base.
class CVillageTaskBase : public CBaseObject {
public:
    virtual ~CVillageTaskBase();
    virtual EVillageTaskType GetTaskType() const = 0;
    virtual bool IsValid() const = 0;
    virtual bool IsTaskAvailable() const = 0;
    virtual bool IsThingInvolvedWithTask(const CThing&) const = 0;
    virtual void RemoveThingFromTask(const CThing&, bool&) = 0;
    virtual bool IsCreatureAssignedToTask(const CThing&) const = 0;
    virtual void AssignCreatureToTask(const CThing&) = 0;
    CVillageTaskBase();
    CVillageTaskBase(const CVillageTaskBase&);
    CVillageTaskBase& operator=(const CVillageTaskBase&);
};
FABLE_STATIC_ASSERT(sizeof(CVillageTaskBase)==8);
#endif
