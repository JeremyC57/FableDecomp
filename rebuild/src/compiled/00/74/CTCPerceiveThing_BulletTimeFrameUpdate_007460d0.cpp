// CTCPerceiveThing::BulletTimeFrameUpdate() -- the bullet-time update is the frame update (the slot-9 FrameUpdate)
// Leaf: ?BulletTimeFrameUpdate@CTCPerceiveThing@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007460D0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 10 of CTCPerceiveThing's vtable 0x012688CC (the slot holds this body in 1 vtable(s), owner CTCPerceiveThing); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts BulletTimeFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?BulletTimeFrameUpdate@CTCPerceiveThing@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCPerceiveThing::vfunc_10 (rebuild/src/compiled/00/74/CTCPerceiveThing_vfunc10_007460d0.cpp).
// Opt-ins as in the landed row CAIStateGroupBehaviourBase_UpdateFocusAttentionOnThingActive_009578a0.cpp (the CTCPerceiveThing part).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCPerceiveThing.h"
void CTCPerceiveThing::BulletTimeFrameUpdate()
{
    FrameUpdate();
}
