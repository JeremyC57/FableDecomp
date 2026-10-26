// Behaviour test for 0x00851E70 (CScriptModuleGoToLookInDirection::Update): the owner's position becomes
// GoTo and its look-at GoTo + LookDirection.
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector operator+(const C3DVector& other) const
    {
        C3DVector result;
        result.X = X + other.X;
        result.Y = Y + other.Y;
        result.Z = Z + other.Z;
        return result;
    }
};

struct CCameraState {
    C3DVector LookAt;
    C3DVector Position;
};

struct CScriptModuleGoToLookInDirection {
    CCameraState* POwner;
    C3DVector     GoTo;
    C3DVector     LookDirection;
    void Update();
};

void CScriptModuleGoToLookInDirection::Update()
{
    POwner->Position = GoTo;
    POwner->LookAt = GoTo + LookDirection;
}

int main()
{
    CCameraState cam;
    CScriptModuleGoToLookInDirection m;
    m.POwner = &cam;
    m.GoTo.X = 1.0f; m.GoTo.Y = 2.0f; m.GoTo.Z = 3.0f;
    m.LookDirection.X = 0.5f; m.LookDirection.Y = -1.0f; m.LookDirection.Z = 4.0f;
    m.Update();
    if (cam.Position.X != 1.0f || cam.Position.Y != 2.0f || cam.Position.Z != 3.0f ||
        cam.LookAt.X != 1.5f || cam.LookAt.Y != 1.0f || cam.LookAt.Z != 7.0f) {
        std::printf("00851e70_TEST FAIL\n");
        return 1;
    }
    std::printf("00851e70_TEST PASS\n");
    return 0;
}