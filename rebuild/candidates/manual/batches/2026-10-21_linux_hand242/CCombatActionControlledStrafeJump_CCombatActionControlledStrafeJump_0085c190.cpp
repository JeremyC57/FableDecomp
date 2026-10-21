// CCombatAction_ControlledStrafeJump::CCombatAction_ControlledStrafeJump(CThingCreatureBase*, CThing*, const C3DVector&, const C3DVector&) -- a strafe-jump combat action (code 150, no sounds) that keeps flattened copies of the required and the original facing: each is normalised, or replaced by the owner's current facing when it is (nearly) zero
// Leaf: ??0CCombatAction_ControlledStrafeJump
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085C190, 337 bytes (352 with padding). `ret 10h`; vtable 0x01276974.
// Callees: CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) 0x00857F30, C3DVector::Normalise
//          0x00A14440 (catalogued C3DVector::GetScaled), the owner's physics body (+0x60) virtual slot 70 (+0x118,
//          GetFacing, C3DVector by hidden pointer). Float pool 0x0129BA3C (1e-4f, read as a variable).
//
// Shape: the body assigns RollMode, then the two facings (implicit copies through `lea` + eax: an init list
// would put the byte store after them); both Z components are cleared in the body (OriginalFacing first); each facing is
// `if (v.GetLengthSquared() > eps * eps) v.Normalise(); else v = owner->PhysicsBody->GetFacing();`.
// Layout: local model. engine/CCombatAction_ControlledStrafeJump.h is the Ego_r PDB view (RequiredFacing +0x12c,
// OriginalFacing +0x138, AddedRollMode +0x144 over a 0x12c-byte CCombatActionBase); retail's CCombatActionBase is
// 0x114 bytes (the landed CCombatAction ctors, e.g. 0x008C94D0), so the members sit 0x18 lower: RequiredFacing
// +0x114, OriginalFacing +0x120, AddedRollMode +0x12c.
struct CThing;
extern float g_Epsilon;                     // 0x0129BA3C (1e-4f)

struct C3DVector {
    float X, Y, Z;
    void Normalise();                       // 0x00A14440
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CSoundPair {                         // the zeroed two-dword sound argument of CCombatActionBase
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CPhysicsBody {                       // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();          // slot 70 (+0x118)
};
struct CThingCreatureBase {
    unsigned char _pad00[0x60];
    CPhysicsBody* PhysicsBody;              // +0x60
};
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x110];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};
struct CCombatAction_ControlledStrafeJump : CCombatActionBase {    // vtable 0x01276974
    C3DVector RequiredFacing;               // +0x114
    C3DVector OriginalFacing;               // +0x120
    bool      AddedRollMode;                // +0x12c
    virtual void v00();
    CCombatAction_ControlledStrafeJump(CThingCreatureBase* owner, CThing* target, const C3DVector& requiredFacing,
                                       const C3DVector& originalFacing);
};

CCombatAction_ControlledStrafeJump::CCombatAction_ControlledStrafeJump(CThingCreatureBase* owner, CThing* target,
                                                                       const C3DVector& requiredFacing, const C3DVector& originalFacing)
    : CCombatActionBase(owner, target, 150, CSoundPair())
{
    AddedRollMode = false;
    RequiredFacing = requiredFacing;
    OriginalFacing = originalFacing;
    OriginalFacing.Z = 0.0f;
    RequiredFacing.Z = 0.0f;
    if (RequiredFacing.GetLengthSquared() > g_Epsilon * g_Epsilon)
        RequiredFacing.Normalise();
    else
        RequiredFacing = owner->PhysicsBody->GetFacing();
    if (OriginalFacing.GetLengthSquared() > g_Epsilon * g_Epsilon)
        OriginalFacing.Normalise();
    else
        OriginalFacing = owner->PhysicsBody->GetFacing();
}
