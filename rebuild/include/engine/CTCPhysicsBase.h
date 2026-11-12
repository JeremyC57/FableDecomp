#ifndef FABLE_ENGINE_CTCPHYSICSBASE_H
#define FABLE_ENGINE_CTCPHYSICSBASE_H
#ifdef FABLE_RETAIL_CTCPHYSICSBASE
// Retail opt-in (Codex pass 11 consolidation 2): the polymorphic retail CTCPhysicsBase : CTCBase (needs FABLE_RETAIL_CTCBASE); see below.
class CPhysicsMeshObject;
// PDB original virtual declarations; retail Physics slots are PDB offsets minus8.
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_COLLISION_RESPONSE
#include "engine/CDefPointer.h"
#include "engine/CPhysicsDef.h"
#endif
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
class C2DVector;
class CInterfaceType;
class CPersistContext;
class CPhysicsCollideResponse;
class CPhysicsDebugInfo;
class CRightHandedSet;
class CTCParamsBase;
class CThingPhysicalDebugInfo;
class CThingPhysicsMeshInfo;
class CTCPhysicsBase : public CTCBase {
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_IMPASSABLE
protected:
 void ImpulseRespondToCollisonWithImpassableObject(const CPhysicsCollideResponse&,const CTCPhysicsBase&,C3DVector&)const;
#endif
#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
public:
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_PHYSICS;}
#endif
    virtual ETCInterfaceType GetInterfaceType() const;
protected:
    virtual C2DVector GetOffsetVector() const;
protected:
    virtual C3DVector GetOffsetVector3D() const;
public:
    virtual void SetPosition(const C3DVector &);
public:
    virtual void ResetPosition(const C3DVector &);
public:
    virtual void ResetRHSet(const CRightHandedSet &);
public:
    virtual void SetPositionToNavigateTo(const C3DVector &);
public:
    virtual void SetForcedToMovePos(const C3DVector &,bool);
public:
    virtual void ResetMovementState();
public:
    virtual void EnablePhysics(bool);
public:
    virtual bool IsPhysicsEnabled() const;
public:
    virtual void EnableGravity(bool);
public:
    virtual bool IsGravityEnabled() const;
public:
    virtual void EnableNavigator(bool);
public:
    virtual bool IsNavigationEnabled() const;
public:
    virtual bool HasNavigationLayers() const;
public:
    virtual void SetInteractionFlags(long);
public:
    virtual void ReturnToDefaultInteractionFlags();
public:
    virtual void OnDroppedFromUpdateList();
public:
    virtual void OnAddedToUpdateList();
public:
    virtual bool IsAlwaysFacingMovementDirection() const;
public:
    virtual bool IsAffectedByGravity() const;
public:
    virtual bool IsAffectedByAir() const;
public:
    virtual bool IsAffectedByWind() const;
public:
    virtual bool IsAngledWithGround() const;
public:
    virtual bool IsImmoveable() const;
public:
    virtual bool IsOnAnySurface() const;
public:
    virtual bool IsOnAnyStationarySurface() const;
public:
    virtual bool IsOnMeshSurface() const;
public:
    virtual bool IsOnGroundSurface() const;
public:
    virtual bool CanCollideWithThings() const;
public:
    virtual bool CanCollideWithThingsOneWay() const;
public:
    virtual bool CanCollideWithLandscape() const;
public:
    virtual bool CanInteractWithThings() const;
public:
    virtual bool IsExcludingCreatures() const;
public:
    virtual bool IsPhysicsMeshImpassable() const;
public:
    virtual float GetFacingAngleXY() const;
public:
    virtual float GetFacingAngleYZ() const;
public:
    virtual float GetFacingAngleXZ() const;
public:
    virtual void SetFacingAngleXY(float);
public:
    virtual void SetFacingAngleYZ(float);
public:
    virtual void SetFacingAngleXZ(float);
public:
    // Retail order (chunk 718 round 3 pending, row 0x0088EDC0; carried here for chunk 732 row 0x006AABE0): the no-argument overload
    // is declared first, so VC7.1's reversed overload group puts it at slot 70 (+0x118; CTCPhysicsBase vtable 0x0125D2C4: slot 69
    // 0x006B1430 `ret 8` takes the float, slot 70 0x006B1410 `ret 4` the hidden result only). Ego_r.pdb defines the bodies in the
    // same order (0x006EE000 XZ, 0x006EE020 M@Z).
    virtual C3DVector GetFacingVector() const;
