// Behaviour test for CCreatureActionHero180RunningTurn::Initialise (0x0085B8F0): sets ST_RUN_180_TURN, measures the turn
// from the facing to RequiredFacing (clamped dot, sign from the cross product), runs the base Initialise, rotates the
// facing about Z by -(rotation - turn*sign) and sets the orientation (up 0,0,1).
#include <cstdio>
#include <cstring>
#include <cmath>

static char g_anim[64]; static int g_baseInit = 0;
static float g_rotAngle = 0; static float g_rotAxisZ = 0;
struct CCharString { const char* s; CCharString(const char* t) : s(t) {} };
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Rotate(const C3DVector& axis, float angle) { g_rotAxisZ = axis.Z; g_rotAngle = angle; X = X + angle; }
};
struct CRightHandedSet {
    C3DVector Up, Forward;
    CRightHandedSet(const C3DVector& f) : Up(0.0f, 0.0f, 1.0f), Forward(f) {}
};
static C3DVector g_setUp, g_setFwd; static int g_sets = 0;
struct CPhysicsBody {
    C3DVector Facing;
    C3DVector GetFacing() const { return Facing; }
    void SetOrientation(const CRightHandedSet& s) { g_setUp = s.Up; g_setFwd = s.Forward; ++g_sets; }
};
struct CThing { CPhysicsBody* PhysicsBody; CPhysicsBody* GetPhysics() const { return PhysicsBody; } };
struct CAnimationEntry { float Rot; void GetRotationInAnimation(float* r) const { *r = Rot; } };
struct CPtr { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CCombatActionBase {
    CPtr PCreature; CAnimationEntry* PAnimEntry;
    void SetAnimationName(const CCharString& n) { std::strcpy(g_anim, n.s); }
    void Initialise() { ++g_baseInit; }
};
struct CCreatureActionHero180RunningTurn : CCombatActionBase { C3DVector RequiredFacing; void Initialise(); };

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
    float turn = (float)acos(dot) * 0.15915494f;
    float angle = rotation - turn * sign;
    C3DVector forward = PCreature.GetPItem()->GetPhysics()->GetFacing();
    forward.Rotate(C3DVector(0.0f, 0.0f, 1.0f), -angle);
    PCreature.GetPItem()->GetPhysics()->SetOrientation(CRightHandedSet(forward));
}

static int near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    int ok = 1;
    CPhysicsBody body; body.Facing = C3DVector(0.0f, 1.0f, 0.0f);
    CThing thing; thing.PhysicsBody = &body;
    CAnimationEntry entry; entry.Rot = 0.5f;
    CCreatureActionHero180RunningTurn a; a.PCreature.m_pItem = &thing; a.PAnimEntry = &entry;
    // opposite facing: dot -1 -> turn 0.5 rev; cross = 1*0 - 0*(-1) = 0 -> sign +1; angle = 0.5 - 0.5 = 0
    a.RequiredFacing = C3DVector(0.0f, -1.0f, 0.0f);
    a.Initialise();
    if (std::strcmp(g_anim, "ST_RUN_180_TURN") != 0 || g_baseInit != 1 || g_sets != 1) ok = 0;
    if (!near(g_rotAngle, 0.0f) || !near(g_rotAxisZ, 1.0f)) ok = 0;
    if (!near(g_setUp.Z, 1.0f) || !near(g_setUp.X, 0.0f) || !near(g_setFwd.Y, 1.0f)) ok = 0;
    // perpendicular facing to the right: dot 0 -> turn 0.25; cross = 1*1 - 0*0 = 1 -> sign +1; angle = 0.5-0.25 -> -0.25
    a.RequiredFacing = C3DVector(1.0f, 0.0f, 0.0f);
    a.Initialise();
    if (!near(g_rotAngle, -0.25f)) ok = 0;
    // to the left: cross = 1*(-1) - 0 = -1 < 0 -> sign -1; angle = 0.5 + 0.25 -> -0.75
    a.RequiredFacing = C3DVector(-1.0f, 0.0f, 0.0f);
    a.Initialise();
    if (!near(g_rotAngle, -0.75f)) ok = 0;
    // dot above 1 (unnormalised) clamps to 1 -> turn 0; angle = 0.5 -> -0.5
    a.RequiredFacing = C3DVector(0.0f, 3.0f, 0.0f);
    a.Initialise();
    if (!near(g_rotAngle, -0.5f) || !near(g_setFwd.X, -0.5f)) ok = 0;
    std::printf("0085b8f0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}