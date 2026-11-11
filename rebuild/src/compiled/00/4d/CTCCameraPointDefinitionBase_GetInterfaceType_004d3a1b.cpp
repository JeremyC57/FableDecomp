// CTCCameraPointDefinitionBase::GetInterfaceType() const -- the component's interface type, TCI_CAMERA_POINT (0x3F), shared by every camera-point leaf
// Leaf: ?GetInterfaceType@CTCCameraPointDefinitionBase@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3A1B, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCCameraPointScriptedSpline's vtable 0x0126A0BC (the slot holds this body in 10 vtable(s), owner CTCCameraPointDefinitionBase); the Xbox 360 Test PDB virtual order (third source) aligned on 18 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCCameraPointDefinitionBase@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCCameraPointShapeActivationBase::vfunc_21 (rebuild/src/compiled/00/4d/CTCCameraPointShapeActivationBase_vfunc21_004d3a1b.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCameraPointGeneralCase.h"
ETCInterfaceType CTCCameraPointDefinitionBase::GetInterfaceType() const
{
    return TCI_CAMERA_POINT;
}
