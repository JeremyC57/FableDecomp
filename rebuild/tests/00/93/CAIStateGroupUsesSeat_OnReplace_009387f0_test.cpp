#include <cstdio>
// Behaviour copy of CAIStateGroup_UsesSeat::OnReplace (0x009387F0).
struct Sim { bool gotSeat, seatAlive, sitting, alternate, getUpOk; long index; long freed; long removedMode; long movementUsed; const char* nameUsed; };
static void OnReplace(Sim& s) {
    if (s.gotSeat && s.seatAlive) { s.freed = s.index; s.gotSeat = false; }
    s.index = -1;
    if (s.sitting) {
        long movement = (s.alternate != 0) + 8;
        const char* name = s.alternate ? "ALT" : "STD";
        s.movementUsed = movement; s.nameUsed = name;
        if (!s.getUpOk) s.removedMode = movement;
        s.sitting = false;
    }
}
int main() {
    Sim a = { true, true, true, true, false, 5, -9, 0, 0, 0 };
    OnReplace(a);
    if (a.freed != 5 || a.gotSeat || a.index != -1 || a.sitting || a.movementUsed != 9 || a.removedMode != 9 || a.nameUsed[0] != 'A') { std::printf("009387f0_TEST FAIL\n"); return 1; }
    Sim b = { true, false, true, false, true, 5, -9, 0, 0, 0 };
    OnReplace(b);
    if (b.freed != -9 || !b.gotSeat || b.movementUsed != 8 || b.removedMode != 0) { std::printf("009387f0_TEST FAIL\n"); return 1; }
    std::printf("009387f0_TEST PASS\n"); return 0;
}