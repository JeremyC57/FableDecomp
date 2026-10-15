// Behaviour test for 0x0068D790 (CInputProcessCameraLookAround::ProcessInput): the look vector handed to the camera manager per event.
#include <cstdio>
struct Vec2 { float x, y; Vec2(float a, float b) : x(a), y(b) {} };
struct Event { int analogueAction; float ax, ay; int digitalAction; };
struct CameraManager { int calls; Vec2 last; CameraManager() : calls(0), last(0, 0) {} void SetRightStickLookAroundVector(const Vec2& v) { ++calls; last = v; } };
struct Camera { CameraManager* mgr; bool GetCameraManager(CameraManager** out) { *out = mgr; return true; } };
struct Process {
    Camera* cam;
    bool GetEventGameActionVector(int action, const Event& e, Vec2* v) { if (e.analogueAction != action) return false; v->x = e.ax; v->y = e.ay; return true; }
    bool IsEventGameAction(int action, const Event& e) { return e.digitalAction == action; }
    void ProcessInput(const Event& event)
    {
        Vec2 look(0.0f, 0.0f);
        if (!GetEventGameActionVector(0x4d, event, &look) && !GetEventGameActionVector(0x3d, event, &look)) {
            if (IsEventGameAction(0x3e, event)) look.x = 1.0f;
            else if (IsEventGameAction(0x3f, event)) look.x = -1.0f;
            else return;
        }
        Camera* source = cam; CameraManager* camera = 0; source->GetCameraManager(&camera);
        camera->SetRightStickLookAroundVector(look);
    }
};
int main()
{
    CameraManager m; Camera c; c.mgr = &m; Process p; p.cam = &c; int ok = 1;
    Event a = { 0x4d, 0.25f, -0.5f, 0 }; p.ProcessInput(a); ok = ok && m.calls == 1 && m.last.x == 0.25f && m.last.y == -0.5f;
    Event b = { 0x3d, 0.75f, 0.5f, 0 }; p.ProcessInput(b); ok = ok && m.calls == 2 && m.last.x == 0.75f;
    Event d = { 0, 0, 0, 0x3e }; p.ProcessInput(d); ok = ok && m.calls == 3 && m.last.x == 1.0f && m.last.y == 0.0f;
    Event e = { 0, 0, 0, 0x3f }; p.ProcessInput(e); ok = ok && m.calls == 4 && m.last.x == -1.0f;
    Event f = { 0, 0, 0, 0x10 }; p.ProcessInput(f); ok = ok && m.calls == 4;
    if (!ok) { std::printf("0068d790_TEST FAIL\n"); return 1; }
    std::printf("0068d790_TEST PASS\n");
    return 0;
}