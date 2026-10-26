#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class C2DVector { public: float X, Y; };
struct CPhysicsState {
    C3DVector Position, Velocity, Acceleration;
    void* Attractors; void* IncidentalAttractors;
    bool CullPhysics;
    void TransferPersistantData(const CPhysicsState& s)
    { Position = s.Position; Velocity = s.Velocity; CullPhysics = s.CullPhysics; }
};
struct CBase {
    float DefaultFOV;
    CPhysicsState LookAt;
    CPhysicsState GoTo;
    float CameraFOV, RollAngle, ObservedPositionZOffset;
    C2DVector RightStickVector, LeftStickVector;
    void TransferPersistantBaseData(const CBase& other)
    {
        GoTo.TransferPersistantData(other.GoTo);
        LookAt.TransferPersistantData(other.LookAt);
        RightStickVector = other.RightStickVector;
        LeftStickVector = other.LeftStickVector;
        CameraFOV = other.CameraFOV;
        RollAngle = other.RollAngle;
        ObservedPositionZOffset = other.ObservedPositionZOffset;
    }
};
int main()
{
    CBase a, b;
    b.DefaultFOV = 9; a.DefaultFOV = 1;
    b.GoTo.Position.X = 1; b.GoTo.Position.Y = 2; b.GoTo.Position.Z = 3;
    b.GoTo.Velocity.X = 4; b.GoTo.Velocity.Y = 5; b.GoTo.Velocity.Z = 6; b.GoTo.CullPhysics = true;
    b.GoTo.Acceleration.X = 77; a.GoTo.Acceleration.X = 0;
    b.LookAt.Position.X = 7; b.LookAt.Velocity.Z = 8; b.LookAt.CullPhysics = false; a.LookAt.CullPhysics = true;
    b.CameraFOV = 60; b.RollAngle = 0.5f; b.ObservedPositionZOffset = 2;
    b.RightStickVector.X = 0.25f; b.LeftStickVector.Y = -1;
    a.TransferPersistantBaseData(b);
    if (a.GoTo.Position.Z != 3 || a.GoTo.Velocity.Y != 5 || !a.GoTo.CullPhysics || a.GoTo.Acceleration.X != 0 ||
        a.LookAt.Position.X != 7 || a.LookAt.Velocity.Z != 8 || a.LookAt.CullPhysics || a.CameraFOV != 60 ||
        a.RollAngle != 0.5f || a.ObservedPositionZOffset != 2 || a.RightStickVector.X != 0.25f ||
        a.LeftStickVector.Y != -1 || a.DefaultFOV != 1) { std::printf("0084cf80_TEST FAIL\n"); return 1; }
    std::printf("0084cf80_TEST PASS\n");
    return 0;
}