// Behaviour test for 0x007BEDD0 NThingShot::CTCShotDefinitionBase::GetShootAtPos (model): returns TargetPos (+0x51,
// 1-aligned) by value.
#include <cstdio>
#include <cstring>
#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class CTCShotDefinitionBase {
public:
    unsigned char _pad00[0x51];
    C3DVector     TargetPos;                        // +0x51
    C3DVector GetShootAtPos() const { return TargetPos; }
};
#pragma pack(pop)
int main()
{
    CTCShotDefinitionBase shot;
    std::memset(&shot, 0x5a, sizeof(shot));
    shot.TargetPos.X = 1.5f; shot.TargetPos.Y = -2.25f; shot.TargetPos.Z = 3.75f;
    C3DVector v = shot.GetShootAtPos();
    int ok = sizeof(CTCShotDefinitionBase) == 0x5d && v.X == 1.5f && v.Y == -2.25f && v.Z == 3.75f
             && ((const unsigned char*)&shot)[0x50] == 0x5a;
    std::printf(ok ? "007bedd0_TEST PASS\n" : "007bedd0_TEST FAIL\n");
    return ok ? 0 : 1;
}
