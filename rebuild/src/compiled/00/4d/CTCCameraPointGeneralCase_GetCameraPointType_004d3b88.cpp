// CTCCameraPointGeneralCase::GetCameraPointType() const -- the camera-point type of the general case (4)
// Leaf: ?GetCameraPointType@CTCCameraPointGeneralCase@@UBE?AW4ECameraPointType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3B88, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 28 of CTCCameraPointGeneralCase's vtable 0x0123B194 (the slot holds this body in 1 vtable(s), owner CTCCameraPointGeneralCase); the Xbox 360 Test PDB virtual order (third source) aligned on 18 TLC-named slots of that vtable puts GetCameraPointType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetCameraPointType@CTCCameraPointGeneralCase@@UBE?AW4ECameraPointType@@XZ; `ret` matches the signature.
// Was: CTCCameraPointGeneralCase::vfunc_28 (rebuild/src/compiled/00/4d/CTCCameraPointGeneralCase_vfunc28_004d3b88.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCameraPointGeneralCase.h"
ECameraPointType CTCCameraPointGeneralCase::GetCameraPointType() const
{
    return (ECameraPointType)4;
}
