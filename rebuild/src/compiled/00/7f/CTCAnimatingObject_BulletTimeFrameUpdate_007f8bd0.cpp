// CTCAnimatingObject::BulletTimeFrameUpdate() -- the bullet-time update is the frame update (the slot-9 FrameUpdate)
// Leaf: ?BulletTimeFrameUpdate@CTCAnimatingObject@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8BD0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 10 of CTCAnimatingObject's vtable 0x012417E4 (the slot holds this body in 1 vtable(s), owner CTCAnimatingObject); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts BulletTimeFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?BulletTimeFrameUpdate@CTCAnimatingObject@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCAnimatingObject::vfunc_10 (rebuild/src/compiled/00/7f/CTCAnimatingObject_vfunc10_007f8bd0.cpp).
// Opt-ins as in the landed row CTCAnimatingObject_PlayDefaultAnimation_007f8f30.cpp.
#define FABLE_RETAIL_CTCANIMATINGOBJECT_VSLOTS2
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
void CTCAnimatingObject::BulletTimeFrameUpdate()
{
    FrameUpdate();
}
