// CTCLight::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCLight@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006423F0, thiscall, `ret`.
// Identity: slot 8 of CTCLight's vtable 0x0125A194 (defined here: the slot holds this body in 1 vtable(s), all of CTCLight or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCLight@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCLight.h (opt-ins as in the landed row CTCLight_DrawFlare_00641ae0.cpp).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CLIGHTDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#include "engine/CThing.h"
#include "engine/CTCLight.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CEnginePrimitiveFlareSprite.h"
bool CTCLight::HasFrameUpdate() const
{
    return true;
}
