// Behaviour test for 0x0092BF50 (parked row): the row's logic over a flat model.
#include <cstdio>

struct V { float X, Y; };
struct Building { V Pos; bool Dead; bool InFOV; };
static int FaceAway(const V& self, Building* nearest, V* look) {
    if (nearest && !nearest->Dead && nearest->InFOV) { look->X = (self.X - nearest->Pos.X) + self.X; look->Y = (self.Y - nearest->Pos.Y) + self.Y; return 1; }
    return 2;
}
int main() {
    V self = { 0, 0 }, look = { 9, 9 }; Building b = { { 3, 4 }, false, true };
    bool ok = FaceAway(self, &b, &look) == 1 && look.X == -3 && look.Y == -4;
    b.InFOV = false; ok = ok && FaceAway(self, &b, &look) == 2 && FaceAway(self, 0, &look) == 2;
    std::printf(ok ? "0092bf50_TEST PASS\n" : "0092bf50_TEST FAIL\n");
    return ok ? 0 : 1;
}
