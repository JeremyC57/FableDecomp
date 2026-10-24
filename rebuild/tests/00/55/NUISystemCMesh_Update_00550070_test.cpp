#include <cstdio>
struct C3DVector { float X, Y, Z; };
static const C3DVector* g_axis = 0; static float g_angle = 0.0f; static float g_baseDt = 0.0f;
struct CRightHandedSet {
    C3DVector Up, Forward;
    void RotateAboutAxis(const C3DVector& axis, float angle) { g_axis = &axis; g_angle = angle; }
};
struct CChangingStateComponent {
    virtual ~CChangingStateComponent() {}
    virtual void Update(float dt) { g_baseDt = dt; }
};
struct CMesh : CChangingStateComponent {
    CRightHandedSet Orientation; C3DVector RotationAxis; float RotationSpeed;
    virtual void Update(float dt) { CChangingStateComponent::Update(dt); Orientation.RotateAboutAxis(RotationAxis, RotationSpeed * dt); }
};
int main()
{
    CMesh m; m.RotationSpeed = 3.0f;
    m.RotationAxis.X = 0; m.RotationAxis.Y = 1; m.RotationAxis.Z = 0;
    CChangingStateComponent* p = &m;
    p->Update(0.5f);
    if (g_baseDt != 0.5f || g_axis != &m.RotationAxis || g_angle != 1.5f) { std::printf("00550070_TEST FAIL\n"); return 1; }
    std::printf("00550070_TEST PASS\n");
    return 0;
}