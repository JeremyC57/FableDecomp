#ifndef FABLE_ENGINE_CISTASK_H
#define FABLE_ENGINE_CISTASK_H
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CVillageTaskBase.h"
// Both raw donors: size 4, unary_function base and private const-reference Task.
class CIsTask : public std::unary_function<CCountedPointer<CVillageTaskBase>,bool> {
    const CVillageTaskBase& Task;
public:
    CIsTask(const CVillageTaskBase& task) : Task(task) { }
    // Native 0065eaf9..0065eb03 compares converted Data against the stored reference.
    bool operator()(const CCountedPointer<CVillageTaskBase>& p) const { return p.Get()==&Task; }
};
FABLE_STATIC_ASSERT(sizeof(CIsTask)==4);
#endif
