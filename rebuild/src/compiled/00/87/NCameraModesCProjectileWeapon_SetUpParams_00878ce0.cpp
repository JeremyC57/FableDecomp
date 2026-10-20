// NCameraModes::CProjectileWeapon::SetUpParams() -- reset the projectile-weapon camera's orbit from its def (initial phi offset in turns, initial height), clear the deltas and build the camera axes around the observed thing's flattened facing
// Grade: MATCH (retry 9) -- the three ZAxis constants are stored through an inline `Set(0, 0, 1)` member: three separate
// member assignments (any order, chained, or before/after the other axes) get scheduled into the YAxis copy by VC7.1,
// while the inline setter's stores stay together ahead of the YAxis `lea ecx,[esi+0B5h]` as in retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00878CE0, length 208 bytes, plain `ret`.
// Callees: NCameraModes::CBase::GetObservedThing() const 0x0084F1B0, the thing's physics component GetFacing (slot 70,
// +0x118, C3DVector by hidden pointer), C3DVector::Normalise 0x00A14440; the float constant 1/360 0x01238E00.
// Layout (retail = Ego_r.pdb CProjectileWeapon - 0x24, byte-packed axes as the landed GetRotatedPosition 0x00878A80):
// CBase PCameraModeDef +0x44, XAxis +0xA9, YAxis +0xB5, ZAxis +0xC1, Phi +0xD0, HeightParam +0xD4, DeltaPhi +0xD8,
// DeltaHeightParam +0xDC, LockDeltaPhi +0xE0, LockDeltaHeightParam +0xE4, DeltaLockDeltaPhi +0xE8,
// DeltaLockDeltaHeightParam +0xEC; CCameraModeDef ProjectileCameraInitialPhiOffsetForCamera +0xA0 /
// ProjectileCameraInitialHeightParamForCamera +0xA4 (PDB +0xA4/+0xA8); CThing PhysicsTC +0x60.
// Shape: the height is a plain dword copy, the phi offset scaled by the 1/360 constant; the six delta clears are one
// chained assignment (stored right to left), which keeps the zero in edi for the facing's Z and ZAxis' zero members;
// the physics component goes through an inline accessor (straight into ecx). The facing is a frame local; YAxis copies
// it and XAxis copies a (Y, -X, 0) temporary through pointer registers (C3DVector has a user copy ctor and no
// operator=); ZAxis is set through the inline Set(x, y, z).
#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& Normalise();                             // 0x00A14440
    void Set(float x, float y, float z) { X = x; Y = y; Z = z; }
};
#pragma pack(pop)

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing();                      // slot 70 (+0x118)
};
struct CThingGameObject {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};

struct CCameraModeDef {
    unsigned char _pad00[0xa0];
    float         ProjectileCameraInitialPhiOffsetForCamera;    // +0xa0
    float         ProjectileCameraInitialHeightParamForCamera;  // +0xa4
};
template <class T> struct CDefPointer {
    T* Object;
};

namespace NCameraModes {
#pragma pack(push, 1)
struct CBase {
    virtual void v00();
    unsigned char _pad04[0x44 - 0x04];
    CDefPointer<const CCameraModeDef> PCameraModeDef;   // +0x44
    const CThingGameObject* GetObservedThing() const;   // 0x0084F1B0
};
struct CProjectileWeapon : CBase {                      // vtable 0x01277804
    unsigned char _pad48[0xa9 - 0x48];
    C3DVector     XAxis;                                // +0xa9
    C3DVector     YAxis;                                // +0xb5
    C3DVector     ZAxis;                                // +0xc1
    unsigned char _padcd[0xd0 - 0xcd];
    float         Phi;                                  // +0xd0
    float         HeightParam;                          // +0xd4
    float         DeltaPhi;                             // +0xd8
    float         DeltaHeightParam;                     // +0xdc
    float         LockDeltaPhi;                         // +0xe0
    float         LockDeltaHeightParam;                 // +0xe4
    float         DeltaLockDeltaPhi;                    // +0xe8
    float         DeltaLockDeltaHeightParam;            // +0xec
    void SetUpParams();
};
#pragma pack(pop)
}

static const float s_oneOver360 = 1.0f / 360.0f;       // 0x01238E00

void NCameraModes::CProjectileWeapon::SetUpParams()
{
    const CCameraModeDef* def = PCameraModeDef.Object;
    HeightParam = def->ProjectileCameraInitialHeightParamForCamera;
    Phi = def->ProjectileCameraInitialPhiOffsetForCamera * s_oneOver360;
    DeltaHeightParam = DeltaPhi = DeltaLockDeltaHeightParam = DeltaLockDeltaPhi = LockDeltaHeightParam = LockDeltaPhi = 0.0f;
    C3DVector facing = GetObservedThing()->GetPhysicsTC()->GetFacing();
    facing.Z = 0.0f;
    facing.Normalise();
    ZAxis.Set(0.0f, 0.0f, 1.0f);
    YAxis = facing;
    XAxis = C3DVector(facing.Y, -facing.X, 0.0f);
}