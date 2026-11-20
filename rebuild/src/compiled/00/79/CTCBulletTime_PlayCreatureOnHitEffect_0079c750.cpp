// CTCBulletTime::PlayCreatureOnHitEffect(CThing* creature) -- remember the hit creature, start the bullet-time radial blur (the damage blur when the hero himself was hit, the normal one otherwise) with its fade, and centre the blur on the creature
// Leaf: ?PlayCreatureOnHitEffect@CTCBulletTime@@QAEXPAVCThingCreatureBase@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079C750, 298 bytes (+6 int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), CTCBase::GetWorld() 0x00686A70 (twice), the radial-blur manager's
//   add (0x006B8550: two zero words, threshold colour, length, intensity colour, fade, range, twist; returns the key) and
//   fade-out (0x006B82E0: fade seconds, key) members (descriptive names; manifest labels _Ucopy/_Cons_val are
//   propagated), CThing::GetPos() const 0x004C73D0, CEnvironmentSpellEffectInterface::SetRadialBlurCenterWorldPos
//   (const C3DVector&, bool) 0x00885110.
// Layout: engine/CTCBulletTime.h (PDef +0x20, HitCreatureBlur +0x44, RadialBlurKey +0x4c; retail = PDB);
//   CSpecialAbilitiesBulletTimeDef at PDB - 4: the RadialBlur* block +0xD4..+0xFC and the DamageRadialBlur* block
//   +0x100..+0x128; the world's +0x18 object holds the spell-effect interface at +0x1960 and the radial-blur manager at
//   +0x1964.
// Shape: the hit creature is stored through the intelligent pointer's inline operator= from a CIntelligentPointer
// Names (NF201, 2026-11-20): own symbol Ego_r.pdb 0x007A2170 + FableWin.pdb 0x026A85B0
//   ?PlayCreatureOnHitEffect@CTCBulletTime@@QAEXPAVCThingCreatureBase@@@Z (engine/CTCBulletTime.h declares it; landed caller 0x00667D30
//   binds it; was the CThing* form). Callees: Ego_r.pdb 0x006F44E0 + FableWin.pdb 0x0246CD10 ?RadialBlurFadeTo@CEnvironmentSpellEffectInterpolator@@
//   QAEPAXMW4ScreenEffectPriority@NScreenEffect@@ABVC3DVector@@M1MMM@Z (0x006B8550; was the stand-in CRadialBlurManager::AddRadialBlur),
//   Ego_r.pdb 0x006F4280 + FableWin.pdb 0x0246CEA0 ?RadialBlurFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z (0x006B82E0; was
//   CRadialBlurManager::FadeOutRadialBlur), Ego_r.pdb 0x008C4B60 + FableWin.pdb 0x02A56CC0 ?SetRadialBlurCenterWorldPos@
//   CEnvironmentSpellEffectInterface@@QAEXABVC3DVector@@_N@Z (0x00885110), reached through the FableWin inline accessors
//   GetWorld().GetEnvironment().GetSpellEffectInterface() / .GetSpellEffectInterpolator() (the CTCBulletTime::Pause 0x0079CF30 recipe).
//   Retail pushes `push 0 ; push 0` for (time, priority): time 0.0f and priority 0 (SCREEN_EFFECT_PRIORITY_OTHER in the FULL enum).
// Ported (NF201) from the local CThing / CIntelligentPointer / def / world models onto the shared headers.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#define FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
#define FABLE_RETAIL_CTCBULLETTIME_FRAME_PASS23
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERFACE
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADETO_VECTOR
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADEOUT
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERFACE_SETRADIALBLURCENTERWORLDPOS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"
#include "engine/CTCBulletTime.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterface.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"

void CTCBulletTime::PlayCreatureOnHitEffect(CThingCreatureBase* creature)
{
    HitCreatureBlur = CIntelligentPointer<CThing>(creature);
    CEnvironmentSpellEffectInterface& spell_effects = GetWorld().GetEnvironment().GetSpellEffectInterface();
    CEnvironmentSpellEffectInterpolator& interpolator = GetWorld().GetEnvironment().GetSpellEffectInterpolator();
    CThing* parent = ParentThing;
    if (HitCreatureBlur.Get() == parent) {
        RadialBlurKey = interpolator.RadialBlurFadeTo(0.0f, (NScreenEffect::ScreenEffectPriority)0, PDef->DamageRadialBlurThresholdCol,
                                                      PDef->DamageRadialBlurLength, PDef->DamageRadialBlurIntensityCol,
                                                      PDef->DamageRadialBlurFade, PDef->DamageRadialBlurRange, PDef->DamageRadialBlurTwist);
        interpolator.RadialBlurFadeOut(PDef->DamageRadialBlurFadeTimeSecs, RadialBlurKey);
    } else {
        RadialBlurKey = interpolator.RadialBlurFadeTo(0.0f, (NScreenEffect::ScreenEffectPriority)0, PDef->RadialBlurThresholdCol,
                                                      PDef->RadialBlurLength, PDef->RadialBlurIntensityCol,
                                                      PDef->RadialBlurFade, PDef->RadialBlurRange, PDef->RadialBlurTwist);
        interpolator.RadialBlurFadeOut(PDef->RadialBlurFadeTimeSecs, RadialBlurKey);
    }
    spell_effects.SetRadialBlurCenterWorldPos(HitCreatureBlur.Get()->GetPos(), true);
}
