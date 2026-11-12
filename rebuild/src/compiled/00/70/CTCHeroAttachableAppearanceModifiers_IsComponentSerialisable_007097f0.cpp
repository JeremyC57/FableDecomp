// CTCHeroAttachableAppearanceModifiers::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCHeroAttachableAppearanceModifiers@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007097F0, thiscall, `ret`.
// Identity: slot 23 of CTCHeroAttachableAppearanceModifiers's vtable 0x01264F0C (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroAttachableAppearanceModifiers or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCHeroAttachableAppearanceModifiers@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHeroAttachableAppearanceModifiers.h (opt-ins as in the landed row global_GetArmourWeightsFromThing_00868e50.cpp).
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_VSLOTS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_STLP_LIST
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CArmourDef.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CCompareDefPointer.h"
#include "stlport/fable_map.h"
bool CTCHeroAttachableAppearanceModifiers::IsComponentSerialisable() const
{
    return true;
}
