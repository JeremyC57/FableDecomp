// behaviour test for CAIStateGroup_Clean::OnActivate (0x00917520)
#include <cstdio>
struct V { float X, Y, Z; };
struct Building { bool hasPhysics; V centre; float floorZ; bool Inside(const V& p) const { return p.Z >= floorZ; } };
struct Group {
    Building* b; float FloorHeight; bool Activated;
    void OnActivate()
    {
        if (Activated) return;
        Activated = true;
        if (b->hasPhysics) {
            V pos = b->centre;
            if (b->Inside(pos)) {
                V last;
                while (b->Inside(pos)) { last = pos; pos.Z -= 0.5f; }
                FloorHeight = last.Z;
            }
        }
    }
};
int main()
{
    Building b = { true, { 0, 0, 3.0f }, 1.2f };
    Group g = { &b, -1.0f, false };
    g.OnActivate();
    bool ok = g.Activated && g.FloorHeight == 1.5f;
    g.FloorHeight = 7; g.OnActivate(); ok = ok && g.FloorHeight == 7;
    Building b2 = { true, { 0, 0, 0.0f }, 1.0f }; Group g2 = { &b2, 9, false }; g2.OnActivate();
    ok = ok && g2.FloorHeight == 9;
    if (!ok) { std::printf("00917520_TEST FAIL\n"); return 1; }
    std::printf("00917520_TEST PASS\n");
    return 0;
}
