// 00911300 CAIStateGroup_MinionGuardReactToBodyFoundMessage::Update
// Grade: DIFFER(658v666) -- probe g (bodyPos default-constructed then assigned) cuts the diff 125->73: early xor cl,cl vs retail xor al,al after the dead flags-temp store; implicit operator= copies via lea edx (retail: X,Y,Z loads into ecx/edx/eax); GetCreature chain register ecx vs edx; merged Terminate+~body+ret block kept after the retry loop (retail keeps it after the final OrderSecuritySweep test, success block last)
// Next step: find the retail CMessageEvent::GetPOriginator<CThing> inline in FableWin (02112220) to get the exact type test; try the copy through a CMessageEvent::GetPos() const& accessor; work the cross-jump keeper (retail keeps the LAST fail copy)
// Leaf: ?Update@CAIStateGroup_MinionGuardReactToBodyFoundMessage@@UAEXXZ
// Flags:
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define CClass CClassFields
#include "engine/CClass.h"
#undef CClass
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingPhysical CThingPhysicalFields
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectFields
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBaseFields
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingAICreature CThingAICreatureFields
#include "engine/CThingAICreature.h"
#undef CThingAICreature
#define CAIBrain CAIBrainFields
#include "engine/CAIBrain.h"
#undef CAIBrain
#define CAIStateGroupBase CAIStateGroupBaseFields
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
class CCharString;class C3DVector;class C2DVector;class CThing;class CThingPhysical;class CThingCreatureBase;class CThingAICreature;class CThingPlayerCreature;class CThingBuilding;class CTCBase;class CMaterialDef;class CEnvironment;class CEngineGraphic;class CInventoryItem;class CThingDef;class CThingProperties;class CInterpolationInfo;class CEnginePrimitivePrepareInfo;class CPhysicsObject;class CTCParamsBase;class CThingLevelScript;class CThingGameState;class CPhysicsObjectDamage;class CCollisionInfo;class CThingDrawInfo;class CThingDebugDrawInfo;class CThingPhysicalProperties;class CSoundPair;class CAnimationEntry;
template<class T>class CDefPointer;template<class T>class CArray;namespace std{template<class T>class allocator;template<class T,class A>class vector;}
class C3DVector;
class CCharString;
class CCreatureAbilityDef;
class CEditEngineDrawInfo;
class CEngineGraphic;
class CEnginePrimitivePrepareInfo;
class CHitParameters;
class CInterpolationInfo;
class CMaterialDef;
class CMemoryDataInputStream;
class CPersistContext;
class CSoundPair;
class CThing;
class CThingBaseDef;
class CThingCreatureBase;
class CThingDrawInformation;
class CThingInfo;
class CThingPhysical;
class CThingPhysicalDebugInfo;
enum ECollideType{};
class C3DVector{public:float X,Y,Z;C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}C3DVector(){}};
// Layout: native CClass17-byte flattened header, rounded20 when used as a polymorphic base.
class CClass:public CBaseObject{public:virtual void Save(CCharString&);private:unsigned char _pad_class[sizeof(CClassFields)-sizeof(CBaseObject)];};
class CThing:public CClass{public:class CSerialiseInfo;
public:virtual bool SerialiseGameState(CPersistContext&);
public:virtual bool ConstructFromLevelScript(const char*);
public:virtual bool ConstructAlreadyExistingThingFromLevelScript(const char*);
public:virtual bool ConstructFromGameStateScript(const char*);
public:virtual void CopyGameState(const CThing&);
public:virtual bool OnCreate();
public:virtual void InitialActivate();
public:virtual void OnInitialActivate();
protected:virtual void OnDie();
protected:virtual void OnKill();
protected:virtual bool PreOnKill();
protected:virtual void OnSetInLimbo(bool);
protected:virtual void CreateFromDef(const CThingBaseDef&);
public:virtual void ConstructFromBinarySave(CMemoryDataInputStream&,const CThing::CSerialiseInfo&);
public:virtual bool IsToBePlacedInMap() const;
public:virtual bool ValidateThing(CCharString&) const;
public:virtual bool WorldUpdate();
public:virtual void WorldUpdateBulletTime();
public:virtual bool RenderUpdate(const CInterpolationInfo*);
public:virtual bool IsEntitySerialisable() const;
public:virtual unsigned long GetChecksum() const;
public:virtual void GetInfo(CThingInfo*) const;
public:virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const;
public:virtual void DrawUpdate(const CInterpolationInfo*);
public:virtual bool DrawIsDrawable() const;
public:virtual void SetAsDrawable(bool);
public:virtual bool DrawIsEditDrawable() const;
public:virtual bool DrawIsInDrawList() const;
public:virtual unsigned long DrawGetPhysicsMesh() const;
public:virtual bool DrawIsShowingFacingDirection() const;
public:virtual bool DrawIsDisplayingInformation() const;
public:virtual bool DrawGetThingDisplayInformation(CThingDrawInformation*) const;
public:virtual bool DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo&) const;
public:virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
public:virtual void SetThingStandingOn(CThing*);
public:virtual CThing* GetThingStandingOn();
public:virtual const CThing* GetThingStandingOn() const;
public:virtual long GetFloorMaterial() const;
public:virtual void OnCollision(ECollideType,const C3DVector&,const C3DVector&,CThing*);
public:virtual void OnNonKillingLevelUnload();
public:virtual void OnLevelUnload();
public:virtual bool IsFrameUpdated() const;
public:virtual void SetPositionUsingBrain();
public:const C3DVector&GetPos()const;bool IsDead()const{return (reinterpret_cast<const unsigned char*>(this)[0x91]&1)!=0;}unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingFields*>(this)->ThingTypeFlags;}static unsigned long __fastcall StatGetTypeFlags(){return 0;}
private:unsigned char _pad_members[sizeof(CThingFields)-sizeof(CClass)];
};
class CThingPhysical:public CThing{
public:virtual bool IsSolidToThing(const CThingPhysical*) const;
public:virtual bool IsAffectedByDamage() const;
public:virtual unsigned long GetProperties() const;
public:virtual bool IsCrop() const;
public:virtual bool IsHidden() const;
public:virtual bool IsTargetedByPlayer(long) const;
public:virtual void SetTargetedByPlayer(long);
public:virtual void ClearTargetedByPlayer(long);
public:virtual bool IsFragile() const;
public:virtual bool CanBlockNavigatingCreature() const;
public:virtual bool CanBlockUpperLevelNavigatingCreature() const;
public:virtual bool UseHighDetailQuadTree() const;
public:virtual CDefPointer<CMaterialDef const > PeekPMaterial() const;
public:virtual void ModifyHealth(float,bool);
private:unsigned char _pad_members[sizeof(CThingPhysicalFields)-sizeof(CThing)];
};
class CThingGameObject:public CThingPhysical{
public:virtual bool DrawIsTargeted() const;
public:virtual bool IsUsingTriggerRadius() const;
public:virtual float GetTriggerRadius() const;
public:virtual void SetTriggerRadius(float);
public:virtual bool IsCover() const;
public:virtual void OnHit(CHitParameters&);
public:virtual bool IsGameObject() const;
public:virtual bool IsIndestructible() const;
public:virtual void ForceAnim(long,bool);
public:virtual void ForceAnim(const CCharString&,bool);
public:virtual long GetMaterialOnHitByMelee() const;
private:unsigned char _pad_members[sizeof(CThingGameObjectFields)-sizeof(CThingPhysical)];
};
class CThingCreatureBase:public CThingGameObject{
protected:virtual unsigned long GetPhonemeAnim(const CCharString&) const;
protected:virtual bool SetActionForMelee(const CCreatureAbilityDef&);
public:virtual bool AnimExists(const CCharString&,bool) const;
public:virtual bool IsNewAttackerGreaterThreat(const CThingCreatureBase&,const CThingCreatureBase&) const;
public:virtual bool IsWillable() const;
public:virtual bool IsControlled() const;
public:virtual bool IsPlayerCharacter() const;
public:virtual float GetMaxPushingSpeed(const C3DVector&) const;
public:virtual float GetMaxWalkingSpeed(const C3DVector&) const;
public:virtual float GetMaxJoggingSpeed(const C3DVector&) const;
public:virtual float GetMaxRunningSpeed(const C3DVector&) const;
public:virtual float GetMaxSprintingSpeed(const C3DVector&) const;
public:virtual float GetMaxFlyingSpeed(const C3DVector&) const;
public:virtual float GetMaxHoverSpeedSlow(const C3DVector&) const;
public:virtual float GetMaxHoverSpeedMedium(const C3DVector&) const;
public:virtual float GetMaxHoverSpeedFast(const C3DVector&) const;
public:virtual float GetMaxAcceleration() const;
public:virtual bool IsToUnsheatheItemsAfterCutscenes() const;
public:virtual CSoundPair GetSoundPair(const CCharString&) const;
private:unsigned char _pad_members[sizeof(CThingCreatureBaseFields)-sizeof(CThingGameObject)];
};
class CThingAICreature:public CThingCreatureBase{
protected:virtual const CEngineGraphic* GetCurrentGraphic() const;
public:virtual bool FeelsGravity() const;
public:virtual float GetFOV() const;
private:unsigned char _pad_members[sizeof(CThingAICreatureFields)-sizeof(CThingCreatureBase)];
};
class CBaseIntelligentPointer{CBaseObjectPointer*PData;public:CBaseIntelligentPointer();virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;void SetPItem(CBaseObject*);};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:__forceinline CIntelligentPointer(T*p){SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(p)));}CIntelligentPointer(const CIntelligentPointer&p){SetPItem(p.GetPItem());}T*Get()const{return static_cast<T*>(GetPItem());}T*operator->()const{return Get();}};
// Layout: original CMessageEvent Pos atc and private intelligent originator at48; unused fields retain their original extent.
class CMessageEvent:public CBaseObject{public:unsigned char Type[4];C3DVector Pos;private:unsigned char _pad_message[0x48-0x18];CIntelligentPointer<CThing>POriginator;public:template<class T>T*GetPOriginator()const{T*result=0;if(POriginator.Get()){unsigned long mask=T::StatGetTypeFlags();const unsigned long&flags=POriginator.Get()->GetTypeFlags();if((flags&mask)==mask)result=static_cast<T*>(POriginator.Get());}return result;}};
class CMessageEventManager{public:void RemoveMessage(const CMessageEvent*);};
class CWorld:public CWorldFields{public:CMessageEventManager&GetMessageEventManager(){return *PMessageEventManager_Data;}};
// Layout: actual CCreatureActionBase derives CBaseObject, shared native extent a8; derived actions add no fields.
class CCreatureActionBase:public CBaseObject{unsigned char _pad_action[sizeof(CCreatureActionBaseFields)-sizeof(CBaseObject)];public:virtual~CCreatureActionBase();};
class CCreatureAction_Curiosity:public CCreatureActionBase{public:CCreatureAction_Curiosity(CThingCreatureBase&);};
class CCreatureAction_OrderSecuritySweep:public CCreatureActionBase{public:CCreatureAction_OrderSecuritySweep(CThingCreatureBase&);};
class CAIBrain;class CWorld;
enum EScriptAIPriority{};enum EAIStateGroupType{};enum EAIActionReturn{};enum EAIActionRepeat{};enum EMovementType{};
class CAIStateGroupBase:public CBaseClass{public:virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);virtual void OnActivate();virtual bool InitCreaturePosition();virtual bool AllowPriorityOverlap();virtual bool IsStillValid()const;virtual const CCharString*GetDebugText()const;virtual bool IsTimingCritical()const;virtual EAIStateGroupType GetStateGroupType()const;
protected:virtual void Cleanup();virtual void OnReplace();virtual bool IsRunnable();virtual void Update()=0;virtual void InitStateGroup(const CWorld&,CThingAICreature&);CAIBrain*PBrain;unsigned char _pad_group[sizeof(CAIStateGroupBaseFields)-8];CThingAICreature&GetCreature()const{return *reinterpret_cast<const CAIBrainFields*>(PBrain)->Creature;}CWorld&GetWorld()const;
EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase&,bool);EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat,CIntelligentPointer<const CThing>,float,bool&,EMovementType,bool,bool,bool,bool,float);EAIActionReturn AIAction_LookAtThing(EAIActionRepeat,CIntelligentPointer<const CThing>,long,float);EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat,const C3DVector,float,bool&,EMovementType,bool,bool,bool,const CThing*,bool,float);
public:void Terminate();};
class CAIStateGroup_MinionGuardReactToBodyFoundMessage:public CAIStateGroupBase{const CMessageEvent*FindBodyFoundEvent();public:virtual void Update();};
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector&,const C3DVector&,float);
void CAIStateGroup_MinionGuardReactToBodyFoundMessage::Update()
{
    const CMessageEvent* message = FindBodyFoundEvent();
    if (message) {
        CIntelligentPointer<const CThing> body(message->GetPOriginator<CThing>());
        if ((!body.Get() || body.Get()->IsDead())) {
            Terminate();
            return;
        }
        C3DVector bodyPos; bodyPos = message->Pos;
        CMessageEventManager* manager = &GetWorld().GetMessageEventManager();
        manager->RemoveMessage(message);
        if (!GFWorld3DDistanceSmallerThan(GetCreature().GetPos(), body.Get()->GetPos(), 2.5f)) {
            if (AIAction_PerformActionAndWait(CCreatureAction_Curiosity(GetCreature()), false) != 2) {
                Terminate();
                return;
            }
            bool arrived = false;
            EAIActionReturn result;
            do {
                if ((!body.Get() || body.Get()->IsDead())) {
                    Terminate();
                    return;
                }
                result = AIAction_MoveToThingProximity((EAIActionRepeat)2, body, 2.5f, arrived, (EMovementType)5, true, false, true, true, 0.0f);
            } while (result == 4);
            if (result != 2) {
                Terminate();
                return;
            }
        }
        if ((body.Get() && !body.Get()->IsDead())) {
            if (AIAction_LookAtThing((EAIActionRepeat)1, body, 0xf, GetCreature().GetFOV() * 0.0027777778f) == 2) {
                bool arrived = false;
                if (AIAction_MoveToPosProximity((EAIActionRepeat)1, bodyPos, 1.5f, arrived, (EMovementType)5, true, false, true, 0, true, 0.0f) != 2) {
                    Terminate();
                    return;
                }
                if (AIAction_PerformActionAndWait(CCreatureAction_OrderSecuritySweep(GetCreature()), false) != 2) {
                    Terminate();
                    return;
                }
            }
        }
    }
    Terminate();
}
