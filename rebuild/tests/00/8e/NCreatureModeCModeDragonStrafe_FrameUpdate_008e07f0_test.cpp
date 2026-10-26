#include <cstdio>
// Behaviour test for NCreatureMode::CModeDragonStrafe::FrameUpdate (retail 0x008E07F0): after the base update the strafe
// timer counts down; at zero the dragon starts the strafe-out-of action, otherwise it moves along its facing by
// Speed / frames-per-second.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { C3DVector r; r.X = X * s; r.Y = Y * s; r.Z = Z * s; return r; }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
struct CPhysicsBody {
    C3DVector Facing;
    C3DVector Position;
    int sets;
    C3DVector GetFacing() { return Facing; }
    void SetPosition(const C3DVector& p) { Position = p; ++sets; }
};
class CCreatureActionBase { public: int Kind; };
class CThingCreatureBase { public:
    CPhysicsBody* PPhysics;
    int actions;
    const C3DVector& GetPosition() const { return PPhysics->Position; }
    bool SetCurrentAction(const CCreatureActionBase& action) { actions += action.Kind; return true; }
};
class CCreatureAction_DragonStrafeOutOf : public CCreatureActionBase { public:
    CCreatureAction_DragonStrafeOutOf(CThingCreatureBase* owner) { Kind = 1; (void)owner; }
};
static long g_FramesPerSecond = 4;
static int g_baseUpdates;
struct CModeBase {
    CThingCreatureBase* ParentThing;
    void FrameUpdate() { ++g_baseUpdates; }
};
struct CModeDragonStrafe : CModeBase {
    long Timer;
    float Speed;
    void FrameUpdate()
    {
        CModeBase::FrameUpdate();
        CThingCreatureBase* creature = ParentThing;
        if (--Timer <= 0) {
            creature->SetCurrentAction(CCreatureAction_DragonStrafeOutOf(creature));
            return;
        }
        float step = Speed / g_FramesPerSecond;
        C3DVector offset = creature->PPhysics->GetFacing() * step;
        C3DVector position = ParentThing->GetPosition() + offset;
        ParentThing->PPhysics->SetPosition(position);
    }
};

int main()
{
    CPhysicsBody body;
    body.Facing = C3DVector(1.0f, 0.0f, -0.5f);
    body.Position = C3DVector(10.0f, 20.0f, 30.0f);
    body.sets = 0;
    CThingCreatureBase dragon;
    dragon.PPhysics = &body; dragon.actions = 0;
    CModeDragonStrafe m;
    m.ParentThing = &dragon; m.Timer = 2; m.Speed = 8.0f;
    g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = body.sets == 1 && body.Position.X == 12.0f && body.Position.Y == 20.0f && body.Position.Z == 29.0f
        && dragon.actions == 0;
    m.FrameUpdate();
    pass = pass && body.sets == 1 && dragon.actions == 1 && m.Timer == 0 && g_baseUpdates == 2;
    if (!pass) { std::printf("008e07f0_TEST FAIL\n"); return 1; }
    std::printf("008e07f0_TEST PASS\n");
    return 0;
}