// Behaviour test for CEnginePrimitiveWeaponTrail::CWeaponTrailEntry::CWeaponTrailEntry (0x0076DE20): both points and
// the creation time are copied.
#include <cstdio>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {
    public:
        CWeaponTrailEntry(const C3DVector& top, const C3DVector& bottom, float creationTime);
        C3DVector Top;
        C3DVector Bottom;
        float     CreationTime;
    };
};
CEnginePrimitiveWeaponTrail::CWeaponTrailEntry::CWeaponTrailEntry(const C3DVector& top, const C3DVector& bottom, float creationTime)
    : Top(top), Bottom(bottom), CreationTime(creationTime)
{
}

int main()
{
    C3DVector top(1.0f, 2.0f, 3.0f), bottom(-4.0f, 5.5f, 0.25f);
    CEnginePrimitiveWeaponTrail::CWeaponTrailEntry e(top, bottom, 12.5f);
    if (e.Top.X != 1.0f || e.Top.Y != 2.0f || e.Top.Z != 3.0f || e.Bottom.X != -4.0f || e.Bottom.Y != 5.5f
        || e.Bottom.Z != 0.25f || e.CreationTime != 12.5f)
        { std::printf("0076de20_TEST FAIL\n"); return 1; }
    std::printf("0076de20_TEST PASS\n");
    return 0;
}