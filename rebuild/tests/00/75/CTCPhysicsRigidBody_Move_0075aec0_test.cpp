// behaviour test: CTCPhysicsRigidBody::Move -- step the body one frame, take its orientation's Z/Y rows as the
// right-handed set, and place the thing so that its centre of mass (local CMPosition offset rotated by that set's
// matrix) lands on the body's simulated centre of mass.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void TransformVector(C3DVector& v) const
    {
        float x = v.X, y = v.Y;
        v.X = x * E11 + y * E21 + v.Z * E31;
        v.Y = x * E12 + y * E22 + v.Z * E32;
        v.Z = x * E13 + y * E23 + v.Z * E33;
    }
};
struct Body {
    float simulatedFor;
    C3DVector CMPosition, CurrentCM, RowY, RowZ;
    C3DVector up, forward, position;
    void Simulate(float dt) { simulatedFor = dt; }
    void Move(long framesPerSecond)
    {
        Simulate(1.0f / (float)framesPerSecond);
        up = RowZ; forward = RowY;
        // GetPositionMatrixFromRHSet stand-in: right = forward x up, rows right/forward/up
        CMatrix3x4 m;
        m.E11 = forward.Y * up.Z - forward.Z * up.Y; m.E12 = forward.Z * up.X - forward.X * up.Z; m.E13 = forward.X * up.Y - forward.Y * up.X;
        m.E21 = forward.X; m.E22 = forward.Y; m.E23 = forward.Z;
        m.E31 = up.X; m.E32 = up.Y; m.E33 = up.Z;
        m.E41 = m.E42 = m.E43 = 0.0f;
        C3DVector offset = CMPosition;
        m.TransformVector(offset);
        position.X = CurrentCM.X - offset.X; position.Y = CurrentCM.Y - offset.Y; position.Z = CurrentCM.Z - offset.Z;
    }
};
int main()
{
    Body b;
    b.CMPosition.X = 1.0f; b.CMPosition.Y = 2.0f; b.CMPosition.Z = 3.0f;
    b.CurrentCM.X = 10.0f; b.CurrentCM.Y = 20.0f; b.CurrentCM.Z = 30.0f;
    b.RowY.X = 0.0f; b.RowY.Y = 1.0f; b.RowY.Z = 0.0f;       // forward = +Y
    b.RowZ.X = 0.0f; b.RowZ.Y = 0.0f; b.RowZ.Z = 1.0f;       // up = +Z  -> identity matrix
    b.Move(30);
    if (b.simulatedFor < 0.0333f || b.simulatedFor > 0.0334f
        || b.position.X != 9.0f || b.position.Y != 18.0f || b.position.Z != 27.0f) {
        std::printf("0075aec0_TEST FAIL\n"); return 1;
    }
    std::printf("0075aec0_TEST PASS\n");
    return 0;
}