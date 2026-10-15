// behaviour test for 0x006E2640 CParticleCreationParameters(long, long, float, const CRightHandedSet&)
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CMap { void* a; void* b; void* c; bool Dirty; CMap() : a(0), b(0), c(0), Dirty(false) {} };
struct CParticleUpdateParameters { CMap VectorParams, UpdateParams; };
struct CParticleCreationParameters {
    bool InterpolatingParticle, ParticleCreationDisabled, UpdateParametersSet, AlwaysUpdateFlag;
    int StartParticleID, EndParticleID; float Interpolate; CRightHandedSet RHSet; CParticleUpdateParameters UpdateParameters;
    CParticleCreationParameters(int s, int e, float t, const CRightHandedSet& rh)
        : InterpolatingParticle(true), ParticleCreationDisabled(false), UpdateParametersSet(false), AlwaysUpdateFlag(false),
          StartParticleID(s), EndParticleID(e), Interpolate(t), RHSet(rh) {}
};
int main()
{
    CRightHandedSet rh; rh.Up.X = 1; rh.Up.Y = 2; rh.Up.Z = 3; rh.Forward.X = 4; rh.Forward.Y = 5; rh.Forward.Z = 6;
    CParticleCreationParameters p(3, 9, 0.25f, rh);
    if (!p.InterpolatingParticle || p.ParticleCreationDisabled || p.StartParticleID != 3 || p.EndParticleID != 9 ||
        p.Interpolate != 0.25f || p.RHSet.Up.Y != 2 || p.UpdateParameters.UpdateParams.Dirty)
    { std::printf("006e2640_TEST FAIL\n"); return 1; }
    std::printf("006e2640_TEST PASS\n");
    return 0;
}
