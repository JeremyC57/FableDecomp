// CTCCreatureHive::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreatureHive@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D45F7, thiscall, `ret`.
// Identity: slot 8 of CTCCreatureHive's vtable 0x0123BDCC (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureHive or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureHive@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCCreatureHive.h (opt-ins as in the landed row CTCCreatureHive_Open_0077d230.cpp).
#define FABLE_RETAIL_CTCCREATUREHIVE_VSLOTS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMATETHING_INIT
#define FABLE_RETAIL_CANIMFILTER
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimateThing.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCCreatureHive.h"
bool CTCCreatureHive::HasFrameUpdate() const
{
    return true;
}
