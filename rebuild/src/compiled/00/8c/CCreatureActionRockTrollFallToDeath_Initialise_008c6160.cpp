// 008c6160 CCreatureAction_RockTrollFallToDeath::Initialise
// Flags:
// Leaf: ?Initialise@CCreatureAction_RockTrollFallToDeath@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define CThing CThingLayout
#define CTCGraphicAppearance CTCGraphicAppearanceLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
#undef CTCGraphicAppearance
#define CThingPhysical CThingPhysicalLayout
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectLayout
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBaseLayout
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingAICreature CThingAICreatureLayout
#include "engine/CThingAICreature.h"
#undef CThingAICreature

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBaseFields
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
class CCharStringData;
class CCharString {CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
#pragma pack(push,1)
// Layout: CClass private byte Type16, parent CBaseObject8, native prefix17.
class CClass:public CBaseObject {
 unsigned char unreadClass[8];unsigned char Type;
public:unsigned long GetType()const{return Type;}
};
class CThing:public CClass {unsigned char unreadThing[sizeof(CThingLayout)-sizeof(CClass)];};
class CThingPhysical:public CThing {unsigned char unreadPhysical[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {unsigned char unreadGameObject[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject {unsigned char unreadCreature[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
class CThingAICreature:public CThingCreatureBase {
 unsigned char unreadAI[sizeof(CThingAICreatureLayout)-sizeof(CThingCreatureBase)];
public:void SuspendAI(bool,void*);
};
#pragma pack(pop)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
// Layout: CBaseObject8 then protected intelligent pointer8; native total168.
class CCreatureActionBase:public CBaseObject {
protected:CIntelligentPointer<CThingCreatureBase>PCreature;
private:unsigned char unreadAction[0xa8-16];
public:
 virtual void EditUpdate(float);
 virtual void PerformAction();
 virtual void FinishAction();
 virtual bool IsPlayingAnimation()const;
 virtual long GetAnimDelayTime()const;
 virtual long GetAnimationIndex()const;
 virtual long GetAnimationTransitionInTime()const;
 virtual long GetAnimationTransitionOutTime()const;
 virtual unsigned long GetAnimExtraFlags()const;
 virtual CCreatureActionBase* Clone()const;
 virtual void Initialise();
};
// Layout: native combat base276; current row touches only inherited fields.
class CCombatActionBase:public CCreatureActionBase {
 unsigned char unreadCombat[0x114-0xa8];
public:virtual void Initialise();void SetCombatAnimation(const CCharString&);
};
class CCreatureAction_RockTrollFallToDeath:public CCombatActionBase {public:virtual void Initialise();};
void CCreatureAction_RockTrollFallToDeath::Initialise(){
 {CCharString name("FALL_TO_DEATH");SetCombatAnimation(name);}
 if(PCreature->GetType()==1){
  CThingAICreature*creature=static_cast<CThingAICreature*>(PCreature.Get());
  creature->SuspendAI(true,0);
 }
 CCombatActionBase::Initialise();
}