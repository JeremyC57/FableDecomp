// Test: CAIStateGroup_BirdStartled::Update -- fly to a random flight position (radius 15) when one is found, terminate either way
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct Bird {
    bool haveFlight; int flown; int terminated; float radiusSeen; C3DVector creaturePos; C3DVector flownTo;
    bool GetRandomFlightPos(const C3DVector& from, float radius, C3DVector& result, bool)
    {
        radiusSeen = radius;
        if (!haveFlight) return false;
        result.X = from.X + 1.0f; result.Y = from.Y; result.Z = from.Z + 2.0f;
        return true;
    }
    void AIAction_RandomFlyToPos(const C3DVector& p) { ++flown; flownTo = p; }
    void Terminate() { ++terminated; }
    void Update()
    {
        C3DVector pos;
        if (!GetRandomFlightPos(creaturePos, 15.0f, pos, true)) { Terminate(); return; }
        AIAction_RandomFlyToPos(pos);
        Terminate();
    }
};
int main()
{
    Bird a = { true, 0, 0, 0.0f, { 1.0f, 2.0f, 3.0f }, { 0, 0, 0 } };
    a.Update();
    Bird b = { false, 0, 0, 0.0f, { 1.0f, 2.0f, 3.0f }, { 0, 0, 0 } };
    b.Update();
    if (a.flown != 1 || a.terminated != 1 || a.flownTo.X != 2.0f || a.flownTo.Z != 5.0f || a.radiusSeen != 15.0f
        || b.flown != 0 || b.terminated != 1) { std::printf("00914600_TEST FAIL\n"); return 1; }
    std::printf("00914600_TEST PASS\n");
    return 0;
}