// Behaviour test for 0x00852D10 NCameraModes::CScriptModuleGoToLookAt(const C3DVector&, const CThing*):
// host intelligent pointer and a graphic appearance whose focal point is position + (0, 0, 2 * t).
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct CTCGraphicAppearance { C3DVector pos; C3DVector GetCameraFocalPos(float t) const { return C3DVector(pos.X, pos.Y, pos.Z + 2 * t); } };
struct CThing { CTCGraphicAppearance* m_pGraphicAppearance; };
struct IP { const CThing* item; IP(const CThing* t) : item(t) {} const CThing* operator->() const { return item; } };
struct Module {
    IP PThing; C3DVector GoTo; C3DVector LookAt;
    Module(const C3DVector& goTo, const CThing* thing) : PThing(thing), GoTo(goTo)
    {
        const CThing* target = PThing.operator->();
        C3DVector focal = target->m_pGraphicAppearance->GetCameraFocalPos(1.0f);
        LookAt = focal;
    }
};
int main()
{
    CTCGraphicAppearance app; app.pos = C3DVector(1, 2, 3);
    CThing t; t.m_pGraphicAppearance = &app;
    Module m(C3DVector(7, 8, 9), &t);
    if (m.PThing.item != &t || m.GoTo.X != 7 || m.GoTo.Z != 9 || m.LookAt.X != 1 || m.LookAt.Z != 5) { std::printf("00852d10_TEST FAIL\n"); return 1; }
    std::printf("00852d10_TEST PASS\n");
    return 0;
}
