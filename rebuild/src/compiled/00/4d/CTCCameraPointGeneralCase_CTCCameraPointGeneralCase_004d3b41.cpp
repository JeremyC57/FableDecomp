// CTCCameraPointGeneralCase::CTCCameraPointGeneralCase(CThing* parent) -- shape-activation base, then the camera defaults (string 5, cage radius and height offset 2, three stick/target flags on)
// Leaf: CTCCameraPointGeneralCase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3B41, length 55 bytes. Called only from the factory at 0x004D82A3
// (`new` of 0x54 bytes).
// Callees: CTCCameraPointShapeActivationBase::CTCCameraPointShapeActivationBase(CThing*) @ 0x004D3A03 (stores vtable
// 0x0123AEDC = CTCCameraPointShapeActivationBase; manifest label "CEnginePrimitiveGroupManager" is propagated).
// Label correction: the chunk calls this CTCSmokeGenerator::CTCSmokeGenerator; the vtable it stores, 0x0123B194, is
// CTCCameraPointGeneralCase (rtti-vtables.tsv), and the PDB CTCCameraPointGeneralCase (0x54 bytes) matches every store:
// CameraStringLength +0x40, CameraCageRadius +0x44, CameraHeightOffset +0x48, AllowRightStickZoom +0x4c,
// AllowRightStickRotation +0x4d, AllowZTarget +0x4e (AutoGoBehind +0x4f and AutoGoBehindTime +0x50 are left alone).
// Shape: /Os pass-through base argument. 0x0122DCDC holds 2.0f.
#pragma optimize("s", on)
struct CThing;

struct CTCCameraPointShapeActivationBase {
    virtual void v00();
    unsigned char _body[0x40 - 4];                          // CTCBase + the camera-point definition fields
    CTCCameraPointShapeActivationBase(CThing* parent);      // 0x004D3A03
};

struct CTCCameraPointGeneralCase : CTCCameraPointShapeActivationBase {
    float CameraStringLength;                               // +0x40
    float CameraCageRadius;                                 // +0x44
    float CameraHeightOffset;                               // +0x48
    bool  AllowRightStickZoom;                              // +0x4c
    bool  AllowRightStickRotation;                          // +0x4d
    bool  AllowZTarget;                                     // +0x4e
    bool  AutoGoBehind;                                     // +0x4f
    float AutoGoBehindTime;                                 // +0x50
    virtual void v00();                                     // own vtable 0x0123B194
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