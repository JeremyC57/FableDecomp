#ifndef FABLE_ENGINE_CAISTATEGROUP_VILLAGETASKSECURITYSWEEP_H
#define FABLE_ENGINE_CAISTATEGROUP_VILLAGETASKSECURITYSWEEP_H

#ifdef FABLE_RETAIL_VILLAGETASK_SECURITYSWEEP_UPDATE_PASS24
#include "engine/CAIStateGroupBase.h"
#include "engine/CVillageTaskSecuritySweep.h"

class CTCVillageMember;

class CAIStateGroup_VillageTaskSecuritySweep : public CAIStateGroupBase {
public:
    virtual void Update();
private:
    CTCVillageMember* PTCVillageMember;
    CIntelligentPointer<CVillageTaskSecuritySweep> PTask;
};

FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_VillageTaskSecuritySweep) == 0x20);
#endif
#endif
