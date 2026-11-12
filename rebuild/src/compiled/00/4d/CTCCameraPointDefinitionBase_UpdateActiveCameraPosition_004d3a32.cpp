// CTCCameraPointDefinitionBase::UpdateActiveCameraPosition() -- does nothing
// Leaf: ?UpdateActiveCameraPosition@CTCCameraPointDefinitionBase@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3A32, thiscall, `ret`.
// Identity: slot 34 of CTCCameraPointScriptedSpline's vtable 0x0126A0BC (defined here: the slot holds this body in 9 vtable(s), all of CTCCameraPointDefinitionBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 18 TLC-named slots of that vtable puts UpdateActiveCameraPosition(void) at this slot; Ego_r/FableWin name set: ?UpdateActiveCameraPosition@CTCCameraPointDefinitionBase@@MAEXXZ; `ret` matches the signature.
// Header: engine/CTCCameraPointGeneralCase.h with the overlay opt-in FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS (needs the polymorphic FABLE_RETAIL_CTCBASE).
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/CTCCameraPointGeneralCase.h"
void CTCCameraPointDefinitionBase::UpdateActiveCameraPosition()
{
}
