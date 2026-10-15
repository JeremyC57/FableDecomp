#include <cstdio>
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator*(float s) const { return C3DVector(x * s, y * s, z * s); }
};
namespace NCameraModes {
struct CPhysicsForceModelBase {
    C3DVector m_source;
    virtual ~CPhysicsForceModelBase() { }
    virtual C3DVector DoVelocityDamping() { return m_source; }
};
struct CPhysicsForceModelGotoOutsideSimple : CPhysicsForceModelBase {
    virtual C3DVector DoVelocityDamping() { return CPhysicsForceModelBase::DoVelocityDamping() * 6.0f; }
};
}
int main()
{
    NCameraModes::CPhysicsForceModelGotoOutsideSimple m;
    NCameraModes::CPhysicsForceModelBase* b = &m;
    m.m_source = C3DVector(1.0f, -2.0f, 0.5f);
    C3DVector r = b->DoVelocityDamping();
    if (r.x != 1.0f * 6.0f || r.y != -2.0f * 6.0f || r.z != 0.5f * 6.0f) { std::printf("008fcc10_TEST FAIL\n"); return 1; }
    m.m_source = C3DVector(0.0f, 0.0f, 0.0f);
    r = b->DoVelocityDamping();
    if (r.x != 0.0f || r.y != 0.0f || r.z != 0.0f) { std::printf("008fcc10_TEST FAIL\n"); return 1; }
    std::printf("008fcc10_TEST PASS\n");
    return 0;
}