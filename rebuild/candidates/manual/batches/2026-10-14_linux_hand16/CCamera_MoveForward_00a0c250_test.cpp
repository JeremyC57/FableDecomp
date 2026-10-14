#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; } };
struct CCamera { C3DVector WorldPosition; C3DVector Up; C3DVector Forward; bool HeightLocked; bool ViewVecZLocked; unsigned long FOVFlags; float HorizontalFOV; float VerticalFOV; float Zoom;
    void MoveForward(float distance) { if (HeightLocked) { WorldPosition.X += Forward.X * distance; WorldPosition.Y += Forward.Y * distance; } else { WorldPosition += Forward * distance; } } };

int main()
{
    CCamera cam; cam.WorldPosition = C3DVector(1.0f, 2.0f, 3.0f); cam.Forward = C3DVector(1.0f, 0.5f, 0.25f);
    cam.HeightLocked = true; cam.MoveForward(2.0f);
    if (cam.WorldPosition.X != 3.0f || cam.WorldPosition.Y != 3.0f || cam.WorldPosition.Z != 3.0f) { std::printf("00a0c250_TEST FAIL\n"); return 1; }
    cam.HeightLocked = false; cam.MoveForward(4.0f);
    if (cam.WorldPosition.X != 7.0f || cam.WorldPosition.Y != 5.0f || cam.WorldPosition.Z != 4.0f) { std::printf("00a0c250_TEST FAIL\n"); return 1; }
    std::printf("00a0c250_TEST PASS\n");
    return 0;
}
