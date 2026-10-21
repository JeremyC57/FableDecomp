// Behaviour test for CAIStateGroup_MinionReactToEnemy::Update (0x00937DA0): model copy, scripted stubs.
#include <cstdio>
#include <cmath>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    void Normalise() { float l = (float)std::sqrt(X * X + Y * Y + Z * Z); if (l > 0.0f) { X /= l; Y /= l; Z /= l; } }
};
struct CThing {
    unsigned char m_flags91;
    C3DVector pos;
    const C3DVector& GetPosition() const { return pos; }
};
struct CThingCreatureBase : CThing { };
struct CThingAICreature : CThingCreatureBase { };
static int g_attack;
static bool IsToAttackTarget(CThingAICreature*, CThingCreatureBase* target) { return g_attack != 0 && target != 0; }
inline float GetDistanceSquaredXY(const CThing* a, const CThing* b)
{
    const C3DVector& posB = b->GetPosition();
    const C3DVector& posA = a->GetPosition();
    float dx = posA.X - posB.X;
    float dy = posA.Y - posB.Y;
    return dx * dx + dy * dy;
}
struct CPointer {
    CThingCreatureBase* item;
    CThingCreatureBase* GetPItem() const { return item; }
};
inline bool IsThingAlive(const CPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }
struct CAIBrain { CThingAICreature* PCreature; };

static int g_move[16], g_mi, g_look[16], g_li, g_terminated;
static C3DVector g_lastMove;
static float g_lastProximity;
struct CAIStateGroup_MinionReactToEnemy {
    CAIBrain* PBrain;
    CPointer PEnemy;
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    void Terminate() { ++g_terminated; }
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool& arrived, long, bool, bool, bool, const CThing*, bool, float)
    {
        if (repeat != 1) return -1;
        g_lastMove = pos; g_lastProximity = proximity; arrived = false; return g_move[g_mi++];
    }
    long AIAction_LookAtThing(long repeat, const CPointer& thing, long frames, float)
    {
        if (repeat != 2 || frames != 0xb || thing.item != PEnemy.item) return -1;
        return g_look[g_li++];
    }
    void Update();
};

void CAIStateGroup_MinionReactToEnemy::Update()
{
    for (;;) {
        if (!IsThingAlive(PEnemy))
            break;
        if (!IsToAttackTarget(GetCreature(), PEnemy.GetPItem()))
            break;
        float distanceSquared = GetDistanceSquaredXY(GetCreature(), PEnemy.GetPItem());
        if (distanceSquared < 25.0f) {
            C3DVector away = GetCreature()->GetPosition() - PEnemy.GetPItem()->GetPosition();
            away.Normalise();
            away *= 2.4f;
            bool arrived;
            if (AIAction_MoveToPosProximity(1, GetCreature()->GetPosition() + away, 0.5f, arrived, 3, true, false, true, 0,
                                            true, 0.0f) != 2)
                break;
        }
        if (!IsThingAlive(PEnemy))
            break;
        long result = AIAction_LookAtThing(2, PEnemy, 0xb, 0.0027777f);
        if (result != 4 && result != 2)
            break;
    }
    Terminate();
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static void Reset() { g_mi = g_li = g_terminated = 0; g_attack = 1; for (int i = 0; i < 16; ++i) { g_move[i] = 2; g_look[i] = 4; } }

int main()
{
    int ok = 1;
    CThingAICreature me; me.m_flags91 = 0; me.pos = C3DVector(10.0f, 0.0f, 1.0f);
    CThingCreatureBase enemy; enemy.m_flags91 = 0; enemy.pos = C3DVector(7.0f, 0.0f, 50.0f);   // 3 units away in XY
    CAIBrain brain = { &me };
    CAIStateGroup_MinionReactToEnemy s; s.PBrain = &brain; s.PEnemy.item = &enemy;
    // close: back off 2.4 units along the (3D) away direction, then look; the look ends (3): terminate
    Reset(); g_look[1] = 3; s.Update();
    if (g_mi != 2 || g_li != 2 || g_terminated != 1 || g_lastProximity != 0.5f) ok = 0;
    {
        float dx = 3.0f, dz = -49.0f, l = (float)std::sqrt(dx * dx + dz * dz);
        if (!Near(g_lastMove.X, 10.0f + 2.4f * dx / l) || !Near(g_lastMove.Z, 1.0f + 2.4f * dz / l) || !Near(g_lastMove.Y, 0.0f)) ok = 0;
    }
    // far (6 units in XY): no move, just look until it fails
    enemy.pos = C3DVector(16.0f, 0.0f, 0.0f);
    Reset(); g_look[0] = 2; g_look[1] = 1; s.Update();
    if (g_mi != 0 || g_li != 2 || g_terminated != 1) ok = 0;
    // exactly 5 units: not closer than 5, no move
    enemy.pos = C3DVector(13.0f, 4.0f, 0.0f);
    Reset(); g_look[0] = 1; s.Update(); if (g_mi != 0 || g_li != 1) ok = 0;
    // close and the move fails: terminate without looking
    enemy.pos = C3DVector(11.0f, 1.0f, 0.0f);
    Reset(); g_move[0] = 3; s.Update(); if (g_mi != 1 || g_li != 0 || g_terminated != 1) ok = 0;
    // not a target any more / dead / gone
    Reset(); g_attack = 0; s.Update(); if (g_mi || g_li || g_terminated != 1) ok = 0;
    enemy.m_flags91 = 1; Reset(); s.Update(); if (g_mi || g_li || g_terminated != 1) ok = 0;
    s.PEnemy.item = 0; Reset(); s.Update(); if (g_mi || g_li || g_terminated != 1) ok = 0;
    if (!ok) { std::printf("00937da0_TEST FAIL\n"); return 1; }
    std::printf("00937da0_TEST PASS\n");
    return 0;
}
