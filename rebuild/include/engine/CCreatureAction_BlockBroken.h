#ifndef FABLE_ENGINE_CCREATUREACTION_BLOCKBROKEN_H
#define FABLE_ENGINE_CCREATUREACTION_BLOCKBROKEN_H
#include "engine/CCombatActionBase.h"
class CCreatureAction_BlockBroken : public CCombatActionBase {
public:
    CCreatureAction_BlockBroken(CThingCreatureBase&,CThing&);
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsActionReadyForInterruption() const;
    virtual bool IsToFaceTargetAtStart() const;
};
#endif
