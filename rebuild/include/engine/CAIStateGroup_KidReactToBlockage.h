#ifndef FABLE_ENGINE_CAISTATEGROUP_KIDREACTTOBLOCKAGE_H
#define FABLE_ENGINE_CAISTATEGROUP_KIDREACTTOBLOCKAGE_H
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
// Layout: both PDBs, original virtual table and native00941fc0 offsets14..2c.
class CAIStateGroup_KidReactToBlockage : public CAIStateGroupBase {
public:
 virtual void InitStateGroup(const CWorld&,CThingAICreature&);
 virtual bool IsRunnable();
 virtual void OnReplace();
 virtual void Update();
 static CAIStateGroupBase* FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL GetName();
private:
 long LastEndState,CurrentActivationMinInterval,LastActivation,CurrentPersistMaxInterval;
 float our_radius,their_radius;
 CIntelligentPointer<CThingCreatureBase> PThingCreature;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_KidReactToBlockage)==0x34);
#endif
