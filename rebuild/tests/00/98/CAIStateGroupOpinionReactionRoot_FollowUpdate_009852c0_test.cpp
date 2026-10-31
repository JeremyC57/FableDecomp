// behaviour test for 0x009852C0 CAIStateGroup_OpinionReactionRoot::FollowUpdate(float distance, bool run): near target
// returns true untouched; far target with TurnToFace false returns false; no wander position returns true; otherwise
// MoveToPosProximity(repeat 1, wander, 0.5, reached, run ? 5 : 3, ...) == 2 is the result
#include <cstdio>
#include <cmath>

struct V3 { float X, Y, Z; };
struct Group {
    V3 creaturePos, targetPos;
    bool turnResult, wanderFound;
    int moveReturn;
    int turnCalls, wanderCalls, moveCalls, lastMovement, lastRepeat;
    float lastWanderRadius, lastProximity;
    V3 lastWanderCentre, lastPos;
    bool TurnToFace() { ++turnCalls; return turnResult; }
    bool GetNearbyWanderPosition(const V3& c, float r, V3& out, bool) {
        ++wanderCalls; lastWanderCentre = c; lastWanderRadius = r;
        out.X = c.X + 1; out.Y = c.Y; out.Z = c.Z; return wanderFound;
    }
    int AIAction_MoveToPosProximity(int repeat, V3 pos, float prox, bool& reached, int movement, bool, bool, bool,
                                    const void*, bool, float) {
        ++moveCalls; lastRepeat = repeat; lastPos = pos; lastProximity = prox; lastMovement = movement; reached = false;
        return moveReturn;
    }
    bool FollowUpdate(float distance, bool run) {
        V3 d = { targetPos.X - creaturePos.X, targetPos.Y - creaturePos.Y, targetPos.Z - creaturePos.Z };
        float lengthSquared = d.X * d.X + d.Y * d.Y + d.Z * d.Z;
        if ((float)sqrt(lengthSquared) > distance) {
            if (!TurnToFace()) return false;
            V3 wander;
            if (GetNearbyWanderPosition(targetPos, distance * 0.9f, wander, false)) {
                int movement = run ? 5 : 3;
                bool reached;
                return AIAction_MoveToPosProximity(1, wander, 0.5f, reached, movement, true, false, true, 0, true, 0.0f) == 2;
            }
        }
        return true;
    }
};

static Group make(float tx) {
    Group g = {};
    g.creaturePos.X = 0; g.targetPos.X = tx; g.turnResult = true; g.wanderFound = true; g.moveReturn = 2;
    return g;
}

int main() {
    int fails = 0;
    Group g = make(3.0f);
    if (!g.FollowUpdate(5.0f, true) || g.turnCalls) { puts("near"); ++fails; }
    g = make(10.0f); g.turnResult = false;
    if (g.FollowUpdate(5.0f, false) || g.wanderCalls) { puts("turn"); ++fails; }
    g = make(10.0f); g.wanderFound = false;
    if (!g.FollowUpdate(5.0f, false) || g.moveCalls) { puts("wander"); ++fails; }
    g = make(10.0f);
    if (!g.FollowUpdate(5.0f, true) || g.lastMovement != 5 || g.lastRepeat != 1 || g.lastProximity != 0.5f ||
        fabs(g.lastWanderRadius - 4.5f) > 1e-5f || g.lastPos.X != 11.0f) { puts("run"); ++fails; }
    g = make(10.0f); g.moveReturn = 1;
    if (g.FollowUpdate(5.0f, false) || g.lastMovement != 3) { puts("walk"); ++fails; }
    g = make(-10.0f); g.moveReturn = 0;
    if (g.FollowUpdate(5.0f, false)) { puts("neg"); ++fails; }
    if (!fails) puts("009852c0_TEST PASS");
    return fails;
}