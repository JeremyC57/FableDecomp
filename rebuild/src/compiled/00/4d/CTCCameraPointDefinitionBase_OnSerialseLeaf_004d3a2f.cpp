// CTCCameraPointDefinitionBase::OnSerialseLeaf(CPersistContext&) -- does nothing
// Leaf: ?OnSerialseLeaf@CTCCameraPointDefinitionBase@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3A2F, thiscall, `ret 4`.
// Identity: slot 33 of CTCCameraPointDefinitionBase's vtable 0x01269B04 (defined here: the slot holds this body in 4 vtable(s), all of CTCCameraPointDefinitionBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts OnSerialseLeaf(CPersistContext &) at this slot; Ego_r/FableWin name set: ?OnSerialseLeaf@CTCCameraPointDefinitionBase@@UAEXAAVCPersistContext@@@Z; `ret` matches the signature.
// Header: engine/CTCCameraPointGeneralCase.h with the overlay opt-in FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS (needs the polymorphic FABLE_RETAIL_CTCBASE).
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/CTCCameraPointGeneralCase.h"
void CTCCameraPointDefinitionBase::OnSerialseLeaf(CPersistContext&)
{
}
