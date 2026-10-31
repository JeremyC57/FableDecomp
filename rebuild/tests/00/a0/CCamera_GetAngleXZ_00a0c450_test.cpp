// Behaviour test for CCamera::GetAngleXZ (0x00A0C450): the camera's roll in turns from the rebuilt unrolled up vector.
#include <cstdio>
#include <math.h>

const double TWO_PI = 6.2831854820251465;

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const float Dot(const C3DVector& other) const { return Z * other.Z + Y * other.Y + X * other.X; }
    const C3DVector& InitialiseFromAngles(float angleXY, float angleYZ, float length)
    {
        float cosYZ = (float)cos(angleYZ * TWO_PI);
        X = (float)(sin(angleXY * TWO_PI) * (cosYZ * length));
        Y = (float)(cos(angleXY * TWO_PI) * (cosYZ * length));
        Z = (float)(sin(angleYZ * TWO_PI) * length);
        return *this;
    }
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

static float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

class CCamera {
public:
    C3DVector       WorldPosition;
    CRightHandedSet RHSet;
    float GetAngleXY() const { return WrapAngle((float)atan2(RHSet.Forward.X, RHSet.Forward.Y) * 0.15915494f); }
    float GetAngleYZ() const
    {
        float length = (float)sqrt(RHSet.Forward.X * RHSet.Forward.X + RHSet.Forward.Y * RHSet.Forward.Y);
        return WrapAngle((float)atan2(RHSet.Forward.Z, length) * 0.15915494f);
    }
    float GetAngleXZ() const
    {
        float heading = GetAngleXY() + 0.5f;
        float pitch = 0.25f - GetAngleYZ();
        C3DVector up;
        up.InitialiseFromAngles(heading, pitch, 1.0f);
        return (float)acos(up.Dot(RHSet.Up)) * 0.15915494f;
    }
};

static bool Near(float a, float b) { return fabs(a - b) < 0.002f; }

int main()
{
    CCamera camera;
    camera.WorldPosition = C3DVector(0.0f, 0.0f, 0.0f);
    camera.RHSet.Forward = C3DVector(0.0f, 1.0f, 0.0f);
    camera.RHSet.Up = C3DVector(0.0f, 0.0f, 1.0f);              // unrolled: no roll
    float a = camera.GetAngleXZ();
    camera.RHSet.Up = C3DVector(1.0f, 0.0f, 0.0f);              // rolled a quarter turn
    float b = camera.GetAngleXZ();
    camera.RHSet.Up = C3DVector(0.0f, 0.0f, -1.0f);             // upside down: half a turn
    float c = camera.GetAngleXZ();
    camera.RHSet.Forward = C3DVector(1.0f, 0.0f, 0.0f);         // other heading, unrolled
    camera.RHSet.Up = C3DVector(0.0f, 0.0f, 1.0f);
    float d = camera.GetAngleXZ();
    if (!Near(a, 0.0f) || !Near(b, 0.25f) || !Near(c, 0.5f) || !Near(d, 0.0f)) {
        std::printf("00a0c450_TEST FAIL\n");
        return 1;
    }
    std::printf("00a0c450_TEST PASS\n");
    return 0;
}