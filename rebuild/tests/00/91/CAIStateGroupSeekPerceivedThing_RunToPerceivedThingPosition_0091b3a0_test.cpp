// Behaviour test for 0x0091B3A0 CAIStateGroup_SeekPerceivedThing::RunToPerceivedThingPosition: the same loop logic through a
// standalone copy of the model (scripted move results, a distance-based IsInRange, counters for the calls made).
#include <cstdio>

enum EAIActionReturn { AI_ACTION_NULL = 0, AI_ACTION_TERMINATE_PROCESS = 1, AI_ACTION_FINISHED = 2, AI_ACTION_FAILED = 3, AI_ACTION_IN_PROGRESS = 4 };

class C3DVector { public: float X, Y, Z; };

static C3DVector Vec(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }

class CThing { public:
    C3DVector Pos;
    const C3DVector& GetPos() const { return Pos; }
};

class CTCPerceiveThing { public:
    const CThing* PerceivedThing;
    float AwarenessTimer;
    C3DVector LastPerceivedPos;
    bool LineOfSight;
    bool IsAwareOfThing() const { return AwarenessTimer != 0.0f; }
    bool IsThingLineOfSight() const { return LineOfSight; }
    const CThing* GetPerceivedThing() const { return PerceivedThing; }
};

struct CCreature {
    C3DVector Pos;
    bool IsInRange(const C3DVector& pos, float range) const
    {
        float dx = pos.X - Pos.X, dy = pos.Y - Pos.Y, dz = pos.Z - Pos.Z;
        return dx * dx + dy * dy + dz * dz <= range * range;
    }
};

struct CStateGroup {
    CCreature* Creature;
    CTCPerceiveThing* Perceive;
    // script
    EAIActionReturn MoveResults[8];
    int MoveCount;
    int LoseAwarenessAtMove;        // the move call (1-based) that zeroes the awareness timer; 0 = never
    int MoveTargetAtMove;           // the move call that relocates LastPerceivedPos far away; 0 = never
    bool StopAfterFirstTest;
    int StoppingTests;
    int PlayCount;
    float LastMoveProximity;
    C3DVector LastMovePos;

    bool IsStopping() { ++StoppingTests; return StopAfterFirstTest; }
    EAIActionReturn AIAction_MoveToPosProximity(int repeat, C3DVector pos, float proximity, bool& reached, int movement)
    {
        (void)repeat; (void)movement;
        reached = true;
        LastMovePos = pos;
        LastMoveProximity = proximity;
        ++MoveCount;
        if (MoveCount == LoseAwarenessAtMove)
            Perceive->AwarenessTimer = 0.0f;
        if (MoveCount == MoveTargetAtMove)
            Perceive->LastPerceivedPos = Vec(1000.0f, 1000.0f, 0.0f);
        return MoveResults[MoveCount - 1];
    }
    void PlayWhatsThat() { ++PlayCount; }

    void RunToPerceivedThingPosition()
    {
        CTCPerceiveThing* perceive = Perceive;
        while (perceive->IsAwareOfThing() && !IsStopping()) {
            EAIActionReturn ret;
            do {
                if (!perceive->IsAwareOfThing())
                    break;
                C3DVector pos = perceive->LastPerceivedPos;
                bool nav_route_calculated;
                ret = AIAction_MoveToPosProximity(2, pos, 0.5f, nav_route_calculated, 5);
                if (Creature->IsInRange(pos, 3.5f)
                    && !Creature->IsInRange(perceive->GetPerceivedThing()->GetPos(), 3.5f)
                    && !perceive->IsThingLineOfSight()
                    && Creature->IsInRange(perceive->LastPerceivedPos, 3.5f))
                    PlayWhatsThat();
            } while (ret == AI_ACTION_IN_PROGRESS);
        }
    }
};

