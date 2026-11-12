#include <cstdio>
// Behaviour test for CNoise::CNoise @ 0x00F36BA0: the seed is cleared before the tables are built
class C2DVector { public: float x, y; };
static unsigned long g_seedSeen = 0xffffffff;
class CNoise {
public:
    CNoise() : Seed(0) { InitPerlinNoise(); }
    void InitPerlinNoise() { g_seedSeen = Seed; for (int i = 0; i < 512; ++i) { P[i] = i; G2[i].x = 1.0f; G2[i].y = 0.0f; } }
    long P[512];
    C2DVector G2[512];
    unsigned long Seed;
};
int main()
{
    CNoise* n = new CNoise;
    if (n->Seed != 0 || g_seedSeen != 0 || n->P[511] != 511 || n->G2[3].x != 1.0f) { std::printf("00f36ba0_TEST FAIL\n"); return 1; }
    delete n;
    std::printf("00f36ba0_TEST PASS\n");
    return 0;
}