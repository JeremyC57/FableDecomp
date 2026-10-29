// CGameScriptThing::MsgWhoHitMe -- query entity events within current script frame interval
// Leaf: MsgWhoHitMe@CGameScriptThingMethods
#include "engine/CScriptThing.h"
#include "fable_string.h"
#include "engine/CBaseObject.h"
struct ScriptMethods;
class C3DVector{ public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}};
#include "engine/CGameScriptThing.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CQuestManager.h"
#include "engine/CSpawnedFuncBase.h"
#define CThing CThingDonorLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish,*Capacity;const ComponentEntry* LowerBound(const ETCInterfaceType& key) const;};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; //004365B0
// Quarantined CThing donor has Components+48, retail has+44; lifecycle+91 confirmed by native.
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Metadata[0x41];unsigned char LifeFlags; ScriptMethods GetScriptThing()const;}; //004C7E50 const per PDB004C9A10
namespace std {template<class T>class allocator;template<class T,class A>class vector;}
typedef long EAIStateGroupTypeABI;typedef long EScriptAIPriorityABI;typedef long EHeroAbilityABI; // enum ABI declarations only
struct CBaseClassNative{CBaseClassNative();virtual ~CBaseClassNative();}; //0099A2D0 ctor,0099A2E0 dtor,VT01231710
class CCPPointerInfo{ public:unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void* Data;}; // actual PDB3field layout, no generated header
struct CountedScriptImp{CScriptThing* Data;CCPPointerInfo* Info;CountedScriptImp():Data(0),Info(0){}CountedScriptImp(const CountedScriptImp& other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;}~CountedScriptImp(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Data=0;Info=0;}};
struct ScriptMethods:CBaseClassNative{
 CountedScriptImp PImp;
 ScriptMethods(){}
 ScriptMethods(const ScriptMethods& other):CBaseClassNative(other),PImp(other.PImp){}
 virtual ~ScriptMethods(); //004AA840
 ScriptMethods& operator=(const ScriptMethods&); //008AB980
 CThingNative* GetThing() const {if(!PImp.Data)return 0;return reinterpret_cast<ScriptMethods*>(PImp.Data)->GetPThing();}
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



struct IntelligentInfo{CThingNative* Object;long References;};
struct CBaseIntelligentPointer{CBaseIntelligentPointer(); /* native00A01B10 */ void*VTable;CBaseObjectPointer* Info;CBaseObject* GetPItem()const;}; //00A01B50, PDB QBEPAVCBaseObject
struct PointerView:CBaseIntelligentPointer{CThingNative* Get()const{return reinterpret_cast<CThingNative*>(GetPItem());}};
// Retail CQuestManager+88 PQuestCurrentlyRunning and CScriptBase+2c PActiveThread differ from donor STL container sizes.
typedef CSpawnedFuncBase SpawnedFunc;
struct ScriptBaseNative{unsigned char Containers[0x2c];SpawnedFunc* PActiveThread;};
struct ActiveQuest{unsigned char State[8];ScriptBaseNative* Script;};
struct GameScriptThingManager{ScriptMethods GetNullScriptThing();}; //00704730, non-POD hidden result returned in caller slot
struct QuestManagerNative{void* State;GameScriptThingManager* ScriptThings;unsigned char OtherState[0x80];ActiveQuest* PQuestCurrentlyRunning;};
extern QuestManagerNative* QuestManager; //013B89FC
__forceinline long LastFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->LastScriptFrame:0;}
__forceinline long CurrentFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->CurrentScriptFrame:0;}
// Event kind2 CEventTalkedTo; originator intelligent pointer+8.
// Result CScriptThing is non-POD, size12, hidden caller destination passed to both callees; no explicit out-pointer prototype.
// Real event virtual order: destructor, GetType, OnAdd, GetLifeTime, IsReadyToDie.
// Native base0124F2C4 slots2=57A4FC(ret4),3=810E60,4=57A4FF(true).
namespace NEntityEvents {enum EEventType { HitThing=29,MoralityChanged=35,CutSceneAnimEvent=36 };}
struct EventBase { EventBase(); /* PDB01D93440 */ virtual ~EventBase(); virtual NEntityEvents::EEventType GetType()const=0; virtual void OnAdd(const CTCEntityEvents&); virtual long GetLifeTime()const; virtual bool IsReadyToDie()const; long WorldFrame; };
class CHitParameters;
struct HitEventBase:EventBase {HitEventBase(const CHitParameters&);virtual ~HitEventBase();virtual long GetLifeTime()const;virtual bool IsFlourish()const;CHitParameters* HitParams;void*HitParamsInfo;CThingNative* GetPHitBy()const;float GetBuildupTime()const;}; // PDB008078C0 ctor,00807010 virtual IsFlourish; native00810F30 GetBuildupTime
struct HitByEvent:HitEventBase {HitByEvent(const CHitParameters&);virtual ~HitByEvent();virtual NEntityEvents::EEventType GetType()const;}; // PDB00815E20 genuine const-reference ctor

struct HasHitByThing {HasHitByThing(){}};
struct CreatedAfter {long Frame;CreatedAfter(long f):Frame(f){}};
struct CreatedBeforeOrOn {long Frame;CreatedBeforeOrOn(long f):Frame(f){}};
struct Bind3 {const HasHitByThing* First;const CreatedAfter* Second;const CreatedBeforeOrOn* Third;Bind3(const HasHitByThing&a,const CreatedAfter&b,const CreatedBeforeOrOn&c):First(&a),Second(&b),Third(&c){}};
struct EntityEventsMethods:CTCEntityEvents { bool GetEvent(const HitByEvent**,const Bind3&)const; }; //008D4ED0, PDB0090BBE0
struct CGameScriptThingMethods:CGameScriptThing{ScriptMethods MsgWhoHitMe()const;};
ScriptMethods CGameScriptThingMethods::MsgWhoHitMe()const {
 const PointerView* p=reinterpret_cast<const PointerView*>(PThing);
 if(p->Get()&&!(p->Get()->LifeFlags&1)) {
 long last=LastFrame(QuestManager->PQuestCurrentlyRunning);
 long current=CurrentFrame(QuestManager->PQuestCurrentlyRunning);
 CThingNative* thing=p->Get();
 if(thing->UsingComponents[2]&4) {
 EntityEventsMethods* events;
 {ETCInterfaceType key = (ETCInterfaceType)(66);ComponentMap* map=&thing->Components;const ComponentEntry*it=map->LowerBound(key);const ComponentEntry*end=map->Finish;events=static_cast<EntityEventsMethods*>(((void*)(it==end||it->first>66?end:it)->second));}
 const HitByEvent* event;
 if(events->GetEvent(&event,Bind3(HasHitByThing(),CreatedAfter(last),CreatedBeforeOrOn(current))))return event->GetPHitBy()->GetScriptThing();

 }
 }
 return QuestManager->ScriptThings->GetNullScriptThing();
}