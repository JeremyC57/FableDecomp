// NThingShot::CTCShotDefinitionTearInGround::InitMovement() -- start a ground-tearing shot toward a live target: the horizontal direction to the target's fire-at point, the shot placed on the ground two units along it, moving at the def's speed per frame, with a hit-on-collision component, then the first movement update
// Grade: DIFFER(308v308) -- AddTC this-load placement (the 0x007EAC70 residue mirrored); this sweep: PDB AddTC signature (const CTCParamsBase*) -- inert
// Leaf: InitMovement
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BC8FC, 308 bytes (plain `ret`). NThingShot::CTCShotDefinitionTearInGround vtable
//   0x0123CF9C slot 33. The shot TU is size-optimised with a frame pointer: `optimize("y", off)` + `("s", on)`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), IS_THING_ALIVE<CThing*> @ 0x00436380 (`__fastcall`, by
//   const reference), GetCreatureFireAtPos(CThing*) @ 0x007BEDF0 (`__fastcall`, C3DVector through the hidden pointer in ecx),
//   C3DVector::Normalise @ 0x00A14440, CTCBase::GetWorldMap() @ 0x00686D30, CWorldMap::GetGroundSizeZAt(const C3DVector&)
//   @ 0x0063A300 (float), the physics body's SetPosition (slot 31, +0x7C), CTCPhysicsBase::SetVelocity(const C3DVector&)
//   @ 0x0066F9B0 (manifest "CTCPhysicsRigidBody::SetFacingAnglesFromRHSet", named from this use), the physics body's slot 43
//   (+0xAC, 0x14), CCharString(const char*, long) @ 0x0099EBF0 ("CTCHitOnCollision" @ 0x0123D1D8), CThing::AddTC(const
//   CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0, the shot's UpdateMovement (slot 35, +0x8C).
//   2.0f from 0x0122DCDC; g_framesPerSecond @ 0x01375550.
// Layout: Ego_r.pdb CTCShotDefinitionBase PTargetThing +0x20 (PDB 0x24), PShotDef +0x38; CThingShotDef Speed +0x3C (PDB 0x40);
//   retail CThing PhysicsTC +0x60; CTCPhysicsBase Position +0x0C.
// Shape: the liveness test binds the GetPItem result as a temporary (its frame word is reused by the component-name
//   temporary); the direction is the inline difference with Z zeroed afterwards (the Z subtraction is dead); the ground
//   point's height stays on the x87 stack into the inline sum; C3DVector temporaries share one frame slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector {
    float X, Y, Z;
    __forceinline C3DVector() {}
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& Normalise();                             // 0x00A14440
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CThing;
struct CTCParamsBase;
struct CTCBaseComponent;
C3DVector __fastcall GetCreatureFireAtPos(CThing* thing);   // 0x007BEDF0
bool __fastcall IS_THING_ALIVE(CThing* const& thing);  // 0x00436380
extern long g_framesPerSecond;                          // 0x01375550
struct CWorldMap {
    float GetGroundSizeZAt(const C3DVector& position);  // 0x0063A300
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void SetPosition(const C3DVector& position);    // slot 31 (+0x7c)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void SetCollisionFlags(long flags);         // slot 43 (+0xac)
    void*     ParentThing;                              // +0x04
    unsigned char _pad08[0x4];
    C3DVector Position;                                 // +0x0c
    void SetVelocity(const C3DVector& velocity);        // 0x0066F9B0
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    CTCBaseComponent* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CThingShotDef {
    unsigned char _pad00[0x3c];
    float         Speed;                                // +0x3c
};
namespace NThingShot {
struct CTCShotDefinitionTearInGround {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void InitMovement();                        // slot 33
    virtual void v34();
    virtual void UpdateMovement();                      // slot 35 (+0x8c)
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x20 - 0x08];
    CBaseIntelligentPointer PTargetThing;               // +0x20
    unsigned char _pad28[0x38 - 0x28];
    const CThingShotDef* PShotDef;                      // +0x38 CDefPointer
    CWorldMap* GetWorldMap();                           // 0x00686D30 (CTCBase)
};

void CTCShotDefinitionTearInGround::InitMovement()
{
    if (!IS_THING_ALIVE(PTargetThing.GetPItem()))
        return;
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    C3DVector direction = GetCreatureFireAtPos(PTargetThing.GetPItem()) - physics->Position;
    direction.Z = 0.0f;
    direction.Normalise();
    C3DVector ground = physics->Position;
    ground.Z = GetWorldMap()->GetGroundSizeZAt(ground);
    physics->SetPosition(direction * 2.0f + ground);
    physics->SetVelocity(direction * (PShotDef->Speed / g_framesPerSecond));
    physics->SetCollisionFlags(0x14);
    ParentThing->AddTC("CTCHitOnCollision", false, 0);
    UpdateMovement();
}
}
