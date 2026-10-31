// 0093dae0 CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending
// Codex parked-50 best attempt: DIFFER(48v48); residue: 48/48 bytes; corrected world/scoped-pointer model matches size; final argument pointer is in ECX instead of EDX.
// Flags:
// Leaf: ?IsCombatSequenceActionPending@CAIStateGroup_MeleeCombatNearby2@@ABE_NXZ
#define CBaseObject Pass9CBaseObjectStorage
#include "engine/CBaseObject.h"
#undef CBaseObject
#define CClass Pass9CClassStorage
#include "engine/CClass.h"
#undef CClass
#define CThingPhysical Pass9CThingPhysicalStorage
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject Pass9CThingGameObjectStorage
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase Pass9CThingCreatureBaseStorage
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingAICreature Pass9CThingAICreatureStorage
#include "engine/CThingAICreature.h"
#undef CThingAICreature
#define CAIStateGroupBase Pass9CAIStateGroupBaseStorage
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CAIStateGroup_MeleeCombatNearbyBase Pass9CAIStateGroup_MeleeCombatNearbyBaseStorage
#include "engine/CAIStateGroup_MeleeCombatNearbyBase.h"
#undef CAIStateGroup_MeleeCombatNearbyBase
#define CAIStateGroup_MeleeCombatNearby2 Pass9CAIStateGroup_MeleeCombatNearby2Storage
#include "engine/CAIStateGroup_MeleeCombatNearby2.h"
#undef CAIStateGroup_MeleeCombatNearby2

#define CAIBrain CAIBrainFields
#include "engine/CAIBrain.h"
#undef CAIBrain
#include "engine/CSuspendableProcessMicrothread.h"
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
#include "engine/CBaseClassNonCopyable.h"

class CBaseObject:public CBaseClass{unsigned char pass9_unread[4];protected:virtual~CBaseObject();};
class CClass:public CBaseObject{unsigned char pass9_unread[sizeof(Pass9CClassStorage)-sizeof(CBaseObject)];};
// Layout: primary Thing ancestry at0; no Thing fields are read by this row.
class CThing:public CClass{unsigned char pass9_unread[0x98-sizeof(CClass)];};
class CThingPhysical:public CThing{unsigned char pass9_unread[sizeof(Pass9CThingPhysicalStorage)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical{unsigned char pass9_unread[sizeof(Pass9CThingGameObjectStorage)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject{unsigned char pass9_unread[sizeof(Pass9CThingCreatureBaseStorage)-sizeof(CThingGameObject)];public:bool IsPerformingAnyAction()const;};
class CThingAICreature:public CThingCreatureBase{unsigned char pass9_unread[sizeof(Pass9CThingAICreatureStorage)-sizeof(CThingCreatureBase)];};
// Layout: raw PDB private reference Creature24 follows the true20-byte microthread base.
class CMainGameComponent;
class CAIBrain:public CSuspendableProcessMicrothread {
 const CMainGameComponent& Component;
 CThingAICreature& Creature;
 unsigned char unreadBrainTail[sizeof(CAIBrainFields)-28];
public:
 CThingAICreature& GetCreature()const {return Creature;}
};
class CCombatManager{public:bool IsCreaturePerformingNonInterruptableSequence(const CThingCreatureBase&)const;};
namespace boost{template<class T>class scoped_ptr{T*px;public:T&operator*()const{return *px;}};}
class CProceduralTexture;class CIDrawEnvironment;class CIDrawWorldMap;
class CIDrawWorld{public:virtual const CProceduralTexture*DrawGetProceduralTexture()const=0;virtual const CIDrawEnvironment*DrawGetEnvironment()const=0;virtual const CIDrawWorldMap*DrawGetWorldMap()const=0;virtual~CIDrawWorld();};
// Layout: true original two-base world prefix; protected combat scoped-pointer4c.
class CWorld:public CBaseClassNonCopyable,public CIDrawWorld{
 unsigned char unreadWorld[0x4c-8];
protected:boost::scoped_ptr<CCombatManager>PThingCombatManager;
public:CCombatManager&GetCombatManager(){return *PThingCombatManager;}
};
class CAIStateGroupBase:public CBaseClass{protected:CAIBrain*PBrain;unsigned char pass9_unread[12];protected:CWorld&GetWorld()const;};
class CAIStateGroup_MeleeCombatNearbyBase:public CAIStateGroupBase{unsigned char pass9_unread[sizeof(Pass9CAIStateGroup_MeleeCombatNearbyBaseStorage)-sizeof(CAIStateGroupBase)];};
class CAIStateGroup_MeleeCombatNearby2:public CAIStateGroup_MeleeCombatNearbyBase{unsigned char pass9_unread[sizeof(Pass9CAIStateGroup_MeleeCombatNearby2Storage)-sizeof(CAIStateGroup_MeleeCombatNearbyBase)];
#ifdef PASS9_TEST
 friend int main();
#endif
 bool IsCombatSequenceActionPending()const;
};
bool CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending()const{
 if(PBrain->GetCreature().IsPerformingAnyAction())return true;
 CWorld&world=GetWorld();
 CThingAICreature*creature=&PBrain->GetCreature();
 CCombatManager*manager=&world.GetCombatManager();
 return manager->IsCreaturePerformingNonInterruptableSequence(*creature);
}
