// CTCPerceiveThing::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCPerceiveThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007460E0, thiscall, `ret`.
// Identity: slot 8 of CTCPerceiveThing's vtable 0x012688CC (defined here: the slot holds this body in 1 vtable(s), all of CTCPerceiveThing or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCPerceiveThing@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCPerceiveThing.h (opt-ins as in the landed row CTCPerceiveThing_OnCreate_00744ed0.cpp).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CPERCEIVEDTHINGDEF
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_RADII
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPerceivedThingDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCPerceiveThing.h"
bool CTCPerceiveThing::HasFrameUpdate() const
{
    return true;
}
