#ifndef FABLE_ENGINE_CCREATUREACTION_SCREAMERINTOAGGRESSIVE_H
#define FABLE_ENGINE_CCREATUREACTION_SCREAMERINTOAGGRESSIVE_H
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
// 0096DF50: original class is TU-local in both PDBs and has no data of its own.
namespace {
class CCreatureAction_ScreamerIntoAggressive : public CCombatActionBase {
public:
    CCreatureAction_ScreamerIntoAggressive(CThingCreatureBase& creature,CThing& target)
        : CCombatActionBase(creature,target,150,CSoundPair()) {}
    virtual void Initialise();
    virtual CCreatureActionBase* Clone() const;
    virtual bool IsToFaceTarget() const { return true; }
    virtual bool IsToFaceTargetAtStart() const { return true; }
};
}
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_ScreamerIntoAggressive)==0x114);
#endif
