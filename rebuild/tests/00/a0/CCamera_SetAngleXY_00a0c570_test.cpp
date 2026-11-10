// Behaviour test for 0x00A0C570: CCamera::SetAngleXY (new heading, each vector keeps its own pitch, both normalised)
#include <cstdio>
#include <cmath>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float GetAngleYZ() const {
        float angle = (float)atan2(Z, sqrt(Y * Y + X * X)) * 0.15915494f;
        if (angle < 0.0f || angle >= 1.0f) {
            angle = (float)fmod((double)angle, 1.0);
            if (angle < 0.0f) angle += 1.0f;
        }
        return angle;
    }
    const C3DVector& InitialiseFromAngles(float angleXY, float angleYZ, float length) {
        float cosYZ = (float)cos(angleYZ * (double)6.2831855f);
        X = (float)(sin(angleXY * (double)6.2831855f) * (cosYZ * length));
        Y = (float)(cos(angleXY * (double)6.2831855f) * (cosYZ * length));
        Z = (float)(sin(angleYZ * (double)6.2831855f) * length);
        return *this;
    }
    C3DVector& AccurateNormalise() {
        float m = (float)sqrt(X * X + Y * Y + Z * Z);
        X /= m; Y /= m; Z /= m;
        return *this;
    }
};

class CRightHandedSet {
public:
    C3DVector Up, Forward;
    const C3DVector& GetUp() const { return Up; }
    const C3DVector& GetForward() const { return Forward; }
    void Set(const C3DVector& up, const C3DVector& forward) { Up = up; Forward = forward; }
    void Normalise() { Up.AccurateNormalise(); Forward.AccurateNormalise(); }
};

class CCamera {
public:
    C3DVector WorldPosition;
    CRightHandedSet RHSet;
    void SetAngleXY(float angle);
};

void CCamera::SetAngleXY(float angle)
{
    C3DVector forward;
    forward.InitialiseFromAngles(angle, RHSet.GetForward().GetAngleYZ(), 1.0f);
    C3DVector up;
    up.InitialiseFromAngles(angle, RHSet.GetUp().GetAngleYZ(), 1.0f);
    RHSet.Set(up, forward);
    RHSet.Normalise();
}

static int Near(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    int fails = 0;
    CCamera cam;
    cam.WorldPosition = C3DVector(5.0f, 6.0f, 7.0f);
    cam.RHSet.Up = C3DVector(0.0f, 0.0f, 1.0f);
    cam.RHSet.Forward = C3DVector(0.0f, 2.0f, 0.0f);
    cam.SetAngleXY(0.25f);                          // heading a quarter turn: forward now +X, up stays +Z
    if (!Near(cam.RHSet.Forward.X, 1.0f) || !Near(cam.RHSet.Forward.Y, 0.0f) || !Near(cam.RHSet.Forward.Z, 0.0f)) { std::printf("forward 1\n"); ++fails; }
    if (!Near(cam.RHSet.Up.X, 0.0f) || !Near(cam.RHSet.Up.Y, 0.0f) || !Near(cam.RHSet.Up.Z, 1.0f)) { std::printf("up 1\n"); ++fails; }
    if (cam.WorldPosition.X != 5.0f || cam.WorldPosition.Z != 7.0f) { std::printf("position\n"); ++fails; }
    cam.RHSet.Forward = C3DVector(0.0f, 1.0f, -1.0f);   // pitch -1/8 turn wraps to 7/8 through fmod
    cam.RHSet.Up = C3DVector(0.0f, -1.0f, 1.0f);        // pitch +1/8 turn
    cam.SetAngleXY(0.5f);                           // heading half a turn: -Y
    if (!Near(cam.RHSet.Forward.X, 0.0f) || !Near(cam.RHSet.Forward.Y, -0.70710677f) || !Near(cam.RHSet.Forward.Z, -0.70710677f)) { std::printf("forward 2\n"); ++fails; }
    if (!Near(cam.RHSet.Up.Y, -0.70710677f) || !Near(cam.RHSet.Up.Z, 0.70710677f)) { std::printf("up 2\n"); ++fails; }
    if (fails) { std::printf("00a0c570_TEST FAIL\n"); return 1; }
    std::printf("00a0c570_TEST PASS\n");
    return 0;
}