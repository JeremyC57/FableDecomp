// Grade: DIFFER(342v342): 6 diff lines (was 9) -- NAMED_SCALE plus the landed 0x007A1870 (float)(double) CrossProduct; residue is the frame (0x50 v 0x5c)
// CTCShotDefinitionNymphFun::InitMovement() -- aim the shot at the live target's ground point (0.5 above its position, dropped to the terrain height), randomise the direction by the accuracy, orient the physics along it with a level side axis, and launch it at the def's speed per frame
// Leaf: InitMovement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BC004, 342 bytes (plain `ret`; size TU with frame: optimize("y", off) + ("s", on)).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; IS_THING_ALIVE<CThing*> 0x00436380 (out-of-line `__fastcall`, by
//          reference); CThing::GetPosition 0x004C73D0; CTCBase world-map accessor 0x00686D30 and CWorldMap::GetHeightZBelowPos
//          0x0063E480; __ftol2 0x00BFEA70; CTCBase::GetWorld() 0x00686A70 and the random-seed accessor 0x0049D810;
//          RandomiseShotDirection 0x007BEBE0 (__fastcall, C3DVector by value); C3DVector::Normalise 0x00A14440 (x2);
//          CRightHandedSet(const C3DVector&, const C3DVector&) 0x00487050; the physics' virtual slot 32 (+0x80, set RH set) and
//          slot 38 (+0x98); CTCPhysicsBase velocity setter 0x006B0210 (descriptive names); g_framesPerSecond 0x01375550.
// Layout: retail CTCShotDefinitionBase = PDB - 4 past HitThingsList: PTargetThing +0x20, PShotDef +0x38, AccuracyPercentage
//          +0x40; the shot def's speed +0x3c; CTCPhysicsBase::Position +0x0c; CThing physics +0x60 (descriptive).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    __forceinline C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    void Normalise();                               // 0x00A14440
};
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    const float* pa = &a.X; const float* pb = &b.X;
    return C3DVector((float)(double)(pb[2] * pa[1] - pa[2] * pb[1]), (float)(double)(*(&b.Y - 1) * pa[2] - pa[0] * pb[2]), (float)(double)(pb[1] * pa[0] - pa[1] * *(&b.Y - 1)));
}
struct CRightHandedSet {
    C3DVector Forward;
    C3DVector Up;
    CRightHandedSet(const C3DVector& a, const C3DVector& b);   // 0x00487050
};
C3DVector __fastcall RandomiseShotDirection(const C3DVector& dir, long accuracy, unsigned long* seed);   // 0x007BEBE0
extern long g_framesPerSecond;                      // 0x01375550
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void SetRHSet(const CRightHandedSet& set);   // slot 32 (+0x80)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void SetMoving(bool moving);            // slot 38 (+0x98, descriptive)
    long      _04;
    long      _08;
    C3DVector Position;                             // +0x0c
    void SetVelocity(const C3DVector& velocity);    // 0x006B0210
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* Physics;                        // +0x60
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);   // 0x00436380
struct CBaseIntelligentPointer {
    unsigned char _d[8];
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CWorldMap {
    float GetHeightZBelowPos(const C3DVector& pos) const;   // 0x0063E480
};
struct CWorld {
    unsigned long* GetRandomSeed();                 // 0x0049D810
};
struct CThingShotDef {
    unsigned char _pad00[0x3c];
    float         Speed;                            // +0x3c (descriptive)
};
class CTCShotDefinitionNymphFun {
public:
    virtual void v00();
    CThing*                 ParentThing;            // +0x04
    unsigned char           _pad08[0x20 - 0x08];
    CBaseIntelligentPointer PTargetThing;           // +0x20
    unsigned char           _pad28[0x38 - 0x28];
    const CThingShotDef*    PShotDef;               // +0x38
    long                    _3c;
    float                   AccuracyPercentage;     // +0x40
    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)
    CWorldMap* GetWorldMap();                       // 0x00686D30 (CTCBase)
    void InitMovement();
};

void CTCShotDefinitionNymphFun::InitMovement()
{
    CThing* target = PTargetThing.GetPItem();
    if (IS_THING_ALIVE(target)) {
        CTCPhysicsBase* physics = ParentThing->Physics;
        const C3DVector& targetPos = target->GetPosition();
        C3DVector aim;
        aim.X = targetPos.X;
        aim.Y = targetPos.Y;
        aim.Z = targetPos.Z + 0.5f;
        aim.Z = GetWorldMap()->GetHeightZBelowPos(aim);
        C3DVector dir = aim - physics->Position;
        long accuracy = (long)AccuracyPercentage;
        dir = RandomiseShotDirection(dir, accuracy, GetWorld()->GetRandomSeed());
        dir.Normalise();
        C3DVector side = CrossProduct(C3DVector(dir.Y, -dir.X, 0.0f), dir);
        side.Normalise();
        physics->SetRHSet(CRightHandedSet(side, dir));
        float speed = PShotDef->Speed / g_framesPerSecond;
        C3DVector velocity = dir * speed;
        physics->SetVelocity(velocity);
        physics->SetMoving(false);
    }
}