public:
    virtual C3DVector GetFacingVector(float) const;
public:
    virtual const CRightHandedSet & GetRHSet() const;
public:
    virtual CRightHandedSet GetRHSet(float) const;
public:
    virtual const CRightHandedSet & GetLastRHSet() const;
public:
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet &);
public:
    virtual void SetRHSet(const CRightHandedSet &);
public:
    virtual C3DVector GetImpulse() const;
public:
    virtual void SetRHSetAsFlushOnNormal(const C3DVector &);
public:
    virtual void SetRHSetAsFlushOnGround();
public:
    virtual float GetRenderZOffset(float) const;
public:
    virtual void ResetInterpolation();
public:
    virtual void ApplyControlledImpulse(const C3DVector &);
public:
    virtual void ApplyControlledAcceleration(const C3DVector &,float);
public:
    virtual bool IsPushableByHero() const;
public:
    virtual void SetAsPushableByHero(bool);
public:
    virtual void FrameUpdate();
public:
    virtual bool HasFrameUpdate() const;
public:
    virtual void SetPhysicsMeshBankIndex(long);
public:
    virtual void SetScaleFromOwner();
public:
    virtual void ConstructFromParams(const CTCParamsBase &);
public:
    virtual void OnCreate();
public:
    virtual void InitialActivate();
public:
    virtual void Move();
public:
    virtual bool DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const;
public:
    virtual void OnSerialise(CPersistContext &,bool);
public:
    virtual bool IsComponentSerialisable() const;
public:
    virtual bool IsSerialisableForHeroState() const;
public:
    virtual bool IsCopyable() const;
public:
    virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const;
public:
    virtual bool HasCollidedWithThingFromTo(const CTCPhysicsBase &,const C3DVector &,const C3DVector &,CPhysicsCollideResponse &) const;
public:
    virtual bool HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const;
public:
    virtual bool HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const;
public:
    virtual const CThingPhysicsMeshInfo * GetPhysicsMeshInfo() const;
protected:
    virtual void UpdateInterpolation();
protected:
    virtual void UpdateOldRHSet();
protected:
    virtual void UpdateOldPos();
public:
    virtual ~CTCPhysicsBase();
protected:
    C3DVector Position;
    C3DVector OldPosition;
    long OldPositionLastFrameSet;
    C3DVector Velocity;
    float Radius;
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_COLLISION_RESPONSE
    // Retail opt-in (Codex pass17): both PDBs and native006afe60 prove the real def handle.
    CDefPointer<const CPhysicsDef> PhysicsDef;
    void RespondToCollision(const CPhysicsCollideResponse& response_info,C3DVector& velocity) const;
#else
    unsigned char PhysicsDef[4];
#endif
    bool Moving:1,OldMoving:1;
public:
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
    // Both raw donors: public non-virtual SetAsMoving(bool), native006b0130.
    void SetAsMoving(bool);
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
// Evidence: both original PDB types and donor026fd4f0; native007c1000 accesses.
public:
void SetVelocity(const C3DVector& value){Velocity=value;SetAsMoving(true);}
bool IsMoving()const{return Moving;}
#endif
#endif
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_COLLISION_RESPONSE
    float GetFriction() const {return PhysicsDef->Friction;}
    float GetElasticity() const {return PhysicsDef->Elasticity;}
#endif
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_MAPWHOPOSITION
    // Retail opt-in (Codex pass17): donor024707b0, original pos local and copy.
    C3DVector GetMapwhoPosition() const {C3DVector pos=Position+GetOffsetVector();return pos;}
#endif
    void GetBoundingInfo(C3DVector&,float&) const;
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_CALCULATERADIUS
    // Row 0x00724630: Ego_r / FableWin ?CalculateRadius@CTCPhysicsBase@@IBEMXZ (protected, out of line 0x006AFE20) and
    // ?GetMapwhoPosition@CTCPhysicsBase@@QBE?AVC3DVector@@XZ called out of line (0x006AFCB0). Declarations only.
#ifndef FABLE_RETAIL_CTCPHYSICSBASE_MAPWHOPOSITION
    C3DVector GetMapwhoPosition() const;                             // 0x006AFCB0
#endif
protected:
    float CalculateRadius() const;                                   // 0x006AFE20
