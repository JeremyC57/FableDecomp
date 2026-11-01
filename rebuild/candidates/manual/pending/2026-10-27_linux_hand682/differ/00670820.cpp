// CTCCoopSpirit::UpdateMovement() -- clamp the spirit's physics velocity to its maximum speed and face it along the XY heading, then apply the queued movement, the pull back to the master and the screen-edge forces; the no-movement timer resets while the creature is performing an action
// Grade: DIFFER(483v483) -- the x87 square order of GetLength/Normalise: retail squares Z, Y then X (X*X kept on the stack, slot +8) and loads the velocity copy Z, Y, X; the lane squares Z, X then Y (Y*Y kept) whatever the source order of the sum (expression order, named locals, parenthesised sums, copy-ctor order, getters all tried); everything else byte-exact
// Leaf: ?UpdateMovement@CTCCoopSpirit@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00670820, 483 bytes, thiscall, plain `ret`.
// Name: Ego_r.pdb `?UpdateMovement@CTCCoopSpirit@@AAEXXZ` (private, void); the catalog labels it CTCCoopSpirit::UpdateMovement.
//   Caller: CTCCoopSpirit::FrameUpdate 0x006709F0 region; the callees are the three private sibling updaters
//   UpdateAppliedMovement 0x006702E0, UpdateAttractionToMaster 0x006701A0, UpdateScreenRestrictingForces 0x0066FC90
//   (Ego_r `?UpdateAppliedMovement@CTCCoopSpirit@@AAEXXZ` etc.), CTCPhysicsBase::SetAsMoving(bool) 0x006B0130,
//   CThingCreatureBase::IsPerformingAnyAction() const 0x006622A0, the physics virtual slot 66 (+0x108) =
//   CTCPhysicsBase::SetFacingAngleXY(float) (CTCPhysicsStandard override 0x00723760) and the fmod intrinsic
//   _CIfmod 0x00BFEB28. GetMaxVelocity (Ego_r `?GetMaxVelocity@CTCCoopSpirit@@ABEMXZ`) is inline: retail
//   compares against the literal 1.027f (0x0125B250).
// Globals: EPSILON 0x0129BA3C (1e-4f), g_InvSqrtMantissaTable 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C,
//   1/(2 pi) 0x01237F08, double 1.0 0x01237F00, 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Shape: the velocity is copied into a local C3DVector; GetLength() stores the three squares to the frame
//   (fsqrt on their sum) and the inline fast Normalise re-adds the stored squares; the clamp is
//   `if (length > GetMaxVelocity()) length = GetMaxVelocity();`; the scaled components are written straight back
//   to the physics velocity; the heading is the landed `atan2(X, Y) * (1 / 2pi)` wrapped by the __forceinline
//   WrapAngle (0x00661540 model); the timer tail is `if (IsPerformingAnyAction()) timer = 0; else ++timer;`.
// Layout: CTCBase/CTCPhysicsBase as the landed 0x007AF0F0 virtual-prefix model (engine/CTCPhysicsBase.h is flattened:
//   Velocity_X/Y/Z +0x28 with no virtuals); CTCCoopSpirit = Ego_r (NoMovementTimer +0xc); CThing::PhysicsTC +0x60.
#include <math.h>
#pragma intrinsic(atan2, fmod, sqrt)
#include "engine/CBaseClassNonCopyable.h"

extern const float EPSILON;                          // 0x0129BA3C (1e-4f)
extern unsigned long g_InvSqrtMantissaTable[128];    // 0x013CE558

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = (x * x + y * y) + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

class CThing; class CCharString; class CDefPointeeBase; class C2DVector; class CRightHandedSet; class CTCParamsBase;
class CEnginePrimitivePrepareInfo; class CPersistContext; class CPlayer; class CCoopSpiritDef;
namespace NTCMessages { class CMessageBase; }
enum ETCInterfaceType {};

class CTCBase : public CBaseClassNonCopyable {              // engine/CTCBase.h layout: ParentThing +0x04
public:
    CThing* ParentThing;
private:
    bool ToBeRemoved, IsInGlobalUpdateTCs;
protected: virtual void OnSerialise(CPersistContext&, bool);
public: virtual void OnRemove();
public: virtual void ConstructFromParams(const CTCParamsBase&);
public: virtual void OnCreate();
public: virtual void InitialActivate();
public: virtual void OnInitialActivate();
public: virtual void RenderUpdate(float);
public: virtual bool HasFrameUpdate() const;
public: virtual void FrameUpdate();
public: virtual void BulletTimeFrameUpdate();
public: virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
public: virtual void Copy(const CTCBase&);
public: virtual void OnDie();
public: virtual void OnKill();
public: virtual void OnLevelUnload();
public: virtual void OnSetInLimbo(bool);
public: virtual bool Validate(CCharString&) const;
public: virtual bool IsFreeForDeletion() const;
public: virtual bool IsSerialiseCreatable() const;
public: virtual bool IsSerialisableForHeroState() const;
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual CCharString GetClassTypeName() const;
public: virtual bool IsComponentSerialisable() const;
public: virtual bool IsDriver() const;
public: virtual bool IsCopyable() const;
public: virtual void GlobalUpdate(bool);
public: virtual void HandleMessage(const NTCMessages::CMessageBase&);
};

