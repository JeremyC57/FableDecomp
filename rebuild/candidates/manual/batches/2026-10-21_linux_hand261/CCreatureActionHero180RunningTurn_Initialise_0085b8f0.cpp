// CCreatureActionHero180RunningTurn::Initialise() -- animation ST_RUN_180_TURN; measure the turn from the creature's facing to
// RequiredFacing (clamped dot, sign from the cross product), base Initialise, then rotate the facing about Z by the
// animation's rotation less the turn (in revolutions) and set the creature's orientation to (up, rotated facing)
// Leaf: ?Initialise@CCreatureActionHero180RunningTurn
#include <math.h>
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    void Rotate(const C3DVector& axis, float angle);  // 0x00A13C60
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& forward) : Up(0.0f, 0.0f, 1.0f), Forward(forward) {}
};
struct CPhysicsBody {                                 // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;                    // slot 70 (+0x118)
    VF(71) VF(72) VF(73) VF(74)
    virtual void SetOrientation(const CRightHandedSet& set);   // slot 75 (+0x12c)
};
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsBody* PhysicsBody;                        // +0x60
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
};
struct CAnimationEntry {
    void GetRotationInAnimation(float* pRotation) const;   // 0x005DBFD0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x74 - 0x10];
    CAnimationEntry* PAnimEntry;                      // +0x74
    unsigned char _pad78[0x114 - 0x78];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
};
struct CCreatureActionHero180RunningTurn : CCombatActionBase {
    C3DVector RequiredFacing;                         // +0x114 (PDB +0x12C)
    void Initialise();
};

void CCreatureActionHero180RunningTurn::Initialise()
{
    {
        CCharString name("ST_RUN_180_TURN");
        SetAnimationName(name);
    }
    C3DVector facing = PCreature.GetPItem()->GetPhysics()->GetFacing();
    float dot = facing.Y * RequiredFacing.Y;
    dot += facing.Z * RequiredFacing.Z;
    dot += facing.X * RequiredFacing.X;
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    long sign = -1;
    if (!(facing.Y * RequiredFacing.X - facing.X * RequiredFacing.Y < 0.0f))
        sign = 1;
    CCombatActionBase::Initialise();
    float rotation;
    PAnimEntry->GetRotationInAnimation(&rotation);
    float turn = acos(dot) * 0.15915494f;
    float angle = rotation - turn * sign;
    C3DVector forward = PCreature.GetPItem()->GetPhysics()->GetFacing();
    forward.Rotate(C3DVector(0.0f, 0.0f, 1.0f), -angle);
    PCreature.GetPItem()->GetPhysics()->SetOrientation(CRightHandedSet(forward));
}