static void Setup(CStateGroup& g, CCreature& c, CTCPerceiveThing& p, CThing& t)
{
    c.Pos = Vec(0.0f, 0.0f, 0.0f);
    t.Pos = Vec(50.0f, 0.0f, 0.0f);                 // the thing itself is far away
    p.PerceivedThing = &t;
    p.AwarenessTimer = 3.0f;
    p.LastPerceivedPos = Vec(1.0f, 2.0f, 0.0f);     // within 3.5 of the creature
    p.LineOfSight = false;
    g.Creature = &c; g.Perceive = &p;
    for (int i = 0; i < 8; ++i) g.MoveResults[i] = AI_ACTION_FINISHED;
    g.MoveCount = 0; g.LoseAwarenessAtMove = 0; g.MoveTargetAtMove = 0;
    g.StopAfterFirstTest = false; g.StoppingTests = 0; g.PlayCount = 0;
    g.LastMoveProximity = 0.0f; g.LastMovePos = Vec(0.0f, 0.0f, 0.0f);
}

int main()
{
    bool ok = true;
    CStateGroup g; CCreature c; CTCPerceiveThing p; CThing t;

    // 1. not aware: nothing happens, IsStopping is not even asked
    Setup(g, c, p, t);
    p.AwarenessTimer = 0.0f;
    g.RunToPerceivedThingPosition();
    if (g.MoveCount != 0 || g.PlayCount != 0 || g.StoppingTests != 0) ok = false;

    // 2. aware but stopping: one IsStopping test, no move
    Setup(g, c, p, t);
    g.StopAfterFirstTest = true;
    g.RunToPerceivedThingPosition();
    if (g.MoveCount != 0 || g.PlayCount != 0 || g.StoppingTests != 1) ok = false;

    // 3. two in-progress moves, a finished one, then awareness is lost on the 4th: the inner loop repeats without asking
    //    IsStopping while in progress (moves 1-3 under one test), a finished move goes back to the outer test.
    Setup(g, c, p, t);
    g.MoveResults[0] = AI_ACTION_IN_PROGRESS; g.MoveResults[1] = AI_ACTION_IN_PROGRESS; g.MoveResults[2] = AI_ACTION_FINISHED;
    g.MoveResults[3] = AI_ACTION_IN_PROGRESS;
    g.LoseAwarenessAtMove = 4;
    g.RunToPerceivedThingPosition();
    if (g.MoveCount != 4 || g.StoppingTests != 2 || g.PlayCount != 4) ok = false;
    if (g.LastMoveProximity != 0.5f || g.LastMovePos.X != 1.0f || g.LastMovePos.Y != 2.0f || g.LastMovePos.Z != 0.0f) ok = false;

    // 4. line of sight on the thing: no WHATS_THAT
    Setup(g, c, p, t);
    p.LineOfSight = true;
    g.LoseAwarenessAtMove = 2;
    g.RunToPerceivedThingPosition();
    if (g.MoveCount != 2 || g.PlayCount != 0) ok = false;

    // 5. the thing itself is in range: no WHATS_THAT
    Setup(g, c, p, t);
    t.Pos = Vec(0.5f, 0.5f, 0.0f);
    g.LoseAwarenessAtMove = 1;
    g.RunToPerceivedThingPosition();
    if (g.MoveCount != 1 || g.PlayCount != 0) ok = false;

    // 6. the creature has not reached the last perceived position: no WHATS_THAT
    Setup(g, c, p, t);
    p.LastPerceivedPos = Vec(10.0f, 0.0f, 0.0f);
    g.LoseAwarenessAtMove = 1;
    g.RunToPerceivedThingPosition();
    if (g.MoveCount != 1 || g.PlayCount != 0) ok = false;

    // 7. the last perceived position moves away during the move: the first range test uses the copy (still in range), the last
    //    one reads the live member (now out of range) -> no WHATS_THAT on that pass; the next pass moves to the new position.
    Setup(g, c, p, t);
    g.MoveTargetAtMove = 1;
    g.LoseAwarenessAtMove = 2;
    g.RunToPerceivedThingPosition();
    if (g.MoveCount != 2 || g.PlayCount != 0 || g.LastMovePos.X != 1000.0f) ok = false;

    if (!ok) { std::printf("0091b3a0_TEST FAIL\n"); return 1; }
    std::printf("0091b3a0_TEST PASS\n");
    return 0;
}