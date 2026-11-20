// CTCBulletTime::CTCBulletTime(CThing& thing) -- start slow time: fetch the def, take the hero's time-spell level for duration and multiplier, make the cast noise, start the init and loop sounds and the 2D cast effect, register the slowdown, count the ticks, fade the screen filter in with the caster bypassed
// Leaf: ??0CTCBulletTime@@QAE@AAVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079D360, 1115 bytes, ebp frame, `ret 4`.
// Retail-asm: 0x0079D65F `fld dword ptr [ebp-8]` / 0x0079D662 `fadd dword ptr [ebp-0Ch]` / 0x0079D665 `fistp dword ptr [ebp+8]` ..
//   0x0079D679 `dec dword ptr [ebp+8]` -- the engine's inline-assembly ceiling, through the shared engine/GFFloatToLongCeil.h helper
//   (the tick count). No asm in this source.
// Source shape: Ego_d.pdb locals (thing, ptc_inventory_abilities, pcreature, effect, interp, flags, ptc_carrying, def_index,
//   pspecial_effects, pemitter) and line table (lines 48..109), one statement per line entry: 49 pcreature, 51 def_index, 52 GetDef,
//   55/56 duration and multiplier, 58 the looping effect cleared, 60..63 the hero's time-spell level, 65/66 multiplier and duration
//   again (in that order), 68 MakeNoise, 69/70 the two sounds, 72..79 the CAST effect, 83 the slowdown, 86 the tick count, 93/94
//   the bypass record, 95 interp, 102 the filter, 104..107 the carried things. The debug ASSERTs (lines 53, 87) leave nothing.
//   The second half is the landed UnPause 0x0079D7E0's text with ScreenFilterFadeInSecs for the fade time.
// Audit 2026-11-20: line 49 is 23 debug bytes = the checked debug cast GFDebugPDynamicCast (this row's /Od line is 23 with it, 14
//   with a static_cast; the landed sibling FrameUpdate 0x0079C880 spells it the same way); CDefClassBase::GetSubDef comes from the
//   shared engine/CDefClassBase_GetSubDef.h (the COMDAT this object emits equals retail 0x00669580, 182 / 182 B).
// The constructor's `thing` parameter is dead after CTCBase(thing) in retail (its frame slot holds the string temporaries):
//   everything after goes through pcreature = the parent thing (ebx, 0x0079D3DF).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#define FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
#define FABLE_RETAIL_CTCBULLETTIME_UNPAUSE
#define FABLE_RETAIL_CTCCARRYING_SETBYPASSFILTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADETO
#define FABLE_RETAIL_CTCBULLETTIME_CTOR
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_SETSLOWDOWN
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CParticleEmitter.h"
#include "engine/NUISystem_CManager.h"            // the full PDB EEngine2DLayer (ENGINE_2D_LAYER_SPELLS = 82)
#include "engine/CParticleEmitterManager.h"
#include "engine/ThingAlive.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCCarrying.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CBulletTimeManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatToLongCeil.h"

namespace NParticleEngine {
CParticleEmitterManager& __fastcall GetParticleEmitterManager();      // 0x00AEAAF0
}
float __fastcall GFVXToAX(float x);                                   // 0x009E1E10
float __fastcall GFVYToAY(float y);                                   // 0x009E1E40
extern double VIRTUAL_SCREEN_WIDTH;                                   // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                                  // 0x013961F0

CTCBulletTime::CTCBulletTime(CThing& thing)
    : CTCBase(thing), BulletTimeAbilityLevel(0), m_nLevel(0), PItemRemoved(0), HitCreatureBlur(0), RadialBlurKey(0), ScreenFilterKey(0),
      Pausable(true), Paused(false), CutSceneCancel(false)
{
    CThingCreatureBase* pcreature = GFDebugPDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);

    long def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_BULLET_TIME_DEF", 1);
    CGameDefinitionManager::Get().GetDef(def_index, &PDef);

    m_fDuration = PDef->SlowTimeDuration[m_nLevel];
    m_nTimeMultiplier = PDef->BulletTimeSlowdownMultiplier[m_nLevel];

    m_tpLoopingEffect = 0;

    const CTCInventoryAbilities* ptc_inventory_abilities = 0;
    if (pcreature->PeekTC(&ptc_inventory_abilities))
    {
        m_nLevel = ptc_inventory_abilities->GetAbilityLevel(HERO_ABILITY_TIME_SPELL);
    }

    m_nTimeMultiplier = PDef->BulletTimeSlowdownMultiplier[m_nLevel];
    m_fDuration = PDef->SlowTimeDuration[m_nLevel];

    pcreature->MakeNoise("SIMPLE_NOISE");
    PlaySoundW(PDef->InitSound, true);
    LoopSound = PlaySoundW(PDef->LoopSound, true);

    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    long effect = 0;
    if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("CAST", effect))
    {
        NParticleEngine::CParticleEmitter* pemitter = NParticleEngine::GetParticleEmitterManager().Create2DEmitter(effect,
            C2DVector(GFVXToAX((float)VIRTUAL_SCREEN_WIDTH) * 0.5f, GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT) * 0.5f),
            ENGINE_2D_LAYER_SPELLS, true, CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
        if (IS_THING_ALIVE(pemitter))
        {
            pemitter->SetAlwaysUpdate(true);
        }
    }

    GetWorld().GetBulletTimeManager().SetBulletTimeSlowdownMultiplier(m_nTimeMultiplier, this, 0.5f);

    // Inferred: the tick count is read off retail's bytes (0x0079D645 `fild dword ptr [ConstantFPS]` / `fmul dword ptr [esi+2Ch]`, then
    // the ceiling helper), not off Ego_d: Ego_d's line 86 is 20 debug bytes, a statement with one `this` load (this row's /Od form
    // is 44), so that tree's statement is another one; the operand order is byte-neutral (measured).
    BulletTimeRemainingTicks = GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * m_fDuration);

    NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers flags;
    flags.push_back(NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser(pcreature, true));
    CEnvironmentSpellEffectInterpolator& interp = GetWorld().GetEnvironment().GetSpellEffectInterpolator();

    ScreenFilterKey = interp.ScreenFilterFadeTo(PDef->ScreenFilterFadeInSecs,
        NScreenEffect::SCREEN_EFFECT_PRIORITY_SPELL,
        PDef->ScreenFilterAlpha[m_nLevel], PDef->ScreenFilterSaturation[m_nLevel], PDef->ScreenFilterContrast[m_nLevel],
        PDef->ScreenFilterBrightness[m_nLevel],
        CRGBFloatColour(PDef->ScreenFilterTintR[m_nLevel], PDef->ScreenFilterTintG[m_nLevel], PDef->ScreenFilterTintB[m_nLevel], 1.0f),
        flags);

    CTCCarrying* ptc_carrying = 0;
    if (pcreature->GetTC(&ptc_carrying))
    {
        ptc_carrying->SetAllCarriedThingsAsBypassFilter(ScreenFilterKey);
    }
}