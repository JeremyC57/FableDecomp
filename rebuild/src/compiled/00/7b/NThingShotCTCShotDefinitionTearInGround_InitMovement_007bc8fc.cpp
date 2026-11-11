// NThingShot::CTCShotDefinitionTearInGround::InitMovement() -- start a ground-tearing shot toward a live target: the horizontal direction to the target's fire-at point, the shot placed on the ground two units along it, moving at the def's speed per frame, with a hit-on-collision component, then the first movement update
// Leaf: ?InitMovement@CTCShotDefinitionTearInGround@NThingShot@@UAEXXZ
// Lever (chunk 840): naming the per-frame speed (`const float speed = PShotDef->Speed / CMainGameComponent::GetConstantFPS();` before SetVelocity)
//   flips VC7.1's tie-break at the AddTC call: the name temporary's `lea eax,[ebp-4]` now precedes the ParentThing load
//   `mov ecx,[edi+4]` as in retail (an IR-numbering tie; every AddTC-site spelling alone kept ours). Same bytes otherwise.
// Names (chunk 840): NThingShot::GetCreatureFireAtPos(const CThing&) (Ego_r.pdb ?GetCreatureFireAtPos@NThingShot@@YI?AVC3DVector@@
//   ABVCThing@@@Z, the landed 0x007BEDF0 leaf is a descriptive free `GetCreatureFireAtPos(CThing*)`: NAME_FIXES), IS_THING_ALIVE<const
//   CThing*> (??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z; the landed bindings use `IS_THING_ALIVE_00436380` placeholders),
//   CThing::AddTC(const CCharString&, bool, const CTCParamsBase*) -> CTCBase* (?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBV
//   CTCParamsBase@@@Z), CWorldMap::GetGroundSizeZAt const (?GetGroundSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z), CTCBase::GetWorldMap
//   (?GetWorldMap@CTCBase@@IAEAAVCWorldMap@@XZ, protected, CWorldMap&; engine/CTCBase.h).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BC8FC, 308 bytes (plain `ret`). NThingShot::CTCShotDefinitionTearInGround vtable
//   0x0123CF9C slot 33. The shot TU is size-optimised with a frame pointer: `optimize("y", off)` + `("s", on)`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), IS_THING_ALIVE<CThing*> @ 0x00436380 (`__fastcall`, by
//   const reference), GetCreatureFireAtPos(CThing*) @ 0x007BEDF0 (`__fastcall`, C3DVector through the hidden pointer in ecx),
//   C3DVector::AccurateNormalise @ 0x00A14440, CTCBase::GetWorldMap() @ 0x00686D30, CWorldMap::GetGroundSizeZAt(const C3DVector&)
//   @ 0x0063A300 (float), the physics body's SetPosition (slot 31, +0x7C), CTCPhysicsBase::SetVelocity(const C3DVector&)
//   @ 0x0066F9B0 (manifest "CTCPhysicsRigidBody::SetFacingAnglesFromRHSet", named from this use), the physics body's slot 43
//   (+0xAC, 0x14), CCharString(const char*, long) @ 0x0099EBF0 ("CTCHitOnCollision" @ 0x0123D1D8), CThing::AddTC(const
//   CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0, the shot's UpdateMovement (slot 35, +0x8C).
//   2.0f from 0x0122DCDC; CMainGameComponent::GetConstantFPS() @ 0x01375550.
// Layout: Ego_r.pdb CTCShotDefinitionBase PTargetThing +0x20 (PDB 0x24), PShotDef +0x38; CThingShotDef Speed +0x3C (PDB 0x40);
//   retail CThing PhysicsTC +0x60; CTCPhysicsBase Position +0x0C.
// Shape: the liveness test binds the GetPItem result as a temporary (its frame word is reused by the component-name
//   temporary); the direction is the inline difference with Z zeroed afterwards (the Z subtraction is dead); the ground
//   point's height stays on the x87 stack into the inline sum; C3DVector temporaries share one frame slot.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class C3DVector {
public:
    float X, Y, Z;
    FABLE_FORCEINLINE C3DVector() {}
    FABLE_FORCEINLINE C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    FABLE_FORCEINLINE C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    FABLE_FORCEINLINE C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    FABLE_FORCEINLINE C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    FABLE_FORCEINLINE C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& AccurateNormalise();                     // 0x00A14440 ?AccurateNormalise@C3DVector@@QAEAAV1@XZ
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CThing;
class CTCParamsBase;
class CTCBase;
namespace NThingShot {
C3DVector __fastcall GetCreatureFireAtPos(const CThing& thing);   // 0x007BEDF0 ?GetCreatureFireAtPos@NThingShot@@YI?AVC3DVector@@ABVCThing@@@Z
}
template<class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x00436380 ??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z
// agentNF16: the global stand-in CMainGameComponent::GetConstantFPS() (0x01375550) is the protected static CMainGameComponent::ConstantFPS
//   (?ConstantFPS@CMainGameComponent@@1JA), read through the inline public accessor GetConstantFPS() (Ego_r + FableWin
//   ?GetConstantFPS@CMainGameComponent@@SIJXZ; shared engine/CMainGameComponent.h). Name-only: the load is the same.
#include "engine/CMainGameComponent.h"
class CWorldMap {
public:
    float GetGroundSizeZAt(const C3DVector& position) const;   // 0x0063A300 ?GetGroundSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z
};
class CTCPhysicsBase {
public:
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
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60 ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"   // CBaseIntelligentPointer 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (Ego_r.pdb)
class CThingShotDef {
public:
    unsigned char _pad00[0x3c];
    float         Speed;                                // +0x3c
};
class CTCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void InitMovement();                        // slot 33
    virtual void v34();
    virtual void UpdateMovement();                      // slot 35 (+0x8c)
    CThing*       ParentThing;                          // +0x04
protected:
    CWorldMap& GetWorldMap();                           // 0x00686D30 ?GetWorldMap@CTCBase@@IAEAAVCWorldMap@@XZ
};
namespace NThingShot {
class CTCShotDefinitionTearInGround : public CTCBase {
public:
    unsigned char _pad08[0x20 - 0x08];
    CBaseIntelligentPointer PTargetThing;               // +0x20
    unsigned char _pad28[0x38 - 0x28];
    const CThingShotDef* PShotDef;                      // +0x38 CDefPointer
    virtual void InitMovement();                        // slot 33
    virtual void UpdateMovement();                      // slot 35 (+0x8c)
};

void CTCShotDefinitionTearInGround::InitMovement()
{
    if (!IS_THING_ALIVE<const CThing*>(reinterpret_cast<CThing*>(PTargetThing.GetPItem())))
        return;
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    C3DVector direction = GetCreatureFireAtPos(*reinterpret_cast<CThing*>(PTargetThing.GetPItem())) - physics->Position;
    direction.Z = 0.0f;
    direction.AccurateNormalise();
    C3DVector ground = physics->Position;
    ground.Z = GetWorldMap().GetGroundSizeZAt(ground);
    physics->SetPosition(direction * 2.0f + ground);
    const float speed = PShotDef->Speed / CMainGameComponent::GetConstantFPS();
    physics->SetVelocity(direction * speed);
    physics->SetCollisionFlags(0x14);
    ParentThing->AddTC("CTCHitOnCollision", false, 0);
    UpdateMovement();
}
}