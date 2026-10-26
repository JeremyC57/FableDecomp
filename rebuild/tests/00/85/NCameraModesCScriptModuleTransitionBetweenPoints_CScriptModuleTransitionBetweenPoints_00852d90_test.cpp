// Behaviour test for 0x00852D90 NCameraModes::CScriptModuleTransitionBetweenPoints(const CThing*, ...):
// the set-up call receives the thing's focal point twice plus the forwarded arguments.
#include <cstdio>
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CTCGraphicAppearance { C3DVector pos; C3DVector GetCameraFocalPos(float t) const { return C3DVector(pos.X * t, pos.Y, pos.Z); } };
class CThing { public: CTCGraphicAppearance* m_pGraphicAppearance; };
struct IP { const CThing* item; IP(const CThing* t) : item(t) {} const CThing* operator->() const { return item; } };
struct Module {
    IP PThing; C3DVector StartPos, EndPos, StartLook, EndLook; long Frames; float FOV;
    void SetUpTransitionCamera(const C3DVector& s, const C3DVector& e, const C3DVector& sl, const C3DVector& el, long frames, float fov)
    { StartPos = s; EndPos = e; StartLook = sl; EndLook = el; Frames = frames; FOV = fov; }
    Module(const CThing* thing, const C3DVector& startLookAt, const C3DVector& endLookAt, long frames, float fov) : PThing(thing)
    {
        const CThing* target = PThing.operator->();
        C3DVector focal = target->m_pGraphicAppearance->GetCameraFocalPos(1.0f);
        SetUpTransitionCamera(focal, focal, startLookAt, endLookAt, frames, fov);
    }
};
int main()
{
    CTCGraphicAppearance app; app.pos = C3DVector(4, 5, 6);
    CThing t; t.m_pGraphicAppearance = &app;
    Module m(&t, C3DVector(1, 0, 0), C3DVector(2, 0, 0), 12, 0.75f);
    if (m.StartPos.X != 4 || m.EndPos.Z != 6 || m.StartLook.X != 1 || m.EndLook.X != 2 || m.Frames != 12 || m.FOV != 0.75f) { std::printf("00852d90_TEST FAIL\n"); return 1; }
    std::printf("00852d90_TEST PASS\n");
    return 0;
}