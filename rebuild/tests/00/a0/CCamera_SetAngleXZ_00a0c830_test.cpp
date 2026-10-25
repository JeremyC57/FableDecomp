// Behaviour test for CCamera::SetAngleXZ(float) (0x00A0C830): a local (0, 0, 1) vector is rotated by the angle about
// the camera's forward axis (the axis passed by reference, the angle forwarded unchanged) and then dropped -- the
// camera itself is left unchanged.
#include <cstdio>

class C3DVector;
static const C3DVector* gAxis = 0;
static float gAngle = 0.0f;
static float gIn[3];
static int gCalls = 0;

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Rotate(const C3DVector& axis, float angle)
    {
        gAxis = &axis;
        gAngle = angle;
        gIn[0] = X; gIn[1] = Y; gIn[2] = Z;
        ++gCalls;
        X = Y = Z = 99.0f;                              // the rotated result is discarded by the caller
    }
};

class CRightHandedSet {
public:
    C3DVector Up;
    C3DVector Forward;
};

class CCamera {
public:
    C3DVector       WorldPosition;
    CRightHandedSet RHSet;
    bool            HeightLocked;
    bool            ViewVecZLocked;
    unsigned long   FOVFlags;
    float           HorizontalFOV;
    float           VerticalFOV;
    float           Zoom;
    void SetAngleXZ(float angle)
    {
        C3DVector v(0.0f, 0.0f, 1.0f);
        v.Rotate(RHSet.Forward, angle);
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 1e-6f && d > -1e-6f; }

int main()
{
    CCamera cam;
    cam.WorldPosition = C3DVector(1.0f, 2.0f, 3.0f);
    cam.RHSet.Up = C3DVector(0.0f, 0.0f, 1.0f);
    cam.RHSet.Forward = C3DVector(0.0f, 1.0f, 0.0f);
    cam.Zoom = 2.5f;
    cam.SetAngleXZ(0.75f);
    int fails = 0;
    if (gCalls != 1 || gAxis != &cam.RHSet.Forward || !Near(gAngle, 0.75f)) ++fails;
    if (!Near(gIn[0], 0.0f) || !Near(gIn[1], 0.0f) || !Near(gIn[2], 1.0f)) ++fails;
    if (!Near(cam.WorldPosition.X, 1.0f) || !Near(cam.RHSet.Forward.Y, 1.0f) || !Near(cam.RHSet.Up.Z, 1.0f)) ++fails;
    if (!Near(cam.Zoom, 2.5f)) ++fails;
    if (fails) { std::printf("00a0c830_TEST FAIL\n"); return 1; }
    std::printf("00a0c830_TEST PASS\n");
    return 0;
}