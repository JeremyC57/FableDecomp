// Behaviour test for CAIStateGroup_PrepareMeal::Update (0x00946190), g++ model.
#include <cstdio>
struct V { float X, Y, Z; };
struct Script { int moveResults[4]; int moveCalls; int lookResult; V lookAt; int idleLeft; int modeAdds, modeRemoves, terminates; };
static void Update(Script& s, V actionForward, V creaturePos)
{
    int r;
    do { r = s.moveResults[s.moveCalls++]; } while (r == 4);
    if (r == 2) {
        V forward = actionForward;
        V sum = { forward.X + creaturePos.X, forward.Y + creaturePos.Y, forward.Z + creaturePos.Z };
        s.lookAt = sum;
        if (s.lookResult == 2) {
            ++s.modeAdds;
            while (s.idleLeft-- > 0) { }
            ++s.modeRemoves;
        }
    }
    ++s.terminates;
}
int main()
{
    Script s = { { 4, 4, 2, 0 }, 0, 2, { 0, 0, 0 }, 3, 0, 0, 0 };
    V f = { 0, 1, 0 }, p = { 10, 20, 30 };
    Update(s, f, p);
    int ok = s.moveCalls == 3 && s.lookAt.X == 10 && s.lookAt.Y == 21 && s.lookAt.Z == 30 && s.modeAdds == 1 && s.modeRemoves == 1 && s.terminates == 1;
    Script t = { { 3, 0, 0, 0 }, 0, 2, { 0, 0, 0 }, 0, 0, 0, 0 };
    Update(t, f, p);
    ok &= t.moveCalls == 1 && t.modeAdds == 0 && t.terminates == 1;
    Script u = { { 2, 0, 0, 0 }, 0, 3, { 0, 0, 0 }, 0, 0, 0, 0 };
    Update(u, f, p);
    ok &= u.modeAdds == 0 && u.terminates == 1 && u.lookAt.Y == 21;
    std::printf(ok ? "00946190_TEST PASS\n" : "00946190_TEST FAIL\n");
    return ok ? 0 : 1;
}
