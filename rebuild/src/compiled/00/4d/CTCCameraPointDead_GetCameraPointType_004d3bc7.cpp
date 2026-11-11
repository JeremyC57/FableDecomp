// CTCCameraPointDead::GetCameraPointType() const -- the camera-point type of the dead camera (5)
// Leaf: ?GetCameraPointType@CTCCameraPointDead@@UBE?AW4ECameraPointType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3BC7, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 28 of CTCCameraPointDead's vtable 0x0123B23C (the slot holds this body in 1 vtable(s), owner CTCCameraPointDead); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts GetCameraPointType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetCameraPointType@CTCCameraPointDead@@UBE?AW4ECameraPointType@@XZ; `ret` matches the signature.
// Was: CTCCameraPointDead::vfunc_28 (rebuild/src/compiled/00/4d/CTCCameraPointDead_vfunc28_004d3bc7.cpp).
// No shared header for CTCCameraPointDead: minimal local definition on its RTTI base CTCCameraPointShapeActivationBase.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCameraPointGeneralCase.h"
class CTCCameraPointDead : public CTCCameraPointShapeActivationBase {
public:
    virtual ECameraPointType GetCameraPointType() const;            // slot 28
};
ECameraPointType CTCCameraPointDead::GetCameraPointType() const
{
    return (ECameraPointType)5;
}
