// NCameraModes::CBase::UpdateRaiseUp(C3DVector&) const -- raises the camera height near the observed thing: blends the camera's height above the thing by tanh of the squared ground distance and adds the thing's focal height (+0.28) faded by exp of minus a quarter of that distance
// Leaf: ?UpdateRaiseUp@CBase@NCameraModes@@
// Grade: DIFFER(194v193) -- one instruction: retail reads the focal Z through GetFocalPos's returned pointer (`fld [eax+8]`) and parks the height in the dead temporary's Z slot (frame 0xC); this named-local form reads `fld [esp+14h]` (1 byte longer, else identical). The `float h = GetFocalPos(1.0f).Z - z;` form reads eax but gives h its own slot (frame 0x10, 193v193 with every frame offset shifted), also with a class-type C3DVector. Tried: one expression (lane evaluates exp first and spills), split tanh statement (fixed the order), appearance local (fixed the push order), named focal vector, ctor'd C3DVector.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084D7E0, 193 bytes, __thiscall, `ret 4`.
// Identity: Ego_r.pdb ?UpdateRaiseUp@CBase@NCameraModes@@IBEXAAVC3DVector@@@Z, directly before GetObservedThingCurrentMeshHeight
//   (TLC 0x0084D8B0, landed) as in Ego_r (0x00845340 / 0x00845420); caller 0x0084DDE5. The manifest label
//   "CTCGraphicAppearance_UpdateHeightInterpolation" is an autoname.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (the manager's observed thing, +0x40 -> +0x100, the inline accessor
//   of the landed 0x0084D8B0), CThing::GetPosition 0x004C73D0, CTCGraphicAppearance::GetFocalPos(float) 0x004C1060 (C3DVector by
//   hidden pointer; TLC order GetFocalPos 0x004C1060 .. GetCameraFocalPos 0x004C12D0 as Ego_r 0x004C2720 .. 0x004C2980),
//   _CItanh 0x00BFECE2 (the tanh intrinsic helper); exp is expanded inline.
// Constants: 0.25f @ 0x0123078C, -0.25f @ 0x01230C8C, 0.28f @ 0x012758D8. Thing appearance TC at +0x64.
// Shape: the squared XY distance is a float local homed in the dead `pos` argument slot, computed in its own block so the
//   two deltas are popped from the FPU stack after the store.
#include <math.h>
#pragma intrinsic(tanh, exp)

class C3DVector {
public:
    float X, Y, Z;
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    C3DVector GetFocalPos(float scale) const;                     // 0x004C1060
};
}

class CThing {
public:
    const C3DVector& GetPosition() const;                         // 0x004C73D0
    unsigned char _pad_0x00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
};

class CBaseIntelligentPointer {
public:
    CThing* GetPItem() const;                                     // 0x00A01B50
};

class CGameCameraManagerView {
public:
    CThing* GetObservedThing() const { return ObservedThing.GetPItem(); }
    unsigned char           _pad_0x00[0x100];
    CBaseIntelligentPointer ObservedThing;                        // +0x100
};

namespace NCameraModes {
class CBase {
protected:
    void UpdateRaiseUp(C3DVector& pos) const;
public:
    unsigned char           _pad_0x00[0x40];
    CGameCameraManagerView* CameraManager;                        // +0x40
};

void CBase::UpdateRaiseUp(C3DVector& pos) const
{
    const C3DVector& thingPos = CameraManager->GetObservedThing()->GetPosition();
    float distanceSquared;
    {
        float dx = thingPos.X - pos.X;
        float dy = thingPos.Y - pos.Y;
        distanceSquared = dy * dy + dx * dx;
    }
    const NGraphicAppearance::CTCGraphicAppearance* appearance = CameraManager->GetObservedThing()->PTCGraphicAppearance;
    C3DVector focal = appearance->GetFocalPos(1.0f);
    focal.Z -= thingPos.Z;
    float raise = (float)tanh(distanceSquared * 0.25f) * (pos.Z - thingPos.Z);
    pos.Z = raise + (float)exp(distanceSquared * -0.25f) * (focal.Z + 0.28f) + thingPos.Z;
}
}