class CTCPhysicsBase : public CTCBase {                     // engine/CTCPhysicsBase.h layout (0x40): Velocity +0x28
    unsigned char unread_0c[0x28 - 0xc];
public:
    C3DVector Velocity;                                     // +0x28
    unsigned char unread_34[0x40 - 0x34];
    void SetAsMoving(bool moving);                          // 0x006B0130
protected: virtual C2DVector GetOffsetVector() const;
protected: virtual C3DVector GetOffsetVector3D() const;
public: virtual void SetPosition(const C3DVector&);
public: virtual void ResetPosition(const C3DVector&);
public: virtual void ResetRHSet(const CRightHandedSet&);
public: virtual void SetPositionToNavigateTo(const C3DVector&);
public: virtual void SetForcedToMovePos(const C3DVector&, bool);
public: virtual void ResetMovementState();
public: virtual void EnablePhysics(bool);
public: virtual bool IsPhysicsEnabled() const;
public: virtual void EnableGravity(bool);
public: virtual bool IsGravityEnabled() const;
public: virtual void EnableNavigator(bool);
public: virtual bool IsNavigationEnabled() const;
public: virtual bool HasNavigationLayers() const;
public: virtual void SetInteractionFlags(long);
public: virtual void ReturnToDefaultInteractionFlags();
public: virtual void OnDroppedFromUpdateList();
public: virtual void OnAddedToUpdateList();
public: virtual bool IsAlwaysFacingMovementDirection() const;
public: virtual bool IsAffectedByGravity() const;
public: virtual bool IsAffectedByAir() const;
public: virtual bool IsAffectedByWind() const;
public: virtual bool IsAngledWithGround() const;
public: virtual bool IsImmoveable() const;
public: virtual bool IsOnAnySurface() const;
public: virtual bool IsOnAnyStationarySurface() const;
public: virtual bool IsOnMeshSurface() const;
public: virtual bool IsOnGroundSurface() const;
public: virtual bool CanCollideWithThings() const;
public: virtual bool CanCollideWithThingsOneWay() const;
public: virtual bool CanCollideWithLandscape() const;
public: virtual bool CanInteractWithThings() const;
public: virtual bool IsExcludingCreatures() const;
public: virtual bool IsPhysicsMeshImpassable() const;
public: virtual float GetFacingAngleXY() const;
public: virtual float GetFacingAngleYZ() const;
public: virtual float GetFacingAngleXZ() const;
public: virtual void SetFacingAngleXY(float);              // slot 66 (+0x108)
public: virtual void SetFacingAngleYZ(float);
public: virtual void SetFacingAngleXZ(float);
};

class CThing { public:                                       // engine/_quarantine/CThing.h: PhysicsTC +0x60
    unsigned char unread_00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
};
class CThingCreatureBase : public CThing { public:
    bool IsPerformingAnyAction() const;                     // 0x006622A0
};

template <class T> struct CDefPointer { const T* Object; };
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); private: void* PData; };
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {};

class CTCCoopSpirit : public CTCBase {                       // Ego_r CTCCoopSpirit (0x34), vtable 0x0125B264
    long                           NoMovementTimer;        // +0x0c
    long                           Score;                  // +0x10
    long                           OffscreenCounter;       // +0x14
    C3DVector                      MovementVector;         // +0x18
    CPlayer*                       Master;                 // +0x24
    CDefPointer<CCoopSpiritDef>    PCoopSpiritDef;         // +0x28
    CIntelligentPointer<CThing>    PParticleEmitter;       // +0x2c

    float GetMaxVelocity() const { return 1.027f; }        // Ego_r ?GetMaxVelocity@CTCCoopSpirit@@ABEMXZ
    CThingCreatureBase& GetParentCreature() const { return *(CThingCreatureBase*)ParentThing; }
    void UpdateMovement();                                  // 0x00670820
    void UpdateAppliedMovement();                           // 0x006702E0
    void UpdateAttractionToMaster();                        // 0x006701A0
    void UpdateScreenRestrictingForces();                   // 0x0066FC90
};

void CTCCoopSpirit::UpdateMovement()
{
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    C3DVector velocity = physics->Velocity;
    float length = velocity.GetLength();
    if (length > EPSILON) {
        velocity.Normalise();
        if (length > GetMaxVelocity())
            length = GetMaxVelocity();
        velocity *= length;
        physics->Velocity = velocity;
        physics->SetAsMoving(true);
        physics->SetFacingAngleXY(WrapAngle((float)atan2(velocity.X, velocity.Y) * 0.15915494f));
    }
    UpdateAppliedMovement();
    UpdateAttractionToMaster();
    UpdateScreenRestrictingForces();
    if (GetParentCreature().IsPerformingAnyAction())
        NoMovementTimer = 0;
    else
        ++NoMovementTimer;
}
