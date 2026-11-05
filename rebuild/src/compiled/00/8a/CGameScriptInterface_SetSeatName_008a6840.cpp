// CGameScriptInterface::SetSeatName(CScriptThing,CCharString) -- set scratchpad SeatName
// Leaf: ?SetSeatName@CGameScriptInterface@@UAEXVCScriptThing@@VCCharString@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 31 (family csm): the CCharString parameter is the header's class with the PDB copy ctor ??0CCharString@@QAE@ABV0@@Z 0x0099EC30 (family fs;
//   was the derived NativeCharString view), the ScriptMethods projection is engine/CScriptThing.h's retail class (virtual ~CScriptThing,
//   ??1CScriptThing@@UAE@XZ 0x004AA840; copy / dtor / GetPThing expanded inline as before), CThingNative is CThing, and the own row is the
//   retail virtual CGameScriptInterface member (was the invented CGameScriptInterfaceMethods; the landed sibling 0x008A1E50 is the model);
//   the scratchpad setter is the PDB CTCAIScratchpad::SetSeatName(CCharString) by value (FableWin.pdb), expanded inline
// Retail008A6840,259 bytes;0040F020 lower_bound,0099A2E0 base destructor,00BFE9BC delete; CTCAIScratchpad donor layout.
#include "fable_string.h"
#include "engine/C3DVector.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const ETCInterfaceType& key);};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; //0040F020
class CThing{ public:void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;}; //004C73D0,004C7E50
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
 float TargetingDistanceOffset;const CAIScratchpadDef* PDef;void SetSeatName(CCharString text){SeatName=text;}
};
FABLE_STATIC_ASSERT(sizeof(CTCAIScratchpadNative)==132);
FABLE_STATIC_ASSERT(offsetof(CTCAIScratchpadNative,Action)==72);
FABLE_STATIC_ASSERT(offsetof(CTCAIScratchpadNative,PDef)==128);
__forceinline CTCAIScratchpadNative* GetScratchpad(CThing* thing){
 CTCAIScratchpadNative* result;
 if(thing->UsingComponents[5]&0x400){ComponentMap* map=&thing->Components;ComponentEntry* it=map->LowerBound((ETCInterfaceType)170);ComponentEntry* end=map->Finish;result=static_cast<CTCAIScratchpadNative*>(((void*)(it==end||it->first>170?end:it)->second));}
 return result; // native requires the scratchpad component; no absent-component fallback is initialized.
}
void CGameScriptInterface::SetSeatName(CScriptThing script,CCharString text){if(!script.GetPThing())return;CTCAIScratchpadNative* tc=GetScratchpad(script.GetPThing());tc->SetSeatName(text);}