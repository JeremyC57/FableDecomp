// CTCClock::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCClock@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00732BC0, thiscall, `ret`.
// Identity: slot 8 of CTCClock's vtable 0x01267044 (defined here: the slot holds this body in 1 vtable(s), all of CTCClock or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCClock@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCClock.h (opt-ins as in the landed row CTCClock_OnInitialActivate_00732850.cpp).
#define FABLE_RETAIL_CTCCLOCK_VSLOTS
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
#include "engine/CTCClock.h"
bool CTCClock::HasFrameUpdate() const
{
    return true;
}
