// CCreatureAction_SummonerLightningOrbInto::FrameUpdate() -- base FrameUpdate; while the target lives, turn the summoner's
// Grade: DIFFER(441v441) -- register swap (retail this in edi, PTarget address in esi) and the cross-product terms load the Y components where ours loads X (VC7.1 commutative-operand order unreachable from 6 source orders, accumulator and inline CrossZ forms)
// facing towards it in the ground plane by at most 1/36 of a revolution per frame (clamped dot, sign from the cross product)
// and set the summoner's orientation to (up, turned facing)
// Leaf: ?FrameUpdate@CCreatureAction_SummonerLightningOrbInto
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    void Normalise();                                 // 0x00A14440
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
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
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
    unsigned char _pad78[0xa8 - 0x78];
    CBaseIntelligentPointer PTarget;                  // +0xA8
    unsigned char _padb0[0x114 - 0xb0];
    void FrameUpdate();                               // 0x00859D80
};
struct CCreatureAction_SummonerLightningOrbInto : CCombatActionBase {
    void FrameUpdate();
};

void CCreatureAction_SummonerLightningOrbInto::FrameUpdate()
{
    CCombatActionBase::FrameUpdate();
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) {
        CThing* target = PTarget.GetPItem();
        const C3DVector& from = PCreature.GetPItem()->GetPosition();
        const C3DVector& to = target->GetPosition();
        C3DVector direction(to.X - from.X, to.Y - from.Y, 0.0f);
        direction.Normalise();
        C3DVector facing = PCreature.GetPItem()->GetPhysics()->GetFacing();
        float dot = direction.Y * facing.Y;
        dot += direction.X * facing.X;
        dot += direction.Z * facing.Z;
        if (dot < -1.0f)
            dot = -1.0f;
        else if (dot > 1.0f)
            dot = 1.0f;
        float angle = 0.027777778f;
        float turn = fabs(acos(dot) * 0.15915494f);
        if (0.027777778f > turn)
            angle = turn;
        if (facing.X * direction.Y - direction.X * facing.Y > 0.0f)
            angle *= -1.0f;
        facing.Rotate(C3DVector(0.0f, 0.0f, 1.0f), angle);
        PCreature.GetPItem()->GetPhysics()->SetOrientation(CRightHandedSet(facing));
    }
}
