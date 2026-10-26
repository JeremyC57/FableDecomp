// behaviour test for 0x00AEDDC0 CParticleEmitter::SetPosition: non-2D emitters take the interpolator and set both matrix translations
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class C3DPositionInterpolator { public: C3DVector StartPosition, EndPosition; };
class CMatrix3x4 { public: float m[9]; float PositionX, PositionY, PositionZ;
    void SetPosition(const C3DVector& v) { PositionX = v.X; PositionY = v.Y; PositionZ = v.Z; } };
struct CParticleEmitter {
    unsigned short Emitter2D : 1; C3DPositionInterpolator PositionInterpolator; CMatrix3x4 Transform, EndTransform;
    bool IsEmitter2D() const { return Emitter2D; }
    void SetPosition(const C3DPositionInterpolator& p)
    {
        if (!IsEmitter2D()) { PositionInterpolator = p; Transform.SetPosition(PositionInterpolator.StartPosition); EndTransform.SetPosition(PositionInterpolator.EndPosition); }
    }
};
int main()
{
    CParticleEmitter e; e.Emitter2D = 0; e.Transform.PositionX = 0; e.EndTransform.PositionZ = 0;
    C3DPositionInterpolator p = { { 1, 2, 3 }, { 4, 5, 6 } };
    e.SetPosition(p);
    CParticleEmitter f; f.Emitter2D = 1; f.Transform.PositionX = -1; f.SetPosition(p);
    if (e.Transform.PositionX != 1 || e.Transform.PositionZ != 3 || e.EndTransform.PositionY != 5 || e.PositionInterpolator.EndPosition.Z != 6 ||
        f.Transform.PositionX != -1) { std::printf("00aeddc0_TEST FAIL\n"); return 1; }
    std::printf("00aeddc0_TEST PASS\n");
    return 0;
}