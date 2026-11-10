#ifndef FABLE_ENGINE_CAISTATEGROUP_VILLAGETASKTORCH_H
#define FABLE_ENGINE_CAISTATEGROUP_VILLAGETASKTORCH_H
#include "engine/CAIStateGroupBase.h"
class CTCVillageMember; class CVillageTaskTorch;
extern const CCharString ACTION_POINT_01;
class CAIStateGroup_VillageTaskTorch:public CAIStateGroupBase {
public:
 virtual void Initialise(CAIBrain & p0, CThingAICreature & p1, long p2, EScriptAIPriority p3, const CCharString & p4);
 virtual void Update();
 virtual bool IsRunnable();
 virtual void Cleanup();
 static CAIStateGroupBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL GetName();
private:
 CTCVillageMember * PTCVillageMember;
 CIntelligentPointer<CVillageTaskTorch> PTask;
public:
 CAIStateGroup_VillageTaskTorch(const CAIStateGroup_VillageTaskTorch & p0);
 CAIStateGroup_VillageTaskTorch();
 virtual ~CAIStateGroup_VillageTaskTorch();
 CAIStateGroup_VillageTaskTorch & operator=(const CAIStateGroup_VillageTaskTorch & p0);
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_VillageTaskTorch)==32);
#endif
