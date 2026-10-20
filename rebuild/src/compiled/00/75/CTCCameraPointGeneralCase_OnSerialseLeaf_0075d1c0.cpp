// CTCCameraPointGeneralCase::OnSerialseLeaf(CPersistContext& context) -- transfer the general-case camera point's string length, cage radius, height offset, right-stick and Z-target permissions and the auto-go-behind settings
// Leaf: OnSerialseLeaf
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075D1C0, length 235 bytes (+ int3 padding), `ret 4`. Vtable slot 33 ("vfunc_33"
// in the catalog; PDB ?OnSerialseLeaf@CTCCameraPointGeneralCase@@UAEXAAVCPersistContext@@@Z, sic).
// Callees: CPersistContext::Transfer(const char*, float&, const float&) @ 0x00410620 and Transfer(const char*, bool&, const bool&)
// @ 0x004045C0 (bool per the PDB members; most landed rows spell this address Transfer<signed char>).
// Layout = Ego_r.pdb CTCCameraPointGeneralCase (84 bytes): CameraStringLength +0x40, CameraCageRadius +0x44, CameraHeightOffset
// +0x48, AllowRightStickZoom +0x4C, AllowRightStickRotation +0x4D, AllowZTarget +0x4E, AutoGoBehind +0x4F, AutoGoBehindTime +0x50.
// Shape: one Transfer per member with a literal default (a const-reference temporary: the first in the frame word, the rest in
// the dead `context` slot; the `true` defaults share bl).
struct CPersistContext {
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
    void Transfer(const char* name, bool& value, const bool& defaultValue);     // 0x004045C0
};
class CTCCameraPointGeneralCase {
public:
    virtual void v00();
    unsigned char _base04[0x40 - 4];            // CTCCameraPointDefinitionBase
    float CameraStringLength;                   // +0x40
    float CameraCageRadius;                     // +0x44
    float CameraHeightOffset;                   // +0x48
    bool  AllowRightStickZoom;                  // +0x4c
    bool  AllowRightStickRotation;              // +0x4d
    bool  AllowZTarget;                         // +0x4e
    bool  AutoGoBehind;                         // +0x4f
    float AutoGoBehindTime;                     // +0x50
    void OnSerialseLeaf(CPersistContext& context);
};

void CTCCameraPointGeneralCase::OnSerialseLeaf(CPersistContext& context)
{
    context.Transfer("StringLength", CameraStringLength, 5.0f);
    context.Transfer("CageRadius", CameraCageRadius, 2.0f);
    context.Transfer("HeightOffset", CameraHeightOffset, 2.0f);
    context.Transfer("AllowRightStickZoom", AllowRightStickZoom, true);
    context.Transfer("AllowRightStickRotation", AllowRightStickRotation, true);
    context.Transfer("AllowZTarget", AllowZTarget, true);
    context.Transfer("AutoGoBehind", AutoGoBehind, true);
    context.Transfer("AutoGoBehindTime", AutoGoBehindTime, 3.0f);
}