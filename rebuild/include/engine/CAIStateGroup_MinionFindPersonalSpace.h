#ifndef FABLE_ENGINE_CAISTATEGROUP_MINIONFINDPERSONALSPACE_H
#define FABLE_ENGINE_CAISTATEGROUP_MINIONFINDPERSONALSPACE_H
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
class CThingCreatureBase;
// Original complete class: Ego_r member PThingCreature+14 and both PDB methods.
class CAIStateGroup_MinionFindPersonalSpace:public CAIStateGroupBase {
public:virtual void Update();
private:
 bool IsSpaceInvader(const CThingCreatureBase& creature)const;
 CIntelligentPointer<CThingCreatureBase> PThingCreature;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_MinionFindPersonalSpace)==0x1c);
#endif
