// CVtable012CDF28::Main -- acquire scripting ownership, disable hero pushing, and yield until termination
// Retail00D408B0,292 bytes. RegistrationD3C08B names SpeedFriend; factoryD407C0
// installs nativeVT012CDF28. PDB corroborates NScript::CQ_GuildTrainingScript::CSpeedFriend.
// Actual Thing, Game, Resource and BaseObject headers; named full-width PMFs for verified slots7/8.
// Leaf: Main
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
struct ScriptThingCopy:CScriptThing {ScriptThingCopy(const CScriptThing&);~ScriptThingCopy();};
namespace NScript {
struct CPredicateBase {virtual bool operator()()const=0;virtual CPredicateBase* Clone()const=0;virtual ~CPredicateBase(){}};
struct CIsThingConscious:CPredicateBase {
 ScriptThingCopy Thing;
 __forceinline CIsThingConscious(const CScriptThing& t):Thing(t){}
 virtual bool operator()()const;virtual CPredicateBase* Clone()const;
 ~CIsThingConscious(){} //inline native vptr reset then Thing destructor4AA840
};
}
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
extern const unsigned char ResourceVTable[]; //0127094C
// Generated resource header flattens CBaseObject. This field-free API binds the
// genuine CBaseObject constructor 0099A380 (vptr129A7D8, IntelligentPointer+4=0).
// It is not the distinct CBaseIntelligentPointer constructor 00A01B10.
struct ScriptResource:CScriptGameResourceObjectScriptedThingBase {
 void InitializeCBaseObject(); //0099A380, this points to flattened base at offset0
 __forceinline ScriptResource(){InitializeCBaseObject();__vftable=(void*)ResourceVTable;PImp_Data=0;PImp_Info=0;}
 ~ScriptResource(); //007E74D0
 bool IsScripting(); //00CD23B9
 void Release(); //00CD2770
};
#include "fable_string.h"
struct CThingNative;struct C3DVector{float X,Y,Z;};
// Restored genuine virtual ABI for by-value CScriptThing; all79 native slots named.
namespace std {template<class T>class allocator;template<class T,class A>class vector;}
typedef long EAIStateGroupTypeABI;typedef long EScriptAIPriorityABI;typedef long EHeroAbilityABI; // enum ABI declarations only
struct CBaseClassNative{CBaseClassNative(){}virtual ~CBaseClassNative();}; //0099A2D0 ctor,0099A2E0 dtor,VT01231710
struct CCPPointerInfo{unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void* Data;}; // actual PDB3field layout, no generated header
struct CountedScriptImp{CScriptThing* Data;CCPPointerInfo* Info;CountedScriptImp():Data(0),Info(0){}CountedScriptImp(const CountedScriptImp& other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;}~CountedScriptImp(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Data=0;Info=0;}};
struct ScriptMethods:CBaseClassNative{
 CountedScriptImp PImp;
 ScriptMethods(){}
 __forceinline ScriptMethods(const CScriptThing& other):CBaseClassNative(){PImp.Data=other.PImp_Data;PImp.Info=other.PImp_Info;if(PImp.Info)++PImp.Info->RefCount;}
 ScriptMethods(const ScriptMethods& other):CBaseClassNative(),PImp(other.PImp){}
 virtual ~ScriptMethods(); //004AA840, callee owns by-value argument destruction
virtual const CCharString& GetName() const; //slot1 0x004AA8B0
virtual CCharString GetDefName() const; //slot2 0x004AA8D0
virtual CCharString GetDataString() const; //slot3 0x004AA900
virtual void SetDataString(const CCharString&); //slot4 0x004AA930
virtual C3DVector GetFocalPos() const; //slot5 0x004AA940
virtual const C3DVector& GetPos() const; //slot6 0x004AA980
virtual C3DVector GetHomePos() const; //slot7 0x004AA9A0
virtual CCharString GetCurrentMapName() const; //slot8 0x004AA9E0
virtual CCharString GetHomeMapName() const; //slot9 0x004AAA10
virtual float GetAngleXY() const; //slot10 0x004AAA40
virtual CThingNative* GetPThing() const; //slot11 0x004AAA60
virtual unsigned __int64 GetPThingUniqueID() const; //slot12 0x004AAA70
virtual EAIStateGroupTypeABI GetCurrentStateGroupType() const; //slot13 0x004AAA90
virtual EScriptAIPriorityABI GetCurrentScriptPriority() const; //slot14 0x004AAAA0
virtual bool IsSneaking() const; //slot15 0x004AAAB0
virtual bool IsAwareOfHero() const; //slot16 0x004AAAC0
virtual bool MsgIsTriggeredBy(const CCharString&) const; //slot17 0x004AAAD0
virtual bool MsgIsKilledBy(const CCharString&) const; //slot18 0x004AAAF0
virtual ScriptMethods MsgWhoKilledMe() const; //slot19 0x004ABE60
virtual bool MsgIsUsedBy(const CCharString&) const; //slot20 0x004AAB10
virtual bool MsgIsHitBy(const CCharString&) const; //slot21 0x004AAB30
virtual bool MsgIsHitByWithFlourish(const CCharString&) const; //slot22 0x004AAB50
virtual bool MsgIsHitByWithDecapitate(const CCharString&) const; //slot23 0x004AAB70
virtual bool MsgIsHitByWithWeapon(const CCharString&,const CCharString&) const; //slot24 0x004AAB90
virtual bool MsgIsHitByWithProjectileWeapon(const CCharString&,float&) const; //slot25 0x004AABB0
virtual ScriptMethods MsgWhoHitMe() const; //slot26 0x004ABEC0
virtual bool MsgIsTalkedToBy(const CCharString&) const; //slot27 0x004AABD0
virtual ScriptMethods MsgWhoTalkedToMe() const; //slot28 0x004ABF00
virtual bool MsgExpressionPerformedTo(CCharString&) const; //slot29 0x004AABF0
virtual ScriptMethods MsgWhoExpressedToMe() const; //slot30 0x004ABF40
virtual long MsgHowLongWasExpressionPerformed() const; //slot31 0x004AAC10
virtual bool MsgIsKnockedOutBy(const CCharString&) const; //slot32 0x004AAC20
virtual ScriptMethods MsgWhoKnockedMeOut() const; //slot33 0x004ABF80
virtual bool MsgReceivedMoney(long&) const; //slot34 0x004AAC40
virtual bool MsgIsPresentedWithItem(CCharString&) const; //slot35 0x004AAC60
virtual bool MsgReceivedInventoryItem() const; //slot36 0x004AAC80
virtual bool MsgPerformedSpecialAbility(EHeroAbilityABI) const; //slot37 0x004AACA0
virtual bool MsgPerformedAnySpecialAbility() const; //slot38 0x004AACC0
virtual bool MsgPerformedAnyAggressiveSpecialAbility() const; //slot39 0x004AACE0
virtual bool MsgPerformedAnyNonAggressiveSpecialAbility() const; //slot40 0x004AAD00
virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbilityABI,const CCharString&) const; //slot41 0x004AAD20
virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const; //slot42 0x004AAD40
virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const; //slot43 0x004AAD60
virtual bool MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const; //slot44 0x004AAD80
virtual bool MsgOpenedChest() const; //slot45 0x004AADA0
virtual bool MsgHitEnemyWithMeleeWeapon() const; //slot46 0x004AADC0
virtual bool MsgHitFriendWithMeleeWeapon() const; //slot47 0x004AADE0
virtual bool MsgHitEnemyWithRangedWeapon() const; //slot48 0x004AAE00
virtual bool MsgHitFriendWithRangedWeapon() const; //slot49 0x004AAE20
virtual bool MsgHitEnemyWithBareHands() const; //slot50 0x004AAE40
virtual bool MsgHitFriendWithBareHands() const; //slot51 0x004AAE60
virtual bool MsgAttackedWithMeleeWeaponWithoutHittingAnything() const; //slot52 0x004AAE80
virtual bool MsgAttackedWithBareHandsWithoutHittingAnything() const; //slot53 0x004AAEA0
virtual bool MsgTalkedToAnyone() const; //slot54 0x004AAEC0
virtual bool MsgGetThingsKilled(std::vector<unsigned long,std::allocator<unsigned long> >&) const; //slot55 0x004AAEE0
virtual bool MsgPerformedFlourish() const; //slot56 0x004AAF00
virtual bool MsgPerformedSuccessfulFlourish() const; //slot57 0x004AAF20
virtual bool MsgOnMoralityChanged(long&) const; //slot58 0x004AAF40
virtual bool MsgOnCutSceneAnimEvent(CCharString&) const; //slot59 0x004AAF60
virtual bool MsgIsKicked() const; //slot60 0x004AAF80
virtual bool IsUnconscious() const; //slot61 0x004AAFA0
virtual bool IsUsable() const; //slot62 0x004AAFC0
virtual bool IsBeingCarriedBy(const CCharString&) const; //slot63 0x004AAFE0
virtual bool IsOpenDoor() const; //slot64 0x004AB000
virtual bool IsSummonedCreature() const; //slot65 0x004AB020
virtual void SetAsUsable(bool); //slot66 0x004AB040
virtual void SetFriendsWithEverythingFlag(bool); //slot67 0x004AB060
virtual bool GetActivationTriggerStatus() const; //slot68 0x004AB080
virtual void SetActivationTriggerStatus(bool); //slot69 0x004AB0A0
virtual void SetToKillOnLevelUnload(bool); //slot70 0x004AB0C0
virtual void UpdateThingAttachment(); //slot71 0x004AB0E0
virtual void IncrementScriptCounter(); //slot72 0x004AB0F0
virtual void DecrementScriptCounter(); //slot73 0x004AB100
virtual long GetScriptCounter(); //slot74 0x004AB110
virtual bool IsAlive() const; //slot75 0x004AB130
virtual bool IsDead() const; //slot76 0x004AA890
virtual bool IsNull() const; //slot77 0x004AB150
virtual bool IsEqualTo(const ScriptMethods&) const; //slot78 0x004AA8A0
};
FABLE_STATIC_ASSERT(sizeof(ScriptMethods)==sizeof(CScriptThing));
FABLE_STATIC_ASSERT(offsetof(ScriptMethods,PImp)==offsetof(CScriptThing,PImp_Data));
FABLE_STATIC_ASSERT(offsetof(ScriptMethods,PImp)+offsetof(CountedScriptImp,Info)==offsetof(CScriptThing,PImp_Info));


