#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 008fa7e0: GetVelocity returns a copy of LinkedState->Velocity (not Position or Acceleration).
class C3DVector { public:
    float x; float y; float z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
};
struct CPhysicsState {
    C3DVector Position;
    C3DVector Velocity;
    C3DVector Acceleration;
    const C3DVector& GetVelocity() const { return Velocity; }
};
struct CPhysicsForceModelBase {
    virtual void Slot00() {}
    CPhysicsState* LinkedState;
    C3DVector GetVelocity() const;
};
C3DVector CPhysicsForceModelBase::GetVelocity() const
{
    return LinkedState->GetVelocity();
}
int main()
{
    CPhysicsState s;
    s.Position.x = 1; s.Position.y = 2; s.Position.z = 3;
    s.Velocity.x = 4; s.Velocity.y = 5; s.Velocity.z = 6;
    s.Acceleration.x = 7; s.Acceleration.y = 8; s.Acceleration.z = 9;
    CPhysicsForceModelBase m; m.LinkedState = &s;
    C3DVector v = m.GetVelocity();
    if (v.x != 4 || v.y != 5 || v.z != 6) { std::printf("008fa7e0_TEST FAIL\n"); return 1; }
    std::printf("008fa7e0_TEST PASS\n");
    return 0;
}