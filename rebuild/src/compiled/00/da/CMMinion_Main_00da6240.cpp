// CM_Minion::Main() -- spawn the encounter creature and manage script lifecycle.
// Native script projection: actual flattened headers included; PDB names/layouts preserved.
// All declared virtuals are native signatures and ordering, including reversed overloads.
#define CScriptThing CScriptThingDonor
#include "engine/CScriptThing.h"
#undef CScriptThing
#define CCharString CCharStringDonor
#include "fable_string.h"
#undef CCharString
// Actual flattened donor headers included; genuine virtual interfaces follow.
// CCharString donor omits the native owning copy constructor, restored below.
class CThing; class CRGBColour;
typedef long EAIStateGroupTypeABI;typedef long EScriptAIPriorityABI;
namespace std {template<class T>class allocator;template<class T,class A>class list;template<class T,class A>class vector;template<class T,class U>struct pair;}
enum EScriptAIPriority { ScriptPriorityUnspecified=0 };
enum EClothingCoversArea { ClothingAreaUnspecified=0 };
enum EHeroAbility { HeroAbilityUnspecified=0 };
#ifdef _MSC_VER
typedef unsigned __int64 ScriptUID;
#else
typedef unsigned long long ScriptUID;
#endif
class C3DVector { public:float X,Y,Z;};
class CScriptGameResourceObjectScriptedThingBase;
class CCharString { public:char* Data; CCharString(const char*,long=-1); CCharString(const CCharString&); ~CCharString();}; // 0099EBF0/0099EAE0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo { public:unsigned long References;void (__fastcall *Delete)(void*);void* Data;};
void operator delete(void*);
class CScriptThing : public CBaseClass { public:
 CScriptThing* PImp_Data; CCPPointerInfo* PImp_Info;
 CScriptThing();
 CScriptThing(const CScriptThing&); //004ABE90 owning-copy constructor
 virtual CCharString const& GetName()const; // slot1 004AA8B0
 virtual CCharString GetDefName()const; // slot2 004AA8D0
 virtual CCharString GetDataString()const; // slot3 004AA900
 virtual void SetDataString(CCharString const& s){if(PImp_Data)PImp_Data->SetDataString(s);} // slot4 004AA930
 virtual C3DVector GetFocalPos()const; // slot5 004AA940
 virtual C3DVector const& GetPos()const; // slot6 004AA980
virtual C3DVector GetHomePos() const; //slot7 0x004AA9A0
virtual CCharString GetCurrentMapName() const; //slot8 0x004AA9E0
virtual CCharString GetHomeMapName() const; //slot9 0x004AAA10
virtual float GetAngleXY() const; //slot10 0x004AAA40
virtual CThing* GetPThing() const; //slot11 0x004AAA60
virtual ScriptUID GetPThingUniqueID() const; //slot12 0x004AAA70
virtual EAIStateGroupTypeABI GetCurrentStateGroupType() const; //slot13 0x004AAA90
virtual EScriptAIPriorityABI GetCurrentScriptPriority() const; //slot14 0x004AAAA0
virtual bool IsSneaking() const; //slot15 0x004AAAB0
virtual bool IsAwareOfHero() const; //slot16 0x004AAAC0
virtual bool MsgIsTriggeredBy(const CCharString&) const; //slot17 0x004AAAD0
virtual bool MsgIsKilledBy(const CCharString&) const; //slot18 0x004AAAF0
virtual CScriptThing MsgWhoKilledMe() const; //slot19 0x004ABE60
virtual bool MsgIsUsedBy(const CCharString&) const; //slot20 0x004AAB10
virtual bool MsgIsHitBy(const CCharString&) const; //slot21 0x004AAB30
virtual bool MsgIsHitByWithFlourish(const CCharString&) const; //slot22 0x004AAB50
virtual bool MsgIsHitByWithDecapitate(const CCharString&) const; //slot23 0x004AAB70
virtual bool MsgIsHitByWithWeapon(const CCharString&,const CCharString&) const; //slot24 0x004AAB90
virtual bool MsgIsHitByWithProjectileWeapon(const CCharString&,float&) const; //slot25 0x004AABB0
virtual CScriptThing MsgWhoHitMe() const; //slot26 0x004ABEC0
virtual bool MsgIsTalkedToBy(const CCharString&) const; //slot27 0x004AABD0
virtual CScriptThing MsgWhoTalkedToMe() const; //slot28 0x004ABF00
virtual bool MsgExpressionPerformedTo(CCharString&) const; //slot29 0x004AABF0
virtual CScriptThing MsgWhoExpressedToMe() const; //slot30 0x004ABF40
virtual long MsgHowLongWasExpressionPerformed() const; //slot31 0x004AAC10
virtual bool MsgIsKnockedOutBy(const CCharString&) const; //slot32 0x004AAC20
virtual CScriptThing MsgWhoKnockedMeOut() const; //slot33 0x004ABF80
virtual bool MsgReceivedMoney(long&) const; //slot34 0x004AAC40
virtual bool MsgIsPresentedWithItem(CCharString&) const; //slot35 0x004AAC60
virtual bool MsgReceivedInventoryItem() const; //slot36 0x004AAC80
virtual bool MsgPerformedSpecialAbility(EHeroAbility) const; //slot37 0x004AACA0
virtual bool MsgPerformedAnySpecialAbility() const; //slot38 0x004AACC0
virtual bool MsgPerformedAnyAggressiveSpecialAbility() const; //slot39 0x004AACE0
virtual bool MsgPerformedAnyNonAggressiveSpecialAbility() const; //slot40 0x004AAD00
virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility,const CCharString&) const; //slot41 0x004AAD20
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
virtual bool IsEqualTo(const CScriptThing&) const; //slot78 0x004AA8A0
 virtual ~CScriptThing(); //004AA840 native out-of-line destructor
};
// Native CGameScriptInterface virtual prefix: all100 entries use recovered PDB
// signatures and native raw VT01260F0C order. No fabricated padding slots.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase; class CPersistContext;
class CEntityScriptBase { public:
 virtual ~CEntityScriptBase();
 virtual void Main();
 virtual void Init();
 virtual CScriptBase* GetParentScript()const;
 virtual void OnPersist(CPersistContext&);
 virtual void OnPredicateFail();
 virtual void OnInterrupted();
 CGameScriptInterface* Game;
 CScriptThing Me;
 bool IsActiveThreadTerminating()const; // 00F35B30: thread termination only
};
struct CM_Minion : CEntityScriptBase { void* ParentClass; void* PMasterData; virtual void Main(); };
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CScriptThing)==sizeof(CScriptThingDonor));
FABLE_STATIC_ASSERT(sizeof(CEntityScriptBase)==20);
FABLE_STATIC_ASSERT(offsetof(CEntityScriptBase,Me)==8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CM_Minion)==28);
FABLE_STATIC_ASSERT(sizeof(void (CM_Minion::*)())==4);
#endif
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float); //00CBE2FF
void CM_Minion::Main(){
 while(!IsDistanceBetweenThingsUnder(Me,Game->GetHero(),20.0f)){
  Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 }
 if(!IsActiveThreadTerminating()){
  { CCharString name("MinionAttackingGuard"); CCharString definition("CREATURE_MINION_WARDOG");
    Game->CreateCreature(definition,Me.GetPos(),name,true); }
  Game->RemoveThing(Me,false,true);
 }
}