public:
#endif
    const C3DVector& GetPosition() const { return Position; }
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION
    // Retail opt-in (Codex pass17): original const value-return overload;0049d150.
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION_BODY
    // Nested opt-in (chunk 1014, row 0x007EA2F0): the body of the out-of-line 0x0049D150 (landed as GetInterpolatedPosition), expanded
    // where the TU sees it: OldPosition + (Position - OldPosition) * interpolate.
    C3DVector GetPosition(float interpolate) const { return (Position - OldPosition) * interpolate + OldPosition; }
#else
    C3DVector GetPosition(float interpolate) const;
#endif
#endif
    float GetRadius() const { return Radius; }
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_DIAMETER
    float GetDiameter() const { return (GetRadius()*2.0f); }
#endif
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
    // FableWin GetLastPosition01d8e980, inline read at007bc761: native OldPosition+18.
    const C3DVector& GetLastPosition() const { return OldPosition; }
#endif
    CPhysicsMeshObject GetPhysicsMeshObject() const;
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_GETMASS
// agentFAM04 (2026-11-11): Ego_r.pdb ?GetMass@CTCPhysicsBase@@QBEMXZ (public const, out of line in retail at 0x006AFE00: the
// PhysicsDef Mass +0x38 read; CTCPhysicsStandard::EnableGravity 0x007235E0 calls it). Declaration only: no layout change.
public:
    float GetMass() const;                                             // 0x006AFE00
#endif // FABLE_RETAIL_CTCPHYSICSBASE_GETMASS
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_DUMMYRHSET
// agentFAM04 (2026-11-11): the static right-handed set the base GetRHSet() / GetLastRHSet() return (0x006B1290 /
// 0x006B12A0 `mov eax,0137F9B4h`, 0x006B1450 copies it): FableWin.pdb names its initializer `dynamic initializer for
// 'CTCPhysicsBase::DummyRHSet'` (retail .data 0x0137F9B4 holds it statically: Up (0,0,1), Forward (0,1,0)). Access and
// const-ness are not in either PDB extract (protected assumed, as the class data). Declaration only.
protected:
    static CRightHandedSet DummyRHSet;                                  // 0x0137F9B4
public:
#endif // FABLE_RETAIL_CTCPHYSICSBASE_DUMMYRHSET
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_SEARCHTOOLS
    // Nested opt-in (chunk 758, row 0x00725B70 CTCPhysicsControlled::UpdateAdditionalImpulse): Ego_r.pdb
    // ?GetThingSearchTools@CTCPhysicsBase@@IBEABVCThingSearchTools@@XZ, out of line in retail at 0x006AFD40 (`call PeekWorld
    // 0x00686A80 ; mov eax,[eax+20h]`). Declaration only: no layout change.
protected:
    const class CThingSearchTools& GetThingSearchTools() const;        // 0x006AFD40
#endif
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_WORLDMAP
    // Nested opt-in (agentNF14, rows 0x006AF620 / 0x006AF630): Ego_r.pdb ?GetWorldMap@CTCPhysicsBase@@IAEAAVCWorldMap@@XZ and
    // ?GetWorldMap@CTCPhysicsBase@@IBEABVCWorldMap@@XZ, adjacent 16-byte bodies in the Ego_r donor order (0x006EC300 / 0x006EC310) as
    // in retail: `call GetWorld 0x00686A70 ; jmp [vtbl+30h]` (CWorld slot 12 GetWorldMap) and `call PeekWorld 0x00686A80 ;
    // jmp [vtbl+34h]` (slot 13 PeekWorldMap). They hide CTCBase::GetWorldMap. Declarations only: no layout change.
protected:
    CWorldMap& GetWorldMap();                                          // 0x006AF620
    const CWorldMap& GetWorldMap() const;                              // 0x006AF630
#endif
#if defined(FABLE_RETAIL_CTCPHYSICSBASE_SETVELOCITY_OUTOFLINE) && !(defined(FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING) && defined(FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION))
    // Retail opt-in (chunk Q20, rows 0x007BCF7F / 0x007BD845 InitMovement): Ego_r.pdb ?SetVelocity@CTCPhysicsBase@@QAEXABVC3DVector@@@Z
    // called out of line there (0x007BD188 `call 0x0066F9B0`, the landed body: store Velocity, then SetAsMoving(true)). Declaration
    // only; the inline body under SETASMOVING + CTCSHOTDEFINITIONBASE_COLLISION is the 0x007C1000 TU's.
