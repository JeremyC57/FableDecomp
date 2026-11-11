// CTCCameraPointDefinitionBase::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCCameraPointDefinitionBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3A26, thiscall, `ret`.
// Identity: slot 23 of CTCCameraPointScriptedSpline's vtable 0x0126A0BC (defined here: the slot holds this body in 10 vtable(s), all of CTCCameraPointDefinitionBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 18 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCCameraPointDefinitionBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCCameraPointGeneralCase.h with the overlay opt-in FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS (needs the polymorphic FABLE_RETAIL_CTCBASE).
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/CTCCameraPointGeneralCase.h"
bool CTCCameraPointDefinitionBase::IsComponentSerialisable() const
{
    return true;
}
