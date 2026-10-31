// NCameraModes::CBase::UpdateRaiseUp(C3DVector& pos) const -- raises the camera height near the observed thing: blends the camera's height above the thing by tanh of the squared ground distance and adds the thing's focal height (+0.28) faded by exp of minus a quarter of that distance
// Leaf: ?UpdateRaiseUp@CBase@NCameraModes@@IBEXAAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084D7E0, 193 bytes, __fastcall, `ret 4`.
// Identity: Ego_r.pdb ?UpdateRaiseUp@CBase@NCameraModes@@IBEXAAVC3DVector@@@Z (Ego_r 0x00845340). Donor run Ego_r 0x008452E0..0x00845420
//   (GetDefaultFOV, GetDefaultRollAngle, GetDefaultObservedPositionZOffset, IsRaiseUpOnHeroNearby, IsStretchAroundHero, UpdateRaiseUp,
//   GetObservedThingCurrentMeshHeight) lines up 1:1 with TLC 0x0084D790 (landed GetDefaultFOV), 0x0084D7A0/7B0/7C0/7D0, this row,
//   0x0084D8B0 (landed GetObservedThingCurrentMeshHeight); single caller 0x0084DDE5 (FrameUpdate). The manifest label
//   "CTCGraphicAppearance_UpdateHeightInterpolation" is an autoname.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (the camera manager's PObservedThing at +0x100, through the inline
//   CBase::GetObservedThing, Ego_r ?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ), CThing::GetPos 0x004C73D0,
//   NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float) 0x004C1060 (C3DVector by hidden pointer), _CItanh 0x00BFECE2
//   (the tanh intrinsic helper); exp is expanded inline.
// Constants: 0.25f @ 0x0123078C, -0.25f @ 0x01230C8C, 0.28f @ 0x012758D8.
// Layout: CThing::PTCGraphicAppearance at retail +0x64 (Ego_r +0x74: STLport bitset/CVectorMap are smaller); the CBase member at
//   +0x40 (the camera manager) has no PDB layout record -- descriptive name.
// Shape: the squared XY distance is a float local homed in the dead `pos` argument slot, computed in its own block so the two
//   deltas are popped from the FPU stack after the store. The focal height is `offset.Z = GetFocalPos(1.0f).Z - thingPos.Z` on an
//   otherwise unused C3DVector local: the returned temporary's Z is read through GetFocalPos's result pointer (`fld [eax+8]`) and
//   the local shares the dead temporary's frame words (frame 0xC). A named float (own slot, frame 0x10) or a whole focal vector
//   with `-=` (reads the frame copy, 194 bytes) both miss. The observed thing comes through an inline accessor ON THE MANAGER
//   (manager pointer in eax, `lea ecx,[eax+100h]`); reading the manager's member from CBase directly gives `add ecx,100h`.
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

class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseObject* GetPItem() const;                                // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CThing {
public:
    const C3DVector& GetPos() const;                              // 0x004C73D0
    unsigned char _pad_0x00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
};
class CThingGameObject : public CThing {
};

#define CGameCameraManager CGameCameraManagerLayout
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
class CGameCameraManager : public CGameCameraManagerLayout {   // PObservedThing (CIntelligentPointer<const CThingGameObject>) at +0x100
public:
    const CThingGameObject* GetObservedThing() const   // inline accessor (name descriptive)
    {
        return reinterpret_cast<const CIntelligentPointer<const CThingGameObject>&>(PObservedThing).GetPItem();
    }
};

namespace NCameraModes {
class CBase {
public:
    const CThingGameObject* GetObservedThing() const { return CameraManager->GetObservedThing(); }
protected:
    void UpdateRaiseUp(C3DVector& pos) const;
public:
    unsigned char       _pad_0x00[0x40];
    CGameCameraManager* CameraManager;                            // +0x40 (descriptive)
};

void CBase::UpdateRaiseUp(C3DVector& pos) const
{
    const C3DVector& thingPos = GetObservedThing()->GetPos();
    float distanceSquared;
    {
        float dx = thingPos.X - pos.X;
        float dy = thingPos.Y - pos.Y;
        distanceSquared = dy * dy + dx * dx;
    }
    const NGraphicAppearance::CTCGraphicAppearance* appearance = GetObservedThing()->PTCGraphicAppearance;
    C3DVector offset;
    offset.Z = appearance->GetFocalPos(1.0f).Z - thingPos.Z;
    float raise = (float)tanh(distanceSquared * 0.25f) * (pos.Z - thingPos.Z);
    pos.Z = raise + (float)exp(distanceSquared * -0.25f) * (offset.Z + 0.28f) + thingPos.Z;
}
}