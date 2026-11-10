// CTCBulletTime::Pause() -- pause a pausable bullet time once: stop its looping sound, kill the looping particle effect, fade the screen filter out at once and clear the flag at +0x1c of the thing's TCI_CARRYING component
// Leaf: ?Pause@CTCBulletTime@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079CF30, length 228 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: Ego_r.pdb ?Pause@CTCBulletTime@@QAEXXZ (public; IsPaused / UnPause beside it); same TU as the landed StopSound 0x0079CEE0
//   and ImmediateCancel 0x0079D020.
// Callees: CTCBulletTime::StopSound(unsigned long) 0x0079CEE0 (private, Ego_r ?StopSound@CTCBulletTime@@AAEXK@Z), CBaseIntelligentPointer
//   GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (temporary vtable 0x0125A26C), NParticleEngine::
//   CParticleEmitter::Kill 0x00AEE8C0, CTCBase::GetWorld 0x00686A70, CEnvironmentSpellEffectInterpolator::ScreenFilterFadeOut(float,
//   void*) 0x006B83F0 (Ego_r name; proven by the CGameScriptInterface::ScreenFilterFadeOut forwarder 0x00890200, which tail-jumps
//   there; catalog label `_Cons_val<...>` wrong), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (GetTC, TCI_CARRYING 70).
// Layout: engine/CTCBulletTime.h (FABLE_RETAIL_CTCBULLETTIME + new FABLE_RETAIL_CTCBULLETTIME_MEMBERS: the Ego_r member names,
//   m_tpLoopingEffect +0x30 as CIntelligentPointer<NParticleEngine::CParticleEmitter>, LoopSound +0x38, ScreenFilterKey +0x50, Pausable
//   +0x54, Paused +0x55), engine/CTCBase.h (GetWorld, GetParentThing), engine/CIntelligentPointer.h (retail branch + ASSIGN),
//   engine/CParticleEmitter.h (new FABLE_RETAIL_CPARTICLEEMITTER_KILL: IsDead / Kill), engine/CTCCarrying.h (new
//   FABLE_RETAIL_CTCCARRYING_BYPASSFILTERS: BypassFilters +0x1c), engine/CThing.h, engine/CWorld.h, engine/CEnvironment.h (+0x1964
//   SpellEffectInterpolator_ptr), engine/CEnvironmentSpellEffectInterpolator.h.
// Shape: the screen filter fade is one call expression through the FableWin inline accessors GetWorld().GetEnvironment()
//   .GetSpellEffectInterpolator() (new opt-ins FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR / ..._FADEOUT, hdr/README.md):
//   the world chain is read after GetWorld and before the pushes, and the component lookup's key address lands in ecx
//   (`lea ecx,[esp+4]`, 0x0079CFE3); the parked named `CEnvironment* environment` local put it in eax.
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

void CTCBulletTime::Pause()
{
    if (!Pausable || Paused)
        return;
    Paused = true;
    StopSound(LoopSound);
    if (m_tpLoopingEffect.Get() && !m_tpLoopingEffect.Get()->IsDead()) {
        m_tpLoopingEffect.Get()->Kill();
        m_tpLoopingEffect = CIntelligentPointer<NParticleEngine::CParticleEmitter>(0);
    }
    GetWorld().GetEnvironment().GetSpellEffectInterpolator().ScreenFilterFadeOut(0.0f, ScreenFilterKey);
    CTCCarrying* carrying;
    if (GetParentThing().GetTC(TCI_CARRYING, &carrying))
        carrying->BypassFilters = false;
}