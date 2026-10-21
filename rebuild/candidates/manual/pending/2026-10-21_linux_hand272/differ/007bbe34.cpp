// CTCShotDefinitionSpreadShotProjectile::InitMovement() -- aim at a live target: take the target's facing offset scaled by 1.5, rotate it by the shared spread angle (which advances by 0.333 and wraps above 1), drop that point onto the ground, randomise the direction from the shot by the def's spread, then face the shot along it (side vector = its horizontal perpendicular crossed with it) and launch it at the def's speed
// Grade: DIFFER(439v464) -- first pass, logic in place; gaps: retail keeps the target in edi with only a by-reference temp copy in [ebp-4] for IS_THING_ALIVE (ours re-reads the spilled local), and gives every vector its own frame slot (sub esp,0x5c: rh, facing result, velocity, side, aim, dir) where ours shares (0x44); a declared-up-front ordering (probe b) shifts slots but adds movsd copies (480v464)
// Retail: Fable.exe @ 0x007BBE34, 464 bytes (frame-pointer, `leave ; ret`; unaligned entry = /Os TU).
// Callees: GetPItem 0x00A01B50, IS_THING_ALIVE 0x00436380 (__fastcall), target frame vslot 70 (+0x118), C3DVector::RotateInPlace 0x00A14040,
//   CThing::GetPosition 0x004C73D0, CTCBase::GetWorldMap 0x00686D30, CWorldMap::GetHeightZBelowPos 0x0063E480, __ftol2, CTCBase::GetWorld 0x00686A70,
//   CWorld::GetWorldSeed 0x0049D810, RandomiseShotDirection 0x007BEBE0 (__fastcall), C3DVector::Normalise 0x00A14440, CRightHandedSet ctor 0x00487050,
//   physics vslot 32 (+0x80) SetRHSet, CTCPhysicsBase::ApplyAcceleration 0x006B0210, physics vslot 38 (+0x98)
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void RotateInPlace(float angle);                    // 0x00A14040
    void Normalise();                                   // 0x00A14440
};
inline C3DVector operator*(const C3DVector& v, float s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector Cross(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
struct CRightHandedSet {
    C3DVector Forward;
    C3DVector Up;
    CRightHandedSet(const C3DVector& side, const C3DVector& forward);   // 0x00487050
};
struct CFrameSource {
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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing();                      // vslot 70 (+0x118)
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void SetRHSet(const CRightHandedSet& rh);   // vslot 32 (+0x80)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void SetFrozen(bool frozen);                // vslot 38 (+0x98)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Position;                                 // +0x0c
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x60 - 0x04];
    CFrameSource* Frame;                                // +0x60 (physics component of a shot)
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x00436380 (CThing*)
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CWorldMap { float GetHeightZBelowPos(const C3DVector& pos); };   // 0x0063E480
struct CWorld { unsigned long* GetWorldSeed(); };       // 0x0049D810
C3DVector __fastcall RandomiseShotDirection(const C3DVector& direction, long spread, unsigned long* seed);   // 0x007BEBE0
extern float g_SpreadShotAngle;                         // 0x013BAE7C
extern long g_FramesPerSecond;                          // 0x01375550
struct CShotDef { unsigned char _pad[0x3c]; float Speed; };   // +0x3c
struct CTCShotDefinitionSpreadShotProjectile {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x20 - 0x08];
    CBaseIntelligentPointer PTarget;                    // +0x20
    unsigned char _pad28[0x38 - 0x28];
    CShotDef* Def;                                      // +0x38
    unsigned char _pad3c[0x40 - 0x3c];
    float Spread;                                       // +0x40
    CWorldMap* GetWorldMap();                           // 0x00686D30
    CWorld* GetWorld();                                 // 0x00686A70
    void InitMovement();
};

void CTCShotDefinitionSpreadShotProjectile::InitMovement()
{
    CThing* target = PTarget.GetPItem();
    if (!IS_THING_ALIVE(target))
        return;
    CTCPhysicsBase* physics = (CTCPhysicsBase*)ParentThing->Frame;
    C3DVector aim = target->Frame->GetFacing() * 1.5f;
    aim.RotateInPlace(g_SpreadShotAngle);
    const C3DVector& targetPos = target->GetPosition();
    aim.X += targetPos.X;
    aim.Y += targetPos.Y;
    aim.Z += targetPos.Z;
    aim.Z = GetWorldMap()->GetHeightZBelowPos(aim);
    g_SpreadShotAngle += 0.333f;
    if (g_SpreadShotAngle > 1.0f)
        g_SpreadShotAngle -= 1.0f;
    C3DVector dir = aim - physics->Position;
    long spread = (long)Spread;
    dir = RandomiseShotDirection(dir, spread, GetWorld()->GetWorldSeed());
    dir.Normalise();
    C3DVector side = Cross(C3DVector(dir.Y, -dir.X, 0.0f), dir);
    side.Normalise();
    physics->SetRHSet(CRightHandedSet(side, dir));
    physics->ApplyAcceleration(dir * (Def->Speed / (float)g_FramesPerSecond));
    physics->SetFrozen(false);
}
