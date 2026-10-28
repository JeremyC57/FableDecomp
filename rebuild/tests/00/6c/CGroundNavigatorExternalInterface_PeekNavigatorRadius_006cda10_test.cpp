// Behaviour test for 0x006CDA10 CGroundNavigatorExternalInterface::PeekNavigatorRadius: the body radius (+0.1 for the player) or the stored radius (standalone copy of the logic)
#include <cstdio>
struct Nav { float BodyRadius, Radius; bool Player; float Peek(bool physical) const { if (physical) { float r = BodyRadius; if (Player) r += 0.1f; return r; } return Radius; } };

int main()
{
    Nav n; n.BodyRadius = 0.5f; n.Radius = 2.0f; n.Player = false;
    if (n.Peek(false) != 2.0f || n.Peek(true) != 0.5f) { std::printf("006cda10_TEST FAIL\n"); return 1; }
    n.Player = true; if (n.Peek(true) != 0.5f + 0.1f) { std::printf("006cda10_TEST FAIL\n"); return 1; }
    std::printf("006cda10_TEST PASS\n");
    return 0;
}