public:
    void SetVelocity(const C3DVector& velocity);                       // 0x0066F9B0
#endif
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
    // Chunk 764 (row 0x007BC004 CTCShotDefinitionNymphFun::InitMovement): Ego_r.pdb ?ApplyAcceleration@CTCPhysicsBase@@QAEXABVC3DVector@@@Z
    // (retail 0x006B0210, non-virtual). Declaration only: no layout change.
public:
    void ApplyAcceleration(const C3DVector& acceleration);
#endif
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_CREATEPPHYSICSMESHINFO
    // Chunk Q28 (row 0x006B05A0): Ego_r.pdb / FableWin.pdb ?CreatePPhysicsMeshInfo@CTCPhysicsBase@@IAEXABVCEngineGraphic@@@Z
    // (protected, non-virtual). Declaration only: no layout change.
protected:
    void CreatePPhysicsMeshInfo(const class CEngineGraphic& graphic);   // 0x006B05A0
public:
#endif
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_SETPHYSICSMESHINFOFROMGRAPHIC
    // NF28 (2026-11-12, row 0x006B08E0): Ego_r.pdb 0x006ED880 / FableWin.pdb 0x02470D30
    // ?SetPhysicsMeshInfoFromGraphic@CTCPhysicsBase@@QAEXABVCEngineGraphic@@@Z (public, non-virtual), the 5-byte `jmp` onto
    // CreatePPhysicsMeshInfo 0x006B05A0. Declaration only: no layout change.
public:
    void SetPhysicsMeshInfoFromGraphic(const class CEngineGraphic& graphic);   // 0x006B08E0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCPhysicsBase)==0x40);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCPhysicsBase = 0x40.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CDefPointeeBase;

#ifdef FABLE_RETAIL_CTCPHYSICSBASE_DUMMYRHSET
class CRightHandedSet;
#endif
#pragma pack(push, 1)
class CTCPhysicsBase {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    long             sub;                                    // +0x04 retail-only long (previous header)
    unsigned char    _pad_0x08[0x4];                         // +0x08
    float            Position_X;                             // +0x0c C3DVector::X
    float            Position_Y;                             // +0x10 C3DVector::Y
    float            Position_Z;                             // +0x14 C3DVector::Z
    float            OldPosition_X;                          // +0x18 C3DVector::X
    float            OldPosition_Y;                          // +0x1c C3DVector::Y
    float            OldPosition_Z;                          // +0x20 C3DVector::Z
    long             OldPositionLastFrameSet;                // +0x24
    float            Velocity_X;                             // +0x28 C3DVector::X
    float            Velocity_Y;                             // +0x2c C3DVector::Y
    float            Velocity_Z;                             // +0x30 C3DVector::Z
    float            Radius;                                 // +0x34
    CDefPointeeBase* PhysicsDef_Object;                      // +0x38 CDefPointer<CPhysicsDef_const_>::Object
    unsigned char    OldMoving[0x4];                         // +0x3c bitfield group: bool:1
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_GETMASS
    // agentNF19: the same declaration as the retail branch's opt-in (Ego_r.pdb ?GetMass@CTCPhysicsBase@@QBEMXZ, 0x006AFE00),
    // for rows on this layout view (0x007232E0 CTCPhysicsLight::OnCreate). Declaration only: no layout change.
    float GetMass() const;                                   // 0x006AFE00
#endif
#ifdef FABLE_RETAIL_CTCPHYSICSBASE_DUMMYRHSET
    // agentNF19: the retail branch's static (FableWin.pdb `dynamic initializer for 'CTCPhysicsBase::DummyRHSet'`; access as
    // that opt-in assumes), for rows on this layout view (0x006B1450 CTCPhysicsBase::GetRHSet). Declaration only.
protected:
    static CRightHandedSet DummyRHSet;                       // 0x0137F9B4
public:
#endif
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCPhysicsBase) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, sub) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_X) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_Y) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_Z) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_X) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_Y) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_Z) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPositionLastFrameSet) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_X) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_Y) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_Z) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Radius) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, PhysicsDef_Object) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldMoving) == 0x3c);

#endif // FABLE_RETAIL_CTCPHYSICSBASE
#endif // FABLE_ENGINE_CTCPHYSICSBASE_H
