// NCameraModes::CStringAndCageBase::UpdateZVars() -- ease the camera's current height offset toward the desired one above the observed thing (clamped to the world height limits when they apply), faster when it has to drop
// Retail: Fable.exe (TLC, VC7.1) @ 0x00872050, 183 bytes (+ int3 padding), `ret`. Ego_r.pdb
//   `?UpdateZVars@CStringAndCageBase@NCameraModes@@AAEXXZ` (private).
// Leaf: UpdateZVars
// Callees: NCameraModes::CBase::GetObservedThing() const @ 0x0084F1B0, CThing::GetPosition() const @ 0x004C73D0 (const C3DVector&),
//   NCameraModes::CBase::GetCamTools() const @ 0x0084D080 (result unused); float pool 10.0f @ 0x01237F0C, 3.0f @ 0x0122DED4,
//   1.0f @ 0x0122DED8.
// Shape: the thing's Z stays on the x87 stack while the target (z + DesiredHeightOffset) is formed; the world-height bounds are
//   named float locals of the `if` block (copied into the frame as dwords, lo through ecx) handed by value to an inline
//   Clamp(float& value, lo, hi) that rewrites the target in place on the stack top (`v < lo` / `v > hi`; a by-value
//   result spills the target, a two-float struct argument moves the result's frame word); the offset is `target - z`; the easing rate is a literal ternary (10 when dropping, else 3) and the new offset
//   `(CurrentHeightOffset * rate + offset) / (rate + 1)` is parked in a frame word across the GetCamTools call.
// Retail layout (Ego_r.pdb - 0x24 for this class, as the landed CStringAndCage ctor 0x0069DE30): CBase::CameraManager +0x40,
//   ViewOccluded +0xA0; DesiredHeightOffset +0xA4, CurrentHeightOffset +0xB0. CGameCameraManager (retail): MinWorldHeight
//   +0xF8, MaxWorldHeight +0xFC, LimitMinMaxWorldHeight +0x152 (PDB +0x108 / +0x10C / +0x16E).
struct C3DVector { float X, Y, Z; };
class CThing {
public:
    const C3DVector& GetPosition() const;                  // 0x004C73D0
};
class CThingGameObject : public CThing { };
struct CGameCameraManager {
    unsigned char _pad000[0xf8];
    float         MinWorldHeight;                          // +0xf8
    float         MaxWorldHeight;                          // +0xfc
    unsigned char _pad100[0x152 - 0x100];
    bool          LimitMinMaxWorldHeight;                  // +0x152
};
namespace NCameraModes {
class CTools { unsigned char _body[0x24]; };          // CGameCameraManager::CamTools (+0x78)
static inline void Clamp(float& value, float lo, float hi)
{
    if (value < lo)
        value = lo;
    if (value > hi)
        value = hi;
}
class CBase {
public:
    virtual void v00();
    float               DefaultFOV;                        // +0x04
    unsigned char       _pad08[0x40 - 0x08];
    CGameCameraManager* CameraManager;                     // +0x40
    unsigned char       _pad44[0xa0 - 0x44];
    bool                ViewOccluded;                      // +0xa0
    const CThingGameObject* GetObservedThing() const;      // 0x0084F1B0
    const CTools& GetCamTools() const;                     // 0x0084D080
};
class CStringAndCageBase : public CBase {
public:
    float DesiredHeightOffset;                             // +0xa4
    float DesiredStringLength;                             // +0xa8
    float DesiredCageRadius;                               // +0xac
    float CurrentHeightOffset;                             // +0xb0
    float CurrentStringLength;                             // +0xb4
    float CurrentCageRadius;                               // +0xb8
    void UpdateZVars();
};
void CStringAndCageBase::UpdateZVars()
{
    float z = GetObservedThing()->GetPosition().Z;
    float target = z + DesiredHeightOffset;
    if (CameraManager->LimitMinMaxWorldHeight && !ViewOccluded) {
        float minHeight = CameraManager->MinWorldHeight;
        float maxHeight = CameraManager->MaxWorldHeight;
        Clamp(target, minHeight, maxHeight);
    }
    float offset = target - z;
    float rate = offset < CurrentHeightOffset ? 10.0f : 3.0f;
    float height = (CurrentHeightOffset * rate + offset) / (rate + 1.0f);
    GetCamTools();
    CurrentHeightOffset = height;
}
}