// CTCAnimatingObject::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCAnimatingObject@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE87E, thiscall, `ret`.
// Identity: slot 8 of CTCAnimatingObject's vtable 0x012417E4 (defined here: the slot holds this body in 1 vtable(s), all of CTCAnimatingObject or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCAnimatingObject@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCAnimatingObject.h (opt-ins as in the landed row CTCAnimatingObject_PlayDefaultAnimation_007f8f30.cpp).
#define FABLE_RETAIL_CTCANIMATINGOBJECT_VSLOTS
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
#define FABLE_RETAIL_STLP_LIST
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
#include "engine/CTCAnimatingObject.h"
bool CTCAnimatingObject::HasFrameUpdate() const
{
    return true;
}
