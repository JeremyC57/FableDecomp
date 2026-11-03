#ifndef FABLE_ENGINE_CTCTARGETINGBASE_H
#define FABLE_ENGINE_CTCTARGETINGBASE_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CTargetingDef;class CThingCreatureBase;
enum ECreatureInteractionType{CREATURE_INTERACTION_NULL=0,CREATURE_INTERACTION_CONVERSATION=1,CREATURE_INTERACTION_TAG=2,CREATURE_INTERACTION_MULTI_TAG=4,CREATURE_INTERACTION_PURCHASING=8};
// Layout: original TPI2251c, complete native size56; intelligent pointers
// are eight-byte original storage at unaligned offsets13,21,29,37.
class CTCTargetingBase:public CTCBase{
public: static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)8;}
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual void OnInitialActivate();
public: virtual void BulletTimeFrameUpdate();
public: virtual CThing * GetPBestThingForMeleeAttack() const;
public: virtual CThingCreatureBase * GetPBestEnemyCreatureForAttack() const;
public: virtual CThing * GetPBestThingForProjectileTargetingLock() const;
public: virtual CThingCreatureBase * GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType,float,float) const;
public: virtual CThingCreatureBase * GetPBestCreatureForInteractionType(ECreatureInteractionType) const;
public: virtual CThing * GetPTargetedThing() const;
public: virtual void SetPTargetedThing(CThing *);
public: virtual CThing * GetPProjectileTargetedThing() const;
public: virtual CThing * GetPTargetLockedThing() const;
public: virtual bool GetTargetedPos(C3DVector &,float) const;
public: virtual bool IsTargetedThingInRange(unsigned long) const;
protected: virtual void UpdateNearestInterestingThing();
public: virtual ~CTCTargetingBase();
protected:bool ScriptOverrideIgnoreEverything;unsigned char PNearestInterestingThing[8],PScriptOverrideInterestingThing[8],PScriptOverrideRangedTarget[8],PTargetedThing[8];CDefPointer<const CTargetingDef> PTargetingDef;bool LookAtCamera;
};
FABLE_STATIC_ASSERT(sizeof(CTCTargetingBase)==56);
#endif
