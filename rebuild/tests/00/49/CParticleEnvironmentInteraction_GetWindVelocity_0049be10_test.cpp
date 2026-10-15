// behaviour test for 0x0049BE10 CParticleEnvironmentInteraction::GetWindVelocity: wind direction * speed with a world, else WindVelocity
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector operator*(float s) const { C3DVector r; r.X = X * s; r.Y = Y * s; r.Z = Z * s; return r; } };
struct CWind { C3DVector dir; float speed; const C3DVector& GetDirection() const { return dir; } float GetSpeed() const { return speed; } };
struct CWorld { CWind* PWind; CWind* GetWind() const { return PWind; } };
struct CPEI {
    CWorld* PWorld; C3DVector WindVelocity;
    C3DVector GetWindVelocity() const
    {
        if (PWorld) { C3DVector d = PWorld->GetWind()->GetDirection(); return d * PWorld->GetWind()->GetSpeed(); }
        return WindVelocity;
    }
};
int main()
{
    CWind w = { { 1.0f, 0.0f, -2.0f }, 3.0f }; CWorld world = { &w };
    CPEI a = { &world, { 9, 9, 9 } }, b = { 0, { 4, 5, 6 } };
    C3DVector va = a.GetWindVelocity(), vb = b.GetWindVelocity();
    if (va.X != 3.0f || va.Y != 0.0f || va.Z != -6.0f || vb.Y != 5.0f) { std::printf("0049be10_TEST FAIL\n"); return 1; }
    std::printf("0049be10_TEST PASS\n");
    return 0;
}