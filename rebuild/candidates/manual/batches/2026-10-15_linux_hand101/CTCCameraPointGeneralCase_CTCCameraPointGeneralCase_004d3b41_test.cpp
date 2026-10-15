// Behaviour test for CTCCameraPointGeneralCase::CTCCameraPointGeneralCase (0x004D3B41): base built with the parent,
// then string length 5, cage radius and height offset 2, the three stick/target flags on.
#include <cstdio>

struct CThing { int id; };
static CThing* g_baseParent = 0;

struct CTCCameraPointShapeActivationBase {
    virtual ~CTCCameraPointShapeActivationBase() { }
    CTCCameraPointShapeActivationBase(CThing* parent) { g_baseParent = parent; }
};

struct CTCCameraPointGeneralCase : CTCCameraPointShapeActivationBase {
    float CameraStringLength;
    float CameraCageRadius;
    float CameraHeightOffset;
    bool  AllowRightStickZoom;
    bool  AllowRightStickRotation;
    bool  AllowZTarget;
    bool  AutoGoBehind;
    float AutoGoBehindTime;
    CTCCameraPointGeneralCase(CThing* parent);
};

CTCCameraPointGeneralCase::CTCCameraPointGeneralCase(CThing* parent)
    : CTCCameraPointShapeActivationBase(parent)
{
    CameraHeightOffset = CameraCageRadius = 2.0f;
    CameraStringLength = 5.0f;
    AllowRightStickZoom = true;
    AllowRightStickRotation = true;
    AllowZTarget = true;
}

int main()
{
    CThing thing;
    CTCCameraPointGeneralCase c(&thing);
    if (g_baseParent != &thing || c.CameraStringLength != 5.0f || c.CameraCageRadius != 2.0f || c.CameraHeightOffset != 2.0f
        || !c.AllowRightStickZoom || !c.AllowRightStickRotation || !c.AllowZTarget) {
        std::printf("004d3b41_TEST FAIL\n"); return 1;
    }
    std::printf("004d3b41_TEST PASS\n");
    return 0;
}
