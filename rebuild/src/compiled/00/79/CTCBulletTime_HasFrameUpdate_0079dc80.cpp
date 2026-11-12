// CTCBulletTime::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCBulletTime@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079DC80, thiscall, `ret`.
// Identity: slot 8 of CTCBulletTime's vtable 0x0126C49C (defined here: the slot holds this body in 1 vtable(s), all of CTCBulletTime or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBulletTime@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCBulletTime.h (opt-ins as in the landed row CTCBulletTime_Pause_0079cf30.cpp).
#define FABLE_RETAIL_CTCBULLETTIME_VSLOTS
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
bool CTCBulletTime::HasFrameUpdate() const
{
    return true;
}