struct GameCalls;
#pragma pointers_to_members(full_generality,single_inheritance)
union GameSlot {void* Address;void(GameCalls::*Frame)()const;bool(GameCalls::*Start)(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase&,int);void(GameCalls::*SetPushable)(ScriptMethods,bool);};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);
#endif
struct GameCalls:CGameScriptInterface {
 __forceinline void SetIsPushableByHero(const CScriptThing& thing,bool pushable){(this->*reinterpret_cast<const GameSlot*>(__vftable)[844].SetPushable)(ScriptMethods(thing),pushable);}
 void NewScriptFrame()const{(this->*reinterpret_cast<const GameSlot*>(__vftable)[7].Frame)();}
 bool StartScriptingEntity(const CScriptThing& t,ScriptResource& r,int priority){return(this->*reinterpret_cast<const GameSlot*>(__vftable)[8].Start)(t,r,priority);}
};
// Parent flag meaning remains inferred/unknown; only its native offset is asserted.
struct NativeParent {unsigned char UnrecoveredPrefix[0x4c];bool ScriptFlag;};
struct CVtable012CDF28 {void* VTable;GameCalls* Game;CScriptThing Me;NativeParent* Parent;void* Master;
 void SetPredicate(const NScript::CPredicateBase&); //00F35B10
 bool IsActiveThreadTerminating(); //00F35B30
 void Main();
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CVtable012CDF28,Parent)==0x14);
FABLE_STATIC_ASSERT(offsetof(NativeParent,ScriptFlag)==0x4c);
#endif
void CVtable012CDF28::Main(){
 {NScript::CIsThingConscious p(Me);SetPredicate(p);}
 Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 ScriptResource resource;
 if(resource.IsScripting())resource.Release();
 while(!Game->StartScriptingEntity(Me,resource,4)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
 if(IsActiveThreadTerminating())return;
 (Game->*reinterpret_cast<const GameSlot*>(Game->__vftable)[844].SetPushable)(*reinterpret_cast<const ScriptMethods*>(&Me),false);
 do {Game->NewScriptFrame();}while(!IsActiveThreadTerminating());
}