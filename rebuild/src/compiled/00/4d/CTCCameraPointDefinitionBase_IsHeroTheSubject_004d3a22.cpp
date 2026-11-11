// CTCCameraPointDefinitionBase::IsHeroTheSubject() const -- returns HeroIsSubject
// Leaf: ?IsHeroTheSubject@CTCCameraPointDefinitionBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3A22, thiscall, `ret`.
// Identity: slot 30 of CTCCameraPointScriptedSpline's vtable 0x0126A0BC (defined here: the slot holds this body in 10 vtable(s), all of CTCCameraPointDefinitionBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 18 TLC-named slots of that vtable puts IsHeroTheSubject(void) at this slot; Ego_r/FableWin name set: ?IsHeroTheSubject@CTCCameraPointDefinitionBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCCameraPointGeneralCase.h with the overlay opt-in FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS (needs the polymorphic FABLE_RETAIL_CTCBASE); HeroIsSubject is the header member at +0x15 (Ego_r layout, the offset retail reads).
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/CTCCameraPointGeneralCase.h"
bool CTCCameraPointDefinitionBase::IsHeroTheSubject() const
{
    return HeroIsSubject;
}
