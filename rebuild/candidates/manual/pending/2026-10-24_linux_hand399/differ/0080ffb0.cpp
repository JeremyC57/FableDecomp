// Grade: DIFFER(229v229) -- only the final CRightHandedSet copy + virtual call differ: retail loads the physics pointer (mov ecx,[ebx+60h]) first and copies the 24-byte set as a block in dword pairs (+4,+0),(+0Ch,+8),(+14h,+10h) through eax/edx; ours copies vector by vector through eax/ecx/edx and loads the physics pointer after the copy
// NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection(CThing& thing, const C3DVector& direction) -- give a thing a random orientation: spin the default forward axis a random amount about the up axis, tilt both axes by up to +-0.125 about the direction, and hand the set to the thing's physics
// Leaf: ?RandomiseThingOrientationAroundDirection@NExplosiveTrailHelpers@@YIXAAVCThing@@ABVC3DVector@@@Z
// Retail: Fable.exe @ 0x0080FFB0, 229 bytes (+ int3 padding), `__fastcall` (thing in ecx -> ebx, direction in edx ->
//   esi), plain `ret`.
// Name: Ego_r.pdb `?RandomiseThingOrientationAroundDirection@NExplosiveTrailHelpers@@YIXAAVCThing@@ABVC3DVector@@@Z`
//   (name set only); the catalogue already carries the bare name. Callers 0x008107B5 and 0x008DEB24.
// Callees: CWorld::GetPRandomSeed() 0x0049D810 (manifest "CVertexBufferWin32::DoSizeof" is propagated) on the main game
//   component's world (g_pMainGameComponent 0x013B86A0, PWorld +0x24, read once); GFFloatRandom(float, unsigned long*)
//   0x004978A0 (__fastcall: the seed in ecx, the range pushed); C3DVector::Rotate(const C3DVector&, float) 0x00A13C60;
//   the physics component's virtual slot 75 (+0x12c, SetRHSet(const CRightHandedSet&)).
// Layout: retail CThing PhysicsTC +0x60 (PDB +0x70); CRightHandedSet Up +0x00, Forward +0x0c (Ego_r.pdb).
// Shape: the world pointer is a local (one load kept in edi across both seed calls); up (0,0,1) and forward (1,0,0) are
//   separate vectors and the set handed to the physics is a CRightHandedSet temporary built from them (the 24-byte
//   copy before the virtual call); the tilt is one float local (0.125 - random(0.25)) pushed for both tilts.
// Tried: this form (229v229), /Oa (identical), a trivially-copyable C3DVector (identical), and one CRightHandedSet local
//   passed through an inline by-value CThing::SetRHSet (277v229: the C3DVector constructor temporaries are copied).
//   Retail's pairwise copy is a whole-struct block copy of a trivially copyable 24-byte set, so the source likely
//   copies one CRightHandedSet object (initialised in place, not from vector temporaries) into a temporary.
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

class CWorld {
public:
    unsigned long* GetPRandomSeed();                         // 0x0049D810
};

class CMainGameComponent {
public:
    unsigned char _pad_0x00[0x24];
    CWorld*       PWorld;                                    // +0x24 scoped_ptr<CWorld>
    CWorld* GetWorld() const { return PWorld; }
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
    CWorld* world = g_pMainGameComponent->GetWorld();
    C3DVector up(0.0f, 0.0f, 1.0f);
    C3DVector forward(1.0f, 0.0f, 0.0f);
    forward.Rotate(up, GFFloatRandom(1.0f, world->GetPRandomSeed()));
    const float tilt = 0.125f - GFFloatRandom(0.25f, world->GetPRandomSeed());
    up.Rotate(direction, tilt);
    forward.Rotate(direction, tilt);
    thing.PhysicsTC->SetRHSet(CRightHandedSet(up, forward));
}
}
