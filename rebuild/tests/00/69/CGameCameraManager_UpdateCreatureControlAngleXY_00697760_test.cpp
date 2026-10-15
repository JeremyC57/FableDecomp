// Behaviour test for 0x00697760 CGameCameraManager::UpdateCreatureControlAngleXY.
#include <cstdio>
static int g_frame = 50;
static int GetGameFrame() { return g_frame; }
struct CCamera { float angle; float GetAngleXY() { return angle; } };
struct M {
    CCamera Camera; bool ChangeInControlDirection; float ControlAngleXY; bool AngleChange; bool ControlNoControlMovement;
    int ControlNoControlMovementStartFrame;
    void Update()
    {
        if (AngleChange) { if (ChangeInControlDirection) AngleChange = false; }
        if (!AngleChange) ControlAngleXY = Camera.GetAngleXY();
        if (ControlNoControlMovement) { if (ControlNoControlMovementStartFrame == 0) ControlNoControlMovementStartFrame = GetGameFrame(); }
        if (!ControlNoControlMovement) { if (ControlNoControlMovementStartFrame != 0) ControlNoControlMovementStartFrame = 0; }
        ChangeInControlDirection = false;
        ControlNoControlMovement = false;
    }
};
static int Fail() { std::printf("00697760_TEST FAIL\n"); return 1; }
int main()
{
    M m; m.Camera.angle = 1.5f; m.ChangeInControlDirection = false; m.ControlAngleXY = 0.0f; m.AngleChange = true;
    m.ControlNoControlMovement = true; m.ControlNoControlMovementStartFrame = 0;
    m.Update();
    if (m.ControlAngleXY != 0.0f || !m.AngleChange || m.ControlNoControlMovementStartFrame != 50 || m.ControlNoControlMovement) return Fail();
    m.ChangeInControlDirection = true; m.ControlNoControlMovement = true; g_frame = 60;
    m.Update();
    if (m.ControlAngleXY != 1.5f || m.AngleChange || m.ControlNoControlMovementStartFrame != 50 || m.ChangeInControlDirection) return Fail();
    m.Update();
    if (m.ControlNoControlMovementStartFrame != 0) return Fail();
    std::printf("00697760_TEST PASS\n");
    return 0;
}