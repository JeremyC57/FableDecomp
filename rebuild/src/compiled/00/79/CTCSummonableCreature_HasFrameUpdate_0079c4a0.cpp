// CTCSummonableCreature::HasFrameUpdate() const -- returns false
// Leaf: ?HasFrameUpdate@CTCSummonableCreature@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079C4A0, thiscall, `ret`.
// Identity: slot 8 of CTCSummonableCreature's vtable 0x0126C41C (defined here: the slot holds this body in 1 vtable(s), all of CTCSummonableCreature or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCSummonableCreature@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCSummonableCreature.h (opt-ins as in the landed row CGameScriptThing_IsSummonedCreature_008d08a0.cpp).
#define FABLE_RETAIL_CTCSUMMONABLECREATURE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTTHING
#define FABLE_RETAIL_CGAMESCRIPTTHING_QUERIES
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCSummonableCreature.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameScriptThing.h"
bool CTCSummonableCreature::HasFrameUpdate() const
{
    return false;
}
