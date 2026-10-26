#include <cstdio>
static float g_fZero = 0.0f;
static int g_baseCreate = 0;
class CTCPhysicsBase { public:
    float mass;
    void OnCreate() { ++g_baseCreate; }
    float GetMass() { return mass; }
};
struct CTCPhysicsLight : CTCPhysicsBase {
    bool AffectedByGravity : 1;
    bool Other : 1;
    void OnCreate()
    {
        CTCPhysicsBase::OnCreate();
        AffectedByGravity = GetMass() > g_fZero;
    }
};
int main()
{
    CTCPhysicsLight l; l.Other = true; l.AffectedByGravity = false; l.mass = 3.0f;
    l.OnCreate();
    if (!l.AffectedByGravity || !l.Other || g_baseCreate != 1) { std::printf("007232e0_TEST FAIL\n"); return 1; }
    l.mass = 0.0f; l.OnCreate();
    if (l.AffectedByGravity || !l.Other || g_baseCreate != 2) { std::printf("007232e0_TEST FAIL\n"); return 1; }
    std::printf("007232e0_TEST PASS\n");
    return 0;
}