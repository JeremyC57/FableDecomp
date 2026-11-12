// CTCCameraPointTrack::GetCameraPointType() const -- the camera-point type of the track (3)
// Leaf: ?GetCameraPointType@CTCCameraPointTrack@@UBE?AW4ECameraPointType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3B2A, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 28 of CTCCameraPointTrack's vtable 0x0123B0E4 (the slot holds this body in 1 vtable(s), owner CTCCameraPointTrack); the Xbox 360 Test PDB virtual order (third source) aligned on 20 TLC-named slots of that vtable puts GetCameraPointType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetCameraPointType@CTCCameraPointTrack@@UBE?AW4ECameraPointType@@XZ; `ret` matches the signature.
// Was: CTCCameraPointTrack::vfunc_28 (rebuild/src/compiled/00/4d/CTCCameraPointTrack_vfunc28_004d3b2a.cpp).
// No shared header for CTCCameraPointTrack: minimal local definition on its RTTI base CTCCameraPointShapeActivationBase.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCameraPointGeneralCase.h"
class CTCCameraPointTrack : public CTCCameraPointShapeActivationBase {
public:
    virtual ECameraPointType GetCameraPointType() const;            // slot 28
};
ECameraPointType CTCCameraPointTrack::GetCameraPointType() const
{
    return (ECameraPointType)3;
}
