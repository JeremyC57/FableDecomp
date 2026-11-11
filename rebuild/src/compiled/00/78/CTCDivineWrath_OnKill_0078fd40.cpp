// CTCDivineWrath::OnKill() -- the kill path: FinalCleanup
// Leaf: ?OnKill@CTCDivineWrath@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078FD40, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 14 of CTCDivineWrath's vtable 0x0126BE84 (the slot holds this body in 1 vtable(s), owner CTCDivineWrath); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCDivineWrath@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCDivineWrath::vfunc_14 (rebuild/src/compiled/00/78/gapscan_sub0078fd40_0078fd40.cpp).
// Opt-ins as in the landed row CTCDivineWrath_FinalCleanup_0078faf0.cpp.
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_DIVINE_FINALCLEANUP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#include "engine/CThingCreatureBase.h"
#include "engine/CTCDivineWrath.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/ThingAlive.h"
void CTCDivineWrath::OnKill()
{
    FinalCleanup();
}
