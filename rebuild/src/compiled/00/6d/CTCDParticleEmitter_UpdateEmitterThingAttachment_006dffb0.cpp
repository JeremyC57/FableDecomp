// CTCDParticleEmitter::UpdateEmitterThingAttachment(bool interpolate, float time) -- move the attached emitter to its thing (position and orientation from the attached thing physics or the emitter)
// Leaf: ?UpdateEmitterThingAttachment@CTCDParticleEmitter@@QAEX_NM@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags:
// Shape: the orientation is default-constructed and then assigned in an if/else on the emitter, each arm a
//   (flags & 0x20) ? physics RHSet : (emitter orientation | creation RHSet) ternary. The two physics arms then keep
//   different vtable registers (edx / eax) and stay separate, as in retail; one nested ternary initialiser lets VC7.1
//   tail-merge them (613 v 633).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCPhysicsBase CTCPhysicsBaseFields
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CTCGraphicAppearance.h"
#undef CTCGraphicAppearance
#define CWorldMap CWorldMapFields
#include "engine/CWorldMap.h"
#undef CWorldMap
#define CParticleEmitter CParticleEmitterFields
#include "engine/CParticleEmitter.h"
#undef CParticleEmitter
#include "engine/C3DVector.h"
class CRightHandedSet{protected:C3DVector Up,Forward;public:CRightHandedSet(){}CRightHandedSet(const CRightHandedSet&r):Up(r.Up),Forward(r.Forward){}};
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;};
class C3DPositionInterpolator:public CBaseClass{public:C3DVector StartPosition,EndPosition;C3DPositionInterpolator(const C3DVector&,const C3DVector&);};
class C3DOrientationInterpolator:public CBaseClass{public:CRightHandedSet StartOrientation,EndOrientation;C3DOrientationInterpolator(const CRightHandedSet&,const CRightHandedSet&);};
class CBaseObject;class CBaseObjectPointer;
class CBaseIntelligentPointer{private:CBaseObjectPointer*PData;public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T*GetPItem()const{return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem());}};
class CWorldMap:public CWorldMapFields{public:float GetWaterSizeZAt(const C3DVector&)const;};
enum ETCInterfaceType{};class C2DVector;class CTCParamsBase;class CEnginePrimitivePrepareInfo;class CPersistContext;namespace NTCMessages{class CMessageBase;}
// Layout: shared CTCBase prefix; native table omits donor editor-only property transfer pair.
class CTCBase:public CBaseClassNonCopyable{protected:const CWorldMap&PeekWorldMap()const;CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;
protected:virtual void OnSerialise(CPersistContext&,bool);
public:virtual void OnRemove();
public:virtual void ConstructFromParams(const CTCParamsBase&);
public:virtual void OnCreate();
public:virtual void InitialActivate();
public:virtual void OnInitialActivate();
public:virtual void RenderUpdate(float);
public:virtual bool HasFrameUpdate()const;
public:virtual void FrameUpdate();
public:virtual void BulletTimeFrameUpdate();
public:virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
public:virtual void Copy(const CTCBase&);
public:virtual void OnDie();
public:virtual void OnKill();
public:virtual void OnLevelUnload();
public:virtual void OnSetInLimbo(bool);
public:virtual bool Validate(CCharString&)const;
public:virtual bool IsFreeForDeletion()const;
public:virtual bool IsSerialiseCreatable()const;
public:virtual bool IsSerialisableForHeroState()const;
public:virtual ETCInterfaceType GetInterfaceType()const;
public:virtual CCharString GetClassTypeName()const;
public:virtual bool IsComponentSerialisable()const;
public:virtual bool IsDriver()const;
public:virtual bool IsCopyable()const;
public:virtual void GlobalUpdate(bool);
public:virtual void HandleMessage(const NTCMessages::CMessageBase&);
};
// Layout: shared physics base64, actual virtual prefix and unread member storage after12.
class CTCPhysicsBase:public CTCBase{unsigned char unreadPhysics[0x40-0xc];
protected:virtual C2DVector GetOffsetVector()const;
protected:virtual C3DVector GetOffsetVector3D()const;
public:virtual void SetPosition(const C3DVector&);
public:virtual void ResetPosition(const C3DVector&);
public:virtual void ResetRHSet(const CRightHandedSet&);
public:virtual void SetPositionToNavigateTo(const C3DVector&);
public:virtual void SetForcedToMovePos(const C3DVector&,bool);
public:virtual void ResetMovementState();
public:virtual void EnablePhysics(bool);
public:virtual bool IsPhysicsEnabled()const;
public:virtual void EnableGravity(bool);
public:virtual bool IsGravityEnabled()const;
public:virtual void EnableNavigator(bool);
public:virtual bool IsNavigationEnabled()const;
public:virtual bool HasNavigationLayers()const;
public:virtual void SetInteractionFlags(long);
public:virtual void ReturnToDefaultInteractionFlags();
public:virtual void OnDroppedFromUpdateList();
public:virtual void OnAddedToUpdateList();
public:virtual bool IsAlwaysFacingMovementDirection()const;
public:virtual bool IsAffectedByGravity()const;
public:virtual bool IsAffectedByAir()const;
public:virtual bool IsAffectedByWind()const;
public:virtual bool IsAngledWithGround()const;
public:virtual bool IsImmoveable()const;
public:virtual bool IsOnAnySurface()const;
public:virtual bool IsOnAnyStationarySurface()const;
public:virtual bool IsOnMeshSurface()const;
public:virtual bool IsOnGroundSurface()const;
public:virtual bool CanCollideWithThings()const;
public:virtual bool CanCollideWithThingsOneWay()const;
public:virtual bool CanCollideWithLandscape()const;
public:virtual bool CanInteractWithThings()const;
public:virtual bool IsExcludingCreatures()const;
public:virtual bool IsPhysicsMeshImpassable()const;
public:virtual float GetFacingAngleXY()const;
public:virtual float GetFacingAngleYZ()const;
public:virtual float GetFacingAngleXZ()const;
public:virtual void SetFacingAngleXY(float);
public:virtual void SetFacingAngleYZ(float);
public:virtual void SetFacingAngleXZ(float);
public:virtual C3DVector GetFacingVector()const;
public:virtual C3DVector GetFacingVector(float)const;
public:virtual const CRightHandedSet& GetRHSet()const;
public:virtual CRightHandedSet GetRHSet(float)const;
};

