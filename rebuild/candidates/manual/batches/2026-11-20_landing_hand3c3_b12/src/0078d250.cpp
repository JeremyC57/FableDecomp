// CTCBerserk::FrameUpdate() -- on release: play the release action and, once it is accepted, make the noise, kill the start-up effect, attach the berserk opinion source, knock back everything hittable in the blast radius that is not a friendly creature, and put the creature into the berserk mode; afterwards: ramp the hero's berserk skin, tint and scale up over the release animation, count the ability timer down (held while paused or in a cutscene), keep the radial blur centred on the creature, clean the ability up when the timer runs out, and ramp skin, tint and scale back down over the last frames
// Leaf: ?FrameUpdate@CTCBerserk@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078D250, 1729 bytes, `ret`; CTCBase slot 9 of vtable 0x0126BB94.
// Retail-asm: 0x0078D6BC `fld dword ptr [esp+18h]` / 0x0078D6C0 `fistp dword ptr [esp+34h]` (and the same pair at 0x0078D89D /
//   0x0078D8A1): the shared engine/GFFloatToLongNear.h helper inside the two CRGBColour::Interpolate expansions (engine/CRGBColour.h);
//   no asm in this file.
// Identity: Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?FrameUpdate@CTCBerserk@@UAEXXZ; locals as Ego_d.pdb lists them (pcreature;
//   start_animation_finished; man, ptc_mode_manager, ptc_opinion_source; things, search_radius, center, search_area; endit, it;
//   hit_param, point_of_hit, something, direction; pos, sei; and in each of the two ramp blocks ptc_hero_morph, fraction,
//   ptc_graphic_appearance, berserk_skin_fraction, white, c, berserk_fraction), statements as its line table (lines 196..463).
//   The two ramp blocks have the same twelve line sizes (22 / 32 / 19 / 8 / 39 / 14 / 13 / 131 / 26 / 68 / 33 / 21 bytes): the last
//   two are `float berserk_fraction = (BerserkScale - 1.0f) * fraction + 1.0f;` (33: a store to a frame local) and
//   `SetScale(berserk_fraction)` (21) -- berserk_fraction is the scale, not the tint fraction (with it as a copy of `fraction` the
//   first block's SetScale argument is built in the outgoing slot instead of retail's `fstp [esp+14h] ; mov edx,[esp+14h] ; push edx`,
//   94 instructions off).
// Callees: CCreatureAction_ReleaseStrengthBerserk ctor 0x008E68E0, CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase
//   0x0062E900, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0, CThingCreatureBase::MakeNoise 0x00662110,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill 0x004C9B80, CTCBase::GetWorld 0x00686A70 / PeekThingSearchTools
//   0x00686D90, COpinionReactionManager::AttachOpinionSource 0x00700780, CThing::GetPos 0x004C73D0, __ftol2 0x00BFEA70, operator new
//   0x00BFEA0E, GetAllThingsInRoughArea<CThingGameObject, list, CFilter_Bind2<CThingFilter_In2DRange, CThingFilter_CanBeHitByEpicSpell>>
//   0x0078DB20, CTCEnemy::AreThingsEnemies 0x0076D2F0, CHitParameters ctor 0x00789238 / AddDamageWithAttribute 0x00868CA0 / dtor
//   0x00789358, CThingGameObject::OnHit (slot 65, +0x104), the list destructor 0x006FE660, the interface map LowerBound 0x0040F020,
//   CTCCreatureModeManager::IsModeActive 0x0073A8A0 / AddMode 0x0073AE10, CTCHeroMorph::SetBerserkSkin 0x0071BEE0,
//   GFGetInterpolatedColour 0x00A44250, CTCGraphicAppearance::SetTintColour 0x004C00B0 / SetScale 0x004BFA50,
//   CEnvironmentSpellEffectInterpolator::RadialBlurSetCenterWorldPos 0x006B8310, CTCBerserk::CleanupAbility 0x0078CD80,
//   CThing::RemoveTC 0x004C9840. GFastOneOverSqrtTable 0x013CE558.
// Inferred: (1) CRGBColour::Interpolate is expanded at both tint sites (`this` pushed first, the result built in a frame slot and
//   passed by `lea`); VC7.1 calls the plain inline (1683 bytes) and no /d2inlT value gives retail's split (T76..T160 expand the
//   list destructor retail calls, 1705 bytes; T200 1728; T300 1751), so the row sets FABLE_RETAIL_CRGBCOLOUR_INTERPOLATE_FORCEINLINE.
//   (2) FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY (the nine-argument constructor's body visible, kept out of line): without it the
//   two vectors handed to it count as escaped and the frame is laid out differently (1715 bytes, 126 instructions off).
//   (3) the mode manager is looked up on GetParentThing() (retail re-reads [this+4] at 0x0078D58B, not the creature register) and
//   its result is not tested (Ego_d line 282 is 491 bytes: a debug-checked statement); the mode literal 0x24 has no enum header.
//   (4) `AnimTimer < ReleasedAnimationTimer` (`test ah,5 ; jp`, 0x0078D600), a long against a float.
// Fidelity: the row object also carries GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind2<
//   CThingFilter_In2DRange, CThingFilter_CanBeHitByEpicSpell>> (retail 0x0078DB20, 596 bytes, compiled next to this row; the define
//   set of the landed row of that address) and the std::list destructor (retail 0x006FE660, 62 bytes, a folded COMDAT): both grade
//   RELOCATION_MATCH from this file (tools/gother.py). Its copy of the nine-argument CHitParameters constructor is 242 bytes, not
//   retail's 178-byte 0x00789238: that address is not 16-aligned, i.e. the copy the linker kept comes from a size-optimised TU.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_BULLETTIME_PASS23
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTACHOPINIONSOURCE
#define FABLE_RETAIL_CTCOPINIONSOURCE
#define FABLE_RETAIL_CTCOPINIONSOURCE_SETREACTNONSTOP
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCHEROMORPH_TATTOOTHINGS
#define FABLE_RETAIL_CTCHEROMORPH_SETBERSERKSKIN
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_INTERPOLATE
#define FABLE_RETAIL_CRGBCOLOUR_INTERPOLATE_FORCEINLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SCALE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_RELEASESTRENGTHBERSERK
#define FABLE_RETAIL_CTCBERSERK_HIT_EFFECT
#define FABLE_RETAIL_CTCBERSERK_FRAMEUPDATE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CDefClassBase.h"
#include "engine/CSpecialAbilitiesBerserkDef.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ReleaseStrengthBerserk.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CTCOpinionSource.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DExtentsI.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CThingFilter_CanBeHitByEpicSpell.h"
#include "engine/CThingSearchTools.h"
#include "engine/CTCEnemy.h"
#include "engine/CHitParameters.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCHeroMorph.h"
#include "engine/CRGBColour.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/ThingAlive.h"
#include "engine/CTCBerserk.h"

