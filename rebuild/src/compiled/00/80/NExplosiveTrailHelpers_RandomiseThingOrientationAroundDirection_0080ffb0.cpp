// NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection(CThing& thing, const C3DVector& direction) -- give a thing a random orientation: spin the default forward axis a random amount about the up axis, tilt both axes by up to +-0.125 about the direction, and hand the set to the thing's physics
// Leaf: ?RandomiseThingOrientationAroundDirection@NExplosiveTrailHelpers@@YIXAAVCThing@@ABVC3DVector@@@Z
// Retail: Fable.exe @ 0x0080FFB0, 229 bytes (+ int3 padding), `__fastcall` (thing in ecx -> ebx, direction in edx ->
//   esi), plain `ret`.
// Name: Ego_r.pdb / FableWin.pdb `?RandomiseThingOrientationAroundDirection@NExplosiveTrailHelpers@@YIXAAVCThing@@ABVC3DVector@@@Z`
//   (name set); callers 0x008107B5 and 0x008DEB24.
// Callees: CWorld::GetWorldSeed() const 0x0049D810 (`mov eax,[ecx+8] ; add eax,1613Ch`; GetLocalSeed is 0x0049D800) on the
//   main game component's world (g_pMainGameComponent 0x013B86A0, the inline GetWorld() reads +0x24 once);
//   GFFloatRandom(float, unsigned long*) 0x004978A0 (__fastcall: the seed in ecx, the range pushed);
//   C3DVector::Rotate(const C3DVector&, float) 0x00A13C60; the physics component's virtual slot 75 (+0x12c,
//   CTCPhysicsBase::SetRHSet(const CRightHandedSet&): Ego_r donor order SetFacingAnglesFromRHSet, UpdateOldRHSet, ResetRHSet,
//   SetRHSet matches CTCPhysicsStandard's slot-74 0x007238E0 .. slot-75 0x007239C0 bodies, and the slot-75 body calls
//   UpdateOldRHSet (slot 95) then stores the set; the manifest's slot-75 label SetFacingAnglesFromRHSet is the slot-74 name).
// Layout: retail CThing PhysicsTC +0x60 (PDB +0x70); CRightHandedSet Up +0x00, Forward +0x0c (Ego_r.pdb). CTCPhysicsBase is a
//   local polymorphic model: engine/CTCPhysicsBase.h flattens the vptr into a data word and cannot host the slot-75 virtual
//   SetRHSet call (slots 0..74 are fillers, see NOTES.md header debt).
// Shape: the world reference is a local (one load kept in edi across both seed calls); up (0,0,1) and forward (1,0,0) are
//   adjacent vector locals; the tilt is one float local (0.125 - random(0.25)) pushed for both tilts; the set handed to
//   the physics is a CRightHandedSet(up, forward) temporary (the 24-byte pairwise copy). The physics pointer is a NAMED
//   local read before the call: that loads it into ecx ahead of the copy, so the copy runs in eax/edx as retail (the
//   parked `thing.PhysicsTC->SetRHSet(...)` spelling copied through three registers and loaded ecx last).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    void Rotate(const C3DVector& axis, float angle);         // 0x00A13C60
};

class CRightHandedSet {
public:
    C3DVector Up;                                            // +0x00
    C3DVector Forward;                                       // +0x0c
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};

#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
class CWorld : public CWorldLayout {                         // engine/CWorld.h layout; the method is the PDB's
public:
    unsigned long* GetWorldSeed() const;                     // 0x0049D810
};

class CMainGameComponent {
public:
    unsigned char _pad_0x00[0x24];
    CWorld*       PWorld;                                    // +0x24 scoped_ptr<CWorld>
    CWorld& GetWorld() { return *PWorld; }
};
extern CMainGameComponent* g_pMainGameComponent;             // 0x013B86A0

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual void SetRHSet(const CRightHandedSet& rh_set);   // slot 75 (+0x12c)
};

class CThing {
public:
    unsigned char   _pad_0x00[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60 (PDB +0x70)
};

namespace NExplosiveTrailHelpers {
void __fastcall RandomiseThingOrientationAroundDirection(CThing& thing, const C3DVector& direction)
{
    CWorld& world = g_pMainGameComponent->GetWorld();
    C3DVector up(0.0f, 0.0f, 1.0f);
    C3DVector forward(1.0f, 0.0f, 0.0f);
    forward.Rotate(up, GFFloatRandom(1.0f, world.GetWorldSeed()));
    const float tilt = 0.125f - GFFloatRandom(0.25f, world.GetWorldSeed());
    up.Rotate(direction, tilt);
    forward.Rotate(direction, tilt);
    CTCPhysicsBase* physics = thing.PhysicsTC;
    physics->SetRHSet(CRightHandedSet(up, forward));
}
}