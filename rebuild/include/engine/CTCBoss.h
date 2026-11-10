#ifndef FABLE_ENGINE_CTCBOSS_H
#define FABLE_ENGINE_CTCBOSS_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CBossDef.h"
class CThingAICreature;
// Full original Ego_r0x2fa7d data; original private composites retained.
class CTCBoss:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return (ETCInterfaceType)173;}
 virtual ETCInterfaceType GetInterfaceType()const;
 virtual CCharString GetClassTypeName()const;
 virtual bool HasFrameUpdate()const;
 virtual void FrameUpdate();
 long GetBossPhase()const;
 void SetNextBossPhase(long,bool);
 bool HasBossDef()const{return !PBossDef.IsNull();}
 const CBossDef& GetBossDef()const{return *PBossDef;}
private:
 CDefPointer<const CBossDef> PBossDef;
 long GenericCounter,StrikesSinceHit,currentBossPhase,NextWeapon;
 bool BCountStrikes,EnablePhaseChanges;
 C3DVector PhaseStartPos; // Original C3DVector+22, shared opt-in alignment1.
 std::vector<CIntelligentPointer<CThingAICreature> > HelperCreatures;
 std::vector<CIntelligentPointer<CThing> > Effects;
};
FABLE_STATIC_ASSERT(sizeof(CTCBoss)==72);
#endif
