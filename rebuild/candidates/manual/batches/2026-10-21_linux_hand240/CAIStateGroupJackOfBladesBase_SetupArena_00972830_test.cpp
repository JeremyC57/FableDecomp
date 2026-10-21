#include <cstdio>
#include <cstring>
// Behaviour copy of CAIStateGroup_JackOfBladesBase::SetupArena (0x00972830): search box, latch, camera set.
struct Box { long MinX, MinY, MaxX, MaxY; };
struct Thing { const char* def; float x, y; };
static Box MakeBox(float x, float y, long half) { long ix = (long)x, iy = (long)y; Box b = { ix - half, iy - half, ix + half, iy + half }; return b; }
static const Thing* Search(const Box& b, const char* def, const Thing* things, int n) {
    for (int i = 0; i < n; ++i)
        if (std::strcmp(things[i].def, def) == 0 && things[i].x >= b.MinX && things[i].x <= b.MaxX && things[i].y >= b.MinY && things[i].y <= b.MaxY)
            return &things[i];
    return 0;
}
struct State { const Thing* arenaCentre; const char* cameraSet; };
static void SetupArena(State& s, float cx, float cy, const Thing* things, int n, bool hasCamera) {
    {
        const Thing* centre = 0;
        centre = Search(MakeBox(cx, cy, 50), "FrescoCentre", things, n);
        s.arenaCentre = centre;
    }
    if (hasCamera) s.cameraSet = "CAMERA_MANAGER_SET_DEFAULT";
}
int main() {
    Thing things[3] = { { "Tree", 1, 1 }, { "FrescoCentre", 140, 0 }, { "FrescoCentre", 30.5f, -40 } };
    State s = { 0, 0 };
    Box b = MakeBox(10.7f, -3.2f, 50);
    if (b.MinX != -40 || b.MaxY != 47) { std::printf("00972830_TEST FAIL\n"); return 1; }
    SetupArena(s, 10.7f, -3.2f, things, 3, true);
    if (s.arenaCentre != &things[2] || s.cameraSet == 0) { std::printf("00972830_TEST FAIL\n"); return 1; }
    State t = { &things[0], 0 };
    SetupArena(t, 500, 500, things, 3, false);
    if (t.arenaCentre != 0 || t.cameraSet != 0) { std::printf("00972830_TEST FAIL\n"); return 1; }
    std::printf("00972830_TEST PASS\n"); return 0;
}
