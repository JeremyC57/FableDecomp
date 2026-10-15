#include <cstdio>
#include <math.h>
// Behaviour test for 006977f0.

// CGameCameraManager::GetViewWidthAtPosition: 2 * distance * tan(fov * 0.5 * 2pi), the FOV in turns.
struct C3DVector { float X, Y, Z; };
struct CCamera { C3DVector WorldPosition; float HorizontalFOV; float GetHorizontalFOV() const { return HorizontalFOV; } };
struct CGameCameraManager {
    CCamera Camera;
    float GetViewWidthAtPosition(const C3DVector& position)
    {
        float fov = Camera.GetHorizontalFOV();
        C3DVector cameraPosition = Camera.WorldPosition;
        C3DVector delta; delta.X = position.X - cameraPosition.X; delta.Y = position.Y - cameraPosition.Y; delta.Z = position.Z - cameraPosition.Z;
        float distance = (float)sqrt(delta.Z * delta.Z + delta.Y * delta.Y + delta.X * delta.X);
        return (float)(tan((fov * 0.5f) * (double)6.2831855f) * distance) * 2.0f;
    }
};
int main()
{
    CGameCameraManager m; m.Camera.WorldPosition.X = 1.0f; m.Camera.WorldPosition.Y = 2.0f; m.Camera.WorldPosition.Z = 3.0f;
    m.Camera.HorizontalFOV = 0.25f;                 // a quarter turn: tan(45 degrees) = 1
    C3DVector p; p.X = 4.0f; p.Y = 6.0f; p.Z = 3.0f; // distance 5
    float w = m.GetViewWidthAtPosition(p);
    int ok = w > 9.999f && w < 10.001f;
    if (!ok) { std::printf("006977f0_TEST FAIL\n"); return 1; }
    std::printf("006977f0_TEST PASS\n");
    return 0;
}