namespace NGraphicAppearance{
// Layout: existing shared graphic appearance header supplies the native base; called methods do not expose unobserved fields.
class CTCGraphicAppearance:public CTCGraphicAppearanceFields{public:bool GetDummyObject(const CCharString&,long,unsigned long,C3DVector&,CRightHandedSet&,float)const;bool GetHelperPoint(const CCharString&,long,unsigned long,C3DVector&,float)const;};
}
// Layout: shared CThing PhysicsTC60 and PTCGraphicAppearance64 retain original accessor names and namespace.
class CThing:public CThingFields{public:const C3DVector&GetPos()const;CTCPhysicsBase*GetPhysicsTC(){return PhysicsTC;}const CTCPhysicsBase*GetPhysicsTC()const{return PhysicsTC;}NGraphicAppearance::CTCGraphicAppearance*GetTCGraphicAppearance(){return reinterpret_cast<NGraphicAppearance::CTCGraphicAppearance*>(PTCGraphicAppearance);}};
namespace NParticleEngine{
// Layout: shared header native OrientationInterpolator60 and PositionInterpolator94; original end values are7c/a4.
class CParticleEmitter:public CParticleEmitterFields{public:CRightHandedSet GetCurrentOrientation()const;void SetPosition(const C3DPositionInterpolator&);void SetOrientation(const C3DOrientationInterpolator&);const C3DVector&PeekEndPos()const{return reinterpret_cast<const C3DPositionInterpolator&>(PositionInterpolator).EndPosition;}const CRightHandedSet&PeekEndOrientation()const{return reinterpret_cast<const C3DOrientationInterpolator&>(OrientationInterpolator).EndOrientation;}};
}
class CTCDriverBase:public CTCBase{};
// Layout: raw original CTCDParticleEmitter72, corroborated by native field offsets14/1c/24/30/3c/44/45.
class CTCDParticleEmitter:public CTCDriverBase{
private:
 class CParticleCreationParameters{public:bool InterpolatingParticle,ParticleCreationDisabled,UpdateParametersSet,AlwaysUpdateFlag;long StartParticleID,EndParticleID;float Interpolate;CRightHandedSet RHSet;};
 CCharString EmitterName;long ParticleTypeID;CIntelligentPointer<NParticleEngine::CParticleEmitter>PEmitter;CIntelligentPointer<CThing>PAttachedToThing;unsigned long AttachmentFlags;CCharString AttachmentPosName;long AttachmentPosIndex;float FloatOffset;long DieOffTimer,FramesLeftToWaitForParticleToCreate;CCountedPointer<CParticleCreationParameters>PCreationParameters;
 bool WaitingForParticleToLoad:1;bool ForceParticleToLoad:1;bool IndependantObject:1;bool ParticleCreationDisabled:1;bool ResetEmitterPosition:1;bool ResetEmitterOrientation:1;bool AttachedToCamera:1;bool Moving:1;bool AttachedToThing:1;bool AlwaysUpdate:1;bool Asleep:1;
public:void UpdateEmitterThingAttachment(bool,float);
};
void CTCDParticleEmitter::UpdateEmitterThingAttachment(bool interpolate, float time)
{
    if (!PEmitter.GetPItem() && !WaitingForParticleToLoad)
        return;
    C3DVector position = PAttachedToThing.GetPItem()->GetPos();
    CRightHandedSet orientation;
    if (PEmitter.GetPItem())
        orientation = (AttachmentFlags & 0x20) ? PAttachedToThing.GetPItem()->GetPhysicsTC()->GetRHSet()
                                               : PEmitter.GetPItem()->GetCurrentOrientation();
    else
        orientation = (AttachmentFlags & 0x20) ? PAttachedToThing.GetPItem()->GetPhysicsTC()->GetRHSet()
                                               : PCreationParameters->RHSet;
    if (!ResetEmitterPosition && !ResetEmitterOrientation)
        return;
    if (PAttachedToThing.GetPItem()->GetTCGraphicAppearance()) {
        if (AttachmentFlags & 2)
            PAttachedToThing.GetPItem()->GetTCGraphicAppearance()->GetDummyObject(AttachmentPosName, AttachmentPosIndex, 0, position, orientation, time);
        else if (AttachmentFlags & 4)
            PAttachedToThing.GetPItem()->GetTCGraphicAppearance()->GetHelperPoint(AttachmentPosName, AttachmentPosIndex, 0, position, time);
    }
    if (AttachmentFlags & 0x40) {
        float water = PeekWorldMap().GetWaterSizeZAt(position);
        if (water > 0.0f)
            position.Z = water;
    }
    if (AttachmentFlags & 0x80)
        position.Z += FloatOffset;
    if (ResetEmitterPosition) {
        ParentThing.GetPhysicsTC()->SetPosition(position);
        if (PEmitter.GetPItem()) {
            const C3DVector& start = interpolate ? position : PEmitter.GetPItem()->PeekEndPos();
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            emitter->SetPosition(C3DPositionInterpolator(start, position));
        }
    }
    if (ResetEmitterOrientation) {
        if (PEmitter.GetPItem()) {
            const CRightHandedSet& start = interpolate ? orientation : PEmitter.GetPItem()->PeekEndOrientation();
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            emitter->SetOrientation(C3DOrientationInterpolator(start, orientation));
        } else {
            PCreationParameters->RHSet = orientation;
        }
    }
}