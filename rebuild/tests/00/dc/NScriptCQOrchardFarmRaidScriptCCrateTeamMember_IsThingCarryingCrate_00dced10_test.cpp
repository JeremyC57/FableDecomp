#include <cstdio>
// Behaviour test for CCrateTeamMember::IsThingCarryingCrate @ 0x00DCED10.
struct Crate { bool alive; int carrier; bool IsAlive() const { return alive; } bool IsBeingCarriedBy(int c) const { return carrier == c; } };
struct Member {
    Crate* crates; unsigned long count; int outOfActionAfter; int checks;
    bool IsOutOfAction() { ++checks; return outOfActionAfter >= 0 && checks > outOfActionAfter; }
    bool Return(bool value) { if (IsOutOfAction()) return false; return value; }
    bool IsThingCarryingCrate(int thing)
    {
        for (unsigned long i = 0; i < count; ++i) {
            if (IsOutOfAction()) return false;
            if (crates[i].IsAlive()) { if (crates[i].IsBeingCarriedBy(thing)) return Return(true); }
        }
        return Return(false);
    }
};
static int Fail() { std::printf("00dced10_TEST FAIL\n"); return 1; }
int main()
{
    Crate c[3] = {{true, 1}, {false, 2}, {true, 3}};
    Member m = {c, 3, -1, 0};
    if (!m.IsThingCarryingCrate(3) || m.checks != 4) return Fail();
    m.checks = 0; if (m.IsThingCarryingCrate(2) || m.checks != 4) return Fail();
    m.checks = 0; m.outOfActionAfter = 3; if (m.IsThingCarryingCrate(3)) return Fail();
    m.checks = 0; m.outOfActionAfter = 0; if (m.IsThingCarryingCrate(1) || m.checks != 1) return Fail();
    m.count = 0; m.checks = 0; m.outOfActionAfter = -1; if (m.IsThingCarryingCrate(1) || m.checks != 1) return Fail();
    std::printf("00dced10_TEST PASS\n");
    return 0;
}