#include <cstdio>
struct CMapwho { bool in; int placed, died; void Place() { in = true; ++placed; } void OnDie() { in = false; ++died; } };
struct CThing {
    CMapwho* mapwho; bool physics; bool toBePlaced;
    void Update() {
        if (mapwho) {
            if (!mapwho->in && physics && toBePlaced) mapwho->Place();
            else if (mapwho->in && physics && !toBePlaced) mapwho->OnDie();
        }
    }
};
int main()
{
    CMapwho m = { false, 0, 0 };
    CThing t = { &m, true, true };
    t.Update(); t.Update();
    if (m.placed != 1 || !m.in) { std::printf("004c8c00_TEST FAIL\n"); return 1; }
    t.toBePlaced = false; t.Update(); t.Update();
    if (m.died != 1 || m.in) { std::printf("004c8c00_TEST FAIL\n"); return 1; }
    t.physics = false; t.toBePlaced = true; t.Update();
    CThing n = { 0, true, true }; n.Update();
    if (m.placed != 1) { std::printf("004c8c00_TEST FAIL\n"); return 1; }
    std::printf("004c8c00_TEST PASS\n");
    return 0;
}
