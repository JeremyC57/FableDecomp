// CTCBulletTime::OnKill() -- the kill path: FinalCleanup
// Leaf: ?OnKill@CTCBulletTime@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079D7C0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 14 of CTCBulletTime's vtable 0x0126C49C (the slot holds this body in 1 vtable(s), owner CTCBulletTime); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCBulletTime@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCBulletTime::vfunc_14 (rebuild/src/compiled/00/79/gapscan_sub0079d7c0_0079d7c0.cpp).
// Opt-ins as in the landed row CTCBulletTime_HasFrameUpdate_0079dc80.cpp.
#define FABLE_RETAIL_CTCBULLETTIME_VSLOTS2
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#define FABLE_RETAIL_CTCCARRYING_BYPASSFILTERS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADEOUT
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CParticleEmitter.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCCarrying.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"
void CTCBulletTime::OnKill()
{
    FinalCleanup();
}
