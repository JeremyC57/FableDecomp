#include <cstdio>
static float g_fZero = 0.0f;
struct CPhysicsDef { float Mass; };
struct CTCPhysicsBase {
    const CPhysicsDef* PhysicsDef_Object;
    long HasPositiveMass() const { return PhysicsDef_Object->Mass > g_fZero; }
};
int main()
{
    CPhysicsDef d; CTCPhysicsBase b; b.PhysicsDef_Object = &d;
    d.Mass = 2.5f; if (b.HasPositiveMass() != 1) { std::printf("006b14b0_TEST FAIL\n"); return 1; }
    d.Mass = 0.0f; if (b.HasPositiveMass() != 0) { std::printf("006b14b0_TEST FAIL\n"); return 1; }
    d.Mass = -1.0f; if (b.HasPositiveMass() != 0) { std::printf("006b14b0_TEST FAIL\n"); return 1; }
    std::printf("006b14b0_TEST PASS\n");
    return 0;
}