// Test: GetPNearestBonfire -- the thing-search best match (CIsFreeBonfire filter, nearest to the creature) written through the out-pointer, 0 when none
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CThing { public: C3DVector Pos; bool Matches; };
static void GetPBestThing(const CThing* things, int count, const C3DVector& from, const CThing** best)
{
    float bestDist = 0.0f;
    for (int i = 0; i < count; ++i) {
        if (!things[i].Matches) continue;
        float dx = things[i].Pos.X - from.X, dy = things[i].Pos.Y - from.Y, dz = things[i].Pos.Z - from.Z;
        float d = dx * dx + dy * dy + dz * dz;
        if (!*best || d < bestDist) { *best = &things[i]; bestDist = d; }
    }
}
static const CThing* Find(const CThing* things, int count, const C3DVector& creaturePos)
{
    const CThing* nearest = 0;
    GetPBestThing(things, count, creaturePos, &nearest);
    return nearest;
}
int main()
{
    CThing t[3] = { { { 10.0f, 0.0f, 0.0f }, true }, { { 1.0f, 0.0f, 0.0f }, false }, { { 3.0f, 0.0f, 0.0f }, true } };
    C3DVector at = { 0.0f, 0.0f, 0.0f };
    if (Find(t, 3, at) != &t[2] || Find(t + 1, 1, at) != 0) { std::printf("009170b0_TEST FAIL\n"); return 1; }
    std::printf("009170b0_TEST PASS\n");
    return 0;
}