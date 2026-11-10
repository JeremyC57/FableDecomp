// Behaviour test for CThingPhysical::PeekPos (0x006A4D00): the reference is the physics component's Position.
#include <stdio.h>
struct C3DVector { float X, Y, Z; };
class CTCPhysicsBase { public: void* vptr; void* Parent; long Pad; C3DVector Position;
    const C3DVector& GetPosition() const { return Position; } };
class CThingPhysical { public: CTCPhysicsBase* PhysicsTC;
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const C3DVector& PeekPos() const { return GetPhysicsTC()->GetPosition(); } };
int main()
{
    CTCPhysicsBase tc; tc.Position.X = 1.0f; tc.Position.Y = 2.0f; tc.Position.Z = 3.0f;
    CThingPhysical thing; thing.PhysicsTC = &tc;
    const C3DVector& p = thing.PeekPos();
    if (&p != &tc.Position || p.Y != 2.0f) { printf("006a4d00_TEST FAIL\n"); return 1; }
    tc.Position.Z = 9.0f;
    if (thing.PeekPos().Z != 9.0f) { printf("006a4d00_TEST FAIL\n"); return 1; }
    printf("006a4d00_TEST PASS\n");
    return 0;
}
