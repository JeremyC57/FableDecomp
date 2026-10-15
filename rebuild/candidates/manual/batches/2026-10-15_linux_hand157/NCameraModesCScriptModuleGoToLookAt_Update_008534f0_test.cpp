// Behaviour test for 0x008534F0 NCameraModes::CScriptModuleGoToLookAt::Update: live, dead and null targets.
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct CTCGraphicAppearance { C3DVector pos; C3DVector GetCameraFocalPos(float t) const { return C3DVector(pos.X, pos.Y, pos.Z + t); } };
struct CThing { CTCGraphicAppearance* m_pGraphicAppearance; bool m_dead; };
struct IP { const CThing* item; const CThing* GetPItem() const { return item; } const CThing* operator->() const { return item; } };
struct CCameraState { C3DVector LookAt; C3DVector Position; };
struct Module {
    CCameraState* POwner; IP PThing; C3DVector GoTo; C3DVector LookAt;
    void Update()
    {
        POwner->Position = GoTo;
        if (PThing.GetPItem()) {
            if (!PThing->m_dead) {
                const CThing* target = PThing.operator->();
                C3DVector focal = target->m_pGraphicAppearance->GetCameraFocalPos(1.0f);
                LookAt = focal;
            }
        }
        POwner->LookAt = LookAt;
    }
};
int main()
{
    CTCGraphicAppearance app; app.pos = C3DVector(1, 1, 1);
    CThing t; t.m_pGraphicAppearance = &app; t.m_dead = false;
    CCameraState cam; Module m; m.POwner = &cam; m.PThing.item = &t; m.GoTo = C3DVector(5, 5, 5); m.LookAt = C3DVector(0, 0, 0);
    m.Update();
    if (cam.Position.X != 5 || cam.LookAt.Z != 2) { std::printf("008534f0_TEST FAIL\n"); return 1; }
    app.pos = C3DVector(3, 3, 3); t.m_dead = true; m.Update();
    if (cam.LookAt.Z != 2) { std::printf("008534f0_TEST FAIL\n"); return 1; }
    m.PThing.item = 0; m.Update();
    if (cam.LookAt.Z != 2) { std::printf("008534f0_TEST FAIL\n"); return 1; }
    std::printf("008534f0_TEST PASS\n");
    return 0;
}
