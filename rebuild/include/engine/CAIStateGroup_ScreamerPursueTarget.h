#ifndef FABLE_ENGINE_CAISTATEGROUP_SCREAMERPURSUETARGET_H
#define FABLE_ENGINE_CAISTATEGROUP_SCREAMERPURSUETARGET_H
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
// 0096DF50: both donors and native vtable 0128CF44 identify the public Update and OnReplace.
class CAIStateGroup_ScreamerPursueTarget : public CAIStateGroupBase {
public:
    virtual void Update();
    virtual void OnReplace();
private:
    CIntelligentPointer<CThing> PTarget;
    unsigned long TickToGetAggressive;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_ScreamerPursueTarget)==0x20);
#endif