void CTCBerserk::FrameUpdate()
{
    CThingCreatureBase* pcreature = GFDebugPDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);
    if (JustReleased)
    {
        bool start_animation_finished = pcreature->SetCurrentAction(CCreatureAction_ReleaseStrengthBerserk(*pcreature));
        if (start_animation_finished)
        {
            JustReleased = false;
            pcreature->MakeNoise("SIMPLE_NOISE");
            if (IS_THING_ALIVE(StartUpEffect))
                StartUpEffect->Kill(true);
            const COpinionReactionManager& man = GetWorld().PeekOpinionReactionManager();
            CTCOpinionSource* ptc_opinion_source;
            if (man.AttachOpinionSource(*pcreature, "OPINION_SOURCE_BERSERK", &ptc_opinion_source))
                ptc_opinion_source->SetReactNonstop(true);
            if (SpellLevel >= 0)
            {
                float search_radius = PDef->BerserkBlastRadius;
                C3DVector center = pcreature->GetPos();
                C2DBoxI search_area(center.ToC2DCoordI(), (long)search_radius);
                std::list<CThingGameObject*> things;
                PeekThingSearchTools().GetAllThingsInRoughArea<CThingGameObject>(search_area,
                    GFFilter_Bind2(CThingFilter_In2DRange(center, search_radius), CThingFilter_CanBeHitByEpicSpell()), things);
                for (std::list<CThingGameObject*>::iterator it = things.begin(), endit = things.end(); it != endit; ++it)
                {
                    CThingGameObject* something = *it;
                    if (something != pcreature)
                    {
                        if (!(something->GetTypeFlags() & 8) || CTCEnemy::AreThingsEnemies(*pcreature, *something))
                        {
                            C3DVector direction = something->GetPos() - pcreature->GetPos();
                            direction.Normalise();
                            C3DVector point_of_hit = something->GetPos() - direction;
                            CHitParameters hit_param(0, pcreature, 0, point_of_hit, direction, true, false, false, false);
                            hit_param.AddDamageWithAttribute((EDamageAttribute)0, 0.001f);
                            something->OnHit(hit_param);
                        }
                    }
                }
            }
            CTCCreatureModeManager* ptc_mode_manager;
            GetParentThing().GetTC(&ptc_mode_manager);
            if (!ptc_mode_manager->IsModeActive(0x24))
                ptc_mode_manager->AddMode(0x24);
        }
    }
    else
    {
        AnimTimer++;
        if (AnimTimer < ReleasedAnimationTimer)
        {
            float fraction = (float)AnimTimer / ReleasedAnimationTimer;
            CTCHeroMorph* ptc_hero_morph;
            if (pcreature->GetTC(&ptc_hero_morph))
            {
                float berserk_skin_fraction = PDef->LowerBerserkPercent + MaxIncreaseInStrength;
                if (berserk_skin_fraction > 1.0f)
                    berserk_skin_fraction = 1.0f;
                ptc_hero_morph->SetBerserkSkin(berserk_skin_fraction * fraction);
            }
            NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = pcreature->GetTCGraphicAppearance();
            if (ptc_graphic_appearance)
            {
                CRGBColour c(PDef->TintColorR[SpellLevel], PDef->TintColorG[SpellLevel], PDef->TintColorB[SpellLevel]);
                CRGBColour white(255, 255, 255);
                ptc_graphic_appearance->SetTintColour(white.Interpolate(c, fraction), this);
                float berserk_fraction = (BerserkScale - 1.0f) * fraction + 1.0f;
                ptc_graphic_appearance->SetScale(berserk_fraction);
            }
        }
        if (!Paused || Timer < PDef->FramesToFadeOut)
        {
            if (!IsInCutscene)
                Timer--;
            else if (Timer >= PDef->FramesToFadeOut)
                Timer--;
        }
        C3DVector pos = pcreature->GetPos();
        CEnvironmentSpellEffectInterpolator& sei = GetWorld().GetEnvironment().GetSpellEffectInterpolator();
        sei.RadialBlurSetCenterWorldPos(RadialBlurFadeKey, C3DVector(pos.X, pos.Y, pos.Z + PDef->BlurCenterFromFloor));
        if (Timer < 0 && !IsInCutscene)
        {
            CleanupAbility(false);
            pcreature->RemoveTC(TCI_BERSERK);
        }
        else if (Timer < PDef->FramesToFadeOut)
        {
            float fraction = (float)Timer / (float)PDef->FramesToFadeOut;
            CTCHeroMorph* ptc_hero_morph;
            if (pcreature->GetTC(&ptc_hero_morph))
            {
                float berserk_skin_fraction = PDef->LowerBerserkPercent + MaxIncreaseInStrength;
                if (berserk_skin_fraction > 1.0f)
                    berserk_skin_fraction = 1.0f;
                ptc_hero_morph->SetBerserkSkin(berserk_skin_fraction * fraction);
            }
            NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = pcreature->GetTCGraphicAppearance();
            if (ptc_graphic_appearance)
            {
                CRGBColour c(PDef->TintColorR[SpellLevel], PDef->TintColorG[SpellLevel], PDef->TintColorB[SpellLevel]);
                CRGBColour white(255, 255, 255);
                ptc_graphic_appearance->SetTintColour(white.Interpolate(c, fraction), this);
                float berserk_fraction = (BerserkScale - 1.0f) * fraction + 1.0f;
                ptc_graphic_appearance->SetScale(berserk_fraction);
            }
        }
    }
}
