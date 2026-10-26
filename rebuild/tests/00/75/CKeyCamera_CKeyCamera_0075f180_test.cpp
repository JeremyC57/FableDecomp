// Behaviour test for 0x0075F180 CKeyCamera value constructor: arguments land in their fields, DeltaIndex/SpeedFraction zero, empty name.
#include <cstdio>
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class CCharString { public: const char* m_pData; CCharString() : m_pData("") { } };
struct CKeyCamera {
    float DeltaIndex; C3DVector Pos, LookDirection; float FOV, Roll, ShuttleSpeed, Duration, PauseTime; CCharString EventName; float SpeedFraction;
    CKeyCamera(const C3DVector& p, const C3DVector& l, float fov, float roll, float sh, float du, float pa)
        : DeltaIndex(0), Pos(p), LookDirection(l), FOV(fov), Roll(roll), ShuttleSpeed(sh), Duration(du), PauseTime(pa), SpeedFraction(0) { }
};
int main()
{
    CKeyCamera k(C3DVector(1, 2, 3), C3DVector(0, 1, 0), 60.0f, 5.0f, 2.0f, 3.0f, 0.5f);
    if (k.DeltaIndex != 0 || k.SpeedFraction != 0 || k.Pos.Y != 2 || k.LookDirection.Y != 1 || k.FOV != 60.0f || k.Roll != 5.0f ||
        k.ShuttleSpeed != 2.0f || k.Duration != 3.0f || k.PauseTime != 0.5f || k.EventName.m_pData[0] != 0) {
        std::printf("0075f180_TEST FAIL\n");
        return 1;
    }
    std::printf("0075f180_TEST PASS\n");
    return 0;
}