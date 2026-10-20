// Behaviour test for 0x0096C880 CAIStateGroup_EatAtTable::WalkTo (model copy over stub actions).
#include <cstdio>
#include <math.h>
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

struct CRightHandedSet {
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};

struct CThingCreatureBase {
    long mode; C3DVector pos;
    long GetMovementMode() { return mode; }
    const C3DVector& GetPosition() const { return pos; }
};
struct CAIBrain { CThingCreatureBase* PCreature; };
static long g_moveType = -1; static int g_moveCalls = 0, g_lookCalls = 0; static int g_moveBusy = 0, g_lookBusy = 0;
static long g_moveResult = 2, g_lookResult = 2; static C3DVector g_lookAt(0, 0, 0); static float g_turn = 0;
struct CAIStateGroupBase {
    CAIBrain* PBrain;
    long AIAction_MoveToPos(long, C3DVector, bool*, long movementType, bool, bool, CThing*, bool, float)
    { ++g_moveCalls; g_moveType = movementType; return g_moveBusy-- > 0 ? 4 : g_moveResult; }
    long AIAction_LookAtPos(long, C3DVector pos, long, float turn)
    { ++g_lookCalls; g_lookAt = pos; g_turn = turn; return g_lookBusy-- > 0 ? 4 : g_lookResult; }
};
struct CAIStateGroup_EatAtTable : CAIStateGroupBase {
    bool WalkTo(C3DVector pos, CRightHandedSet axes);
};
bool CAIStateGroup_EatAtTable::WalkTo(C3DVector pos, CRightHandedSet axes)
{
    long movementType = 3;
    if (PBrain->PCreature->GetMovementMode() == 1 && (pos - PBrain->PCreature->GetPosition()).GetLength() > 8.0f)
        movementType = 5;
    bool reached;
    long result;
    do {
        result = AIAction_MoveToPos(2, pos, &reached, movementType, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        return false;
    float x = pos.X + axes.Forward.X;
    float y = pos.Y + axes.Forward.Y;
    float z = pos.Z + axes.Forward.Z;
    long i = 0;
    do {
        result = AIAction_LookAtPos(2, C3DVector(x, y, z), 0x11, 0.0027777f);
    } while (result == 4 || ++i < 2);
    return result == 2;
}

static int fail(int n) { std::printf("0096c880_TEST FAIL (%d)\n", n); return 1; }
int main()
{
    CThingCreatureBase creature; creature.mode = 1; creature.pos = C3DVector(0, 0, 0);
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_EatAtTable g; g.PBrain = &brain;
    CRightHandedSet axes; axes.Up = C3DVector(0, 0, 1); axes.Forward = C3DVector(0, 1, 0);
    g_moveBusy = 2;
    if (!g.WalkTo(C3DVector(10, 0, 0), axes) || g_moveType != 5 || g_moveCalls != 3 || g_lookCalls != 2) return fail(1);
    if (g_lookAt.X != 10 || g_lookAt.Y != 1 || g_lookAt.Z != 0 || g_turn != 0.0027777f) return fail(2);
    g_moveCalls = g_lookCalls = 0;
    if (!g.WalkTo(C3DVector(3, 0, 0), axes) || g_moveType != 3) return fail(3);    // near: walk
    creature.mode = 0;
    if (!g.WalkTo(C3DVector(30, 0, 0), axes) || g_moveType != 3) return fail(4);   // wrong mode: walk
    g_moveResult = 1; g_lookCalls = 0;
    if (g.WalkTo(C3DVector(30, 0, 0), axes) || g_lookCalls != 0) return fail(5);
    g_moveResult = 2; g_lookResult = 1;
    if (g.WalkTo(C3DVector(30, 0, 0), axes)) return fail(6);
    std::printf("0096c880_TEST PASS\n");
    return 0;
}
