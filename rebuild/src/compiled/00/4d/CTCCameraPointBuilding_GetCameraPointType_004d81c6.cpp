// CTCCameraPointBuilding::GetCameraPointType() const -- the camera-point type of the building camera (2)
// Leaf: ?GetCameraPointType@CTCCameraPointBuilding@@UBE?AW4ECameraPointType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D81C6, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 28 of CTCCameraPointBuilding's vtable 0x0123ECBC (the slot holds this body in 1 vtable(s), owner CTCCameraPointBuilding); the Xbox 360 Test PDB virtual order (third source) aligned on 18 TLC-named slots of that vtable puts GetCameraPointType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetCameraPointType@CTCCameraPointBuilding@@UBE?AW4ECameraPointType@@XZ; `ret` matches the signature.
// Was: CTCCameraPointBuilding::vfunc_28 (rebuild/src/compiled/00/4d/CTCCameraPointBuilding_vfunc28_004d81c6.cpp).
// No shared header for CTCCameraPointBuilding: minimal local definition on its RTTI base CTCCameraPointDefinitionBase.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCameraPointGeneralCase.h"
class CTCCameraPointBuilding : public CTCCameraPointDefinitionBase {
public:
    virtual ECameraPointType GetCameraPointType() const;            // slot 28
};
ECameraPointType CTCCameraPointBuilding::GetCameraPointType() const
{
    return (ECameraPointType)2;
}
