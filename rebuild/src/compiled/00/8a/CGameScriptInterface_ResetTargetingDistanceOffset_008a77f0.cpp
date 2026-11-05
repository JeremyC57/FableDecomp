// CGameScriptInterface::ResetTargetingDistanceOffset(CScriptThing) -- restore scratchpad string from its definition
// Leaf: ?ResetTargetingDistanceOffset@CGameScriptInterface@@UAEXVCScriptThing@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 30 (family csm): the ScriptMethods projection (??1ScriptMethods@@UAE@XZ bound to 0x004AA840) is engine/CScriptThing.h's retail class
//   (virtual ~CScriptThing, Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ; copy / dtor / GetPThing slot 11 expanded inline as before),
//   CThingNative is the CThing layout, and the own row is the retail virtual CGameScriptInterface member (was the invented
//   CGameScriptInterfaceMethods; the landed sibling 0x008A1E50 is the model)
// Retail 008a77f0, 229 bytes. CTCAIScratchpad type170. Callees0040F020 lower_bound,0099EFB0 stringassignment,0099A2E0 base destructor,00BFE9BC scalar delete.
#include "fable_string.h"
// Donor CThing is quarantined: donor Components+48/lifecycle groups+a1..a4 differ from retail Components+44/LifeFlags+91.
// Include its actual declaration for provenance; the portable native projection below retains retail field boundaries.
#define CThing CThingDonorLayout
#include "engine/_quarantine/CThing.h"
#undef CThing

#include "engine/C3DVector.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const ETCInterfaceType& key);};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; //0040F020
class CThing{ public:void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;}; //004C73D0,004C7E50
FABLE_STATIC_ASSERT(offsetof(CThing,UsingComponents)==0x20);
FABLE_STATIC_ASSERT(offsetof(CThing,Components)==0x44);
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                         // : CBaseClass, vtable 0x01238C8C; virtual ~CScriptThing 0x004AA840
// Retail expands these at this row's sites (the CScriptThing projection had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline CThing* CScriptThing::GetPThing() const { if (!PImp.Data) return 0; return PImp.Data->GetPThing(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#include "engine/CGameScriptInterface.h"

#include "engine/CAIScratchpadDef.h"
#include "engine/CTCBase.h"
// No generated CTCAIScratchpad header. Complete donor PDB fields (132bytes), native offsets identical.
struct CTCAIScratchpadNative:CTCBase {
 unsigned long ThankingPhrase,IgnoringPhrase; C3DVector WanderCentrePoint;
 float WanderMinDistance,WanderMaxDistance;long GossipCounter,MaxGossipPhrase;
 unsigned long WarningPhrase,BeerRequestPhrase;long ScriptingStateGroup;
 float MaxHeroReactionDistance;long ActionFrequency;float ActionFrequencyVariation;
 CCharString Action;bool FaceHeroForAction;CCharString TargetName;float FollowDistance;
 bool AttackHeroOnSight;long TimeToSpendHarassingHero;
 float CombatNearbyEnemyFleeingBreakOffRange,CombatNearbyBreakOffRange;
 bool StealStealableItems,RecoverStealableItems,TakeStealableItemToRandomDestination,KillSelfAndStealableItemAfterReachingDestination,AllowedToFollow;
 CCharString TableName,SeatName;bool DisableHeadLooking,IsPushableByHero,LookForFiniteTime,AvoidRegionExits;
 float TargetingDistanceOffset;const CAIScratchpadDef* PDef;
};
FABLE_STATIC_ASSERT(sizeof(CTCAIScratchpadNative)==132);
FABLE_STATIC_ASSERT(offsetof(CTCAIScratchpadNative,Action)==72);
FABLE_STATIC_ASSERT(offsetof(CTCAIScratchpadNative,PDef)==128);
__forceinline CTCAIScratchpadNative* GetScratchpad(CThing* thing){
 CTCAIScratchpadNative* result;
 if(thing->UsingComponents[5]&0x400){ComponentMap* map=&thing->Components;ComponentEntry* it=map->LowerBound((ETCInterfaceType)170);ComponentEntry* end=map->Finish;result=static_cast<CTCAIScratchpadNative*>(((void*)(it==end||it->first>170?end:it)->second));}
 return result; // native requires the scratchpad component; no absent-component fallback is initialized.
}

void CGameScriptInterface::ResetTargetingDistanceOffset(CScriptThing script){if(!script.GetPThing())return;CTCAIScratchpadNative* tc=GetScratchpad(script.GetPThing());tc->TargetingDistanceOffset=tc->PDef->TargetingDistanceOffset;}