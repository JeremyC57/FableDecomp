// Behaviour test for CCreatureAction_NavToPos::FrameUpdate @ 0x00849BE0: failure and arrival move to the creature's own
// position and finish; a step moves to position + step and looks at it (priority 0x11) unless the player is Z-targeting;
// the base FrameUpdate always runs.
#include <stdio.h>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
enum ENavigatorReturn { NAV_FAILED = 0, NAV_ARRIVED = 1, NAV_MOVING = 2, NAV_OTHER = 3 };
struct CNavOutput { ENavigatorReturn NavResult; C3DVector MovementVector; };
struct CTest {
    C3DVector Pos;
    CNavOutput Output;
    bool ZTargeting;
    bool HasLook;
    int finishes, baseUpdates, looks, moves;
    long lookPriority;
    C3DVector moveTo, lookAt;
    float proximity;
    void SetMoveToPos(const C3DVector& p) { ++moves; moveTo = p; }
    void FinishAction() { ++finishes; }
    void LookAtPos(const C3DVector& p, long priority) { ++looks; lookAt = p; lookPriority = priority; }
    void BaseFrameUpdate() { ++baseUpdates; }
    CNavOutput GetNextNavPosOnRoute(float prox) { proximity = prox; return Output; }
    void FrameUpdate()
    {
        CNavOutput output = GetNextNavPosOnRoute(0.0f);
        if (output.NavResult == NAV_FAILED) {
            SetMoveToPos(Pos);
            FinishAction();
        } else if (output.NavResult == NAV_MOVING) {
            C3DVector position = Pos + output.MovementVector;
            SetMoveToPos(position);
            if (HasLook && !ZTargeting)
                LookAtPos(position, 0x11);
        } else if (output.NavResult == NAV_ARRIVED) {
            SetMoveToPos(Pos);
            FinishAction();
        }
        BaseFrameUpdate();
    }
};

static CTest Make(ENavigatorReturn r, bool z, bool look)
{
    CTest t;
    t.Pos = C3DVector(1, 2, 3);
    t.Output.NavResult = r;
    t.Output.MovementVector = C3DVector(10, 20, 30);
    t.ZTargeting = z; t.HasLook = look;
    t.finishes = t.baseUpdates = t.looks = t.moves = 0;
    t.lookPriority = 0;
    return t;
}

int main()
{
    int ok = 1;
    CTest a = Make(NAV_FAILED, false, true); a.FrameUpdate();
    ok &= a.moves == 1 && a.moveTo.X == 1 && a.finishes == 1 && a.looks == 0 && a.baseUpdates == 1;
    CTest b = Make(NAV_MOVING, false, true); b.FrameUpdate();
    ok &= b.moveTo.X == 11 && b.moveTo.Z == 33 && b.looks == 1 && b.lookPriority == 0x11 && b.lookAt.Y == 22 && b.finishes == 0;
    CTest c = Make(NAV_MOVING, true, true); c.FrameUpdate();
    ok &= c.looks == 0 && c.moves == 1;
    CTest d = Make(NAV_ARRIVED, false, true); d.FrameUpdate();
    ok &= d.finishes == 1 && d.moveTo.Y == 2;
    CTest e = Make(NAV_OTHER, false, true); e.FrameUpdate();
    ok &= e.moves == 0 && e.finishes == 0 && e.baseUpdates == 1;
    if (!ok) { printf("00849be0_TEST FAIL\n"); return 1; }
    printf("00849be0_TEST PASS\n");
    return 0;
}