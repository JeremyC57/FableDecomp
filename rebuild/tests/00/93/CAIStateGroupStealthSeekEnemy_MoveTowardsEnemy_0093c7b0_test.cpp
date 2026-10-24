// Behaviour test for 0x0093C7B0 CAIStateGroup_StealthSeekEnemy::MoveTowardsEnemy: re-issues the move while it is in
// progress, terminates when the seek stops being runnable or the move fails, and leaves the group alone on arrival.
#include <cstdio>

class CThing;
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CTCPerceiveThing {
public:
    C3DVector LastPerceivedPos;
    int runnableFor;                            // test knob: IsStillRunnable answers true this many times
    CTCPerceiveThing() : LastPerceivedPos(1.0f, 2.0f, 3.0f), runnableFor(0) { }
};

struct CAIStateGroupBase {
    int terminated;
    int moves;
    const long* script;                         // test knob: the move results in order
    float lastRadius;
    C3DVector lastPos;
    CAIStateGroupBase() : terminated(0), moves(0), script(0), lastRadius(0.0f), lastPos(0.0f, 0.0f, 0.0f) { }
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e)
    {
        (void)repeat; (void)reached; (void)movementType; (void)a; (void)b; (void)c; (void)leader; (void)d; (void)e;
        lastRadius = radius;
        lastPos = pos;
        return script[moves++];
    }
    void Terminate() { ++terminated; }
};

struct CAIStateGroup_StealthSeekEnemy : CAIStateGroupBase {
    mutable int runnableChecks;
    CAIStateGroup_StealthSeekEnemy() : runnableChecks(0) { }
    bool IsStillRunnable(const CTCPerceiveThing& perceive) const { return runnableChecks++ < perceive.runnableFor; }
    void MoveTowardsEnemy(const CTCPerceiveThing* perceive);
};

void CAIStateGroup_StealthSeekEnemy::MoveTowardsEnemy(const CTCPerceiveThing* perceive)
{
    bool arrived;
    long result;
    do {
        if (!IsStillRunnable(*perceive)) {
            Terminate();
            return;
        }
        C3DVector pos = perceive->LastPerceivedPos;
        result = AIAction_MoveToPosProximity(2, pos, 1.5f, &arrived, 3, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    if (result == 1)
        Terminate();
}

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    int ok = 1;
    static const long arrive[] = { 4, 4, 2 };
    static const long fail[] = { 4, 1 };
    static const long busy[] = { 4, 4, 4, 4 };
    CTCPerceiveThing perceive;
    {
        CAIStateGroup_StealthSeekEnemy g;                  // not runnable at all
        g.script = arrive;
        g.MoveTowardsEnemy(&perceive);
        if (g.terminated != 1 || g.moves != 0) ok = 0;
    }
    {
        CAIStateGroup_StealthSeekEnemy g;                  // arrives on the third move
        perceive.runnableFor = 10;
        g.script = arrive;
        g.MoveTowardsEnemy(&perceive);
        if (g.terminated != 0 || g.moves != 3) ok = 0;
        if (!Near(g.lastRadius, 1.5f) || !Near(g.lastPos.Y, 2.0f) || !Near(g.lastPos.Z, 3.0f)) ok = 0;
    }
    {
        CAIStateGroup_StealthSeekEnemy g;                  // the move fails
        g.script = fail;
        g.MoveTowardsEnemy(&perceive);
        if (g.terminated != 1 || g.moves != 2) ok = 0;
    }
    {
        CAIStateGroup_StealthSeekEnemy g;                  // stops being runnable mid-walk
        perceive.runnableFor = 2;
        g.script = busy;
        g.MoveTowardsEnemy(&perceive);
        if (g.terminated != 1 || g.moves != 2) ok = 0;
    }
    if (!ok) { std::printf("0093c7b0_TEST FAIL\n"); return 1; }
    std::printf("0093c7b0_TEST PASS\n");
    return 0;
}