// behaviour test for 0x006E25C0 CParticleCreationParameters(long, const CRightHandedSet&)
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CMap { void* a; void* b; void* c; bool Dirty; CMap() : a(0), b(0), c(0), Dirty(false) {} };
struct CParticleUpdateParameters { CMap VectorParams, UpdateParams; };
struct CParticleCreationParameters {
    bool InterpolatingParticle, ParticleCreationDisabled, UpdateParametersSet, AlwaysUpdateFlag;
    int StartParticleID, EndParticleID; float Interpolate; CRightHandedSet RHSet; CParticleUpdateParameters UpdateParameters;
    CParticleCreationParameters(int id, const CRightHandedSet& rh)
        : InterpolatingParticle(false), ParticleCreationDisabled(false), UpdateParametersSet(false), AlwaysUpdateFlag(false),
          StartParticleID(id), EndParticleID(0), Interpolate(0.0f), RHSet(rh) {}
};
int main()
{
    CRightHandedSet rh; rh.Up.X = 1; rh.Up.Y = 2; rh.Up.Z = 3; rh.Forward.X = 4; rh.Forward.Y = 5; rh.Forward.Z = 6;
    CParticleCreationParameters p(7, rh);
    if (p.InterpolatingParticle || p.AlwaysUpdateFlag || p.StartParticleID != 7 || p.EndParticleID != 0 || p.Interpolate != 0.0f ||
        p.RHSet.Forward.Z != 6 || p.RHSet.Up.X != 1 || p.UpdateParameters.UpdateParams.a != 0 || p.UpdateParameters.VectorParams.Dirty)
    { std::printf("006e25c0_TEST FAIL\n"); return 1; }
    std::printf("006e25c0_TEST PASS\n");
    return 0;
}
