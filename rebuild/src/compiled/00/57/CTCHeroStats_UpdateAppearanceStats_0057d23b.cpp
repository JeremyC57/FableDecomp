// 0057d23b CTCHeroStats::UpdateAppearanceStats
// Leaf: ?UpdateAppearanceStats@CTCHeroStats@@QAEX_N@Z
// Flags: /O2 /Oy
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CTHINGOBJECTDEF_SIZE_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_SPEED_OWNER_PASS27
#define FABLE_RETAIL_STLP_BITSET_TEST_SPEED_OWNER_PASS27
// Inline-budget: both calls expand HasTC and call bitset::test. Ordinary inline kept HasTC out of line at both sites.
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CBASE_CLASS_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#define FABLE_RETAIL_CHEROSTATSDEF_APPEARANCESCALING
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_COVERS
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_TCBASE
#define FABLE_RETAIL_CTCHEROMORPH
#define FABLE_RETAIL_HERO_MORPH_SERIALISE_PASS29
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTATTOODEF_MEMBERS
#define FABLE_RETAIL_CTATTOODEF_TYPEBALANCED
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_STLP_LIST
// The out-of-line vector destructor is rebuilt as its 35-byte owner at 00581353.
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_CTHINGOBJECTDEF
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_HERO_APPEARANCE_UPDATE_PASS30
#include "engine/CTCHeroStats.h"
#include "engine/CThing.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CTCHeroMorph.h"
#include "engine/CTattooDef.h"
#include "engine/CWorld.h"
#include "engine/GFLimit.h"
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
void CTCHeroStats::UpdateAppearanceStats(bool force_update)
{
    const CTCHeroAttachableAppearanceModifiers* ptc_clothing;
    if (!GetParentThing().PeekTC(&ptc_clothing))
        return;
    const CTCHeroMorph* ptc_hero_morph;
    if (!GetParentThing().PeekTC(&ptc_hero_morph))
        return;
    if (!force_update && FrameUpdatedClothingStats != 0 && ptc_clothing->GetFrameLastChanged() < (unsigned long)FrameUpdatedClothingStats &&
        FrameUpdatedTattooStats != 0 && ptc_hero_morph->GetFrameTattoosLastChanged() < FrameUpdatedTattooStats)
        return;
    Attractiveness = InherentAttractiveness;
    Scariness = InherentScariness;
    AppearanceGoodstrength = 0.0f;
    SoundRadiusMultiplier = 1.0f;
    VisibilityMultiplier = 1.0f;
    std::list<long> clothing_list;
    ptc_clothing->GetAppearanceModifierList(clothing_list, APPEARANCE_CLOTHING);
    ptc_clothing->GetAppearanceModifierList(clothing_list, APPEARANCE_HAIR);
    const CHeroStatsDef& hero_stats_def = GetDefinitionManager().PeekHeroStatsDef();
    for (std::list<long>::const_iterator it = clothing_list.begin(); it != clothing_list.end(); ++it) {
        CDefPointer<const CThingObjectDef> pdef;
        GetDefinitionManager().GetDef(*it, &pdef);
        CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
        pdef->GetSubDef(&pappearance_modifier_def);
        Attractiveness += pappearance_modifier_def->GetTypeBalancedAttractiveness(hero_stats_def.AppearanceModifierScaling);
        Scariness += pappearance_modifier_def->GetTypeBalancedScariness(hero_stats_def.AppearanceModifierScaling);
        AppearanceGoodstrength += pappearance_modifier_def->GetTypeBalancedGoodstrength(hero_stats_def.AppearanceModifierScaling);
        SoundRadiusMultiplier *= pappearance_modifier_def->SoundRadiusMultiplier;
        VisibilityMultiplier *= pappearance_modifier_def->VisibilityMultiplier;
    }
    std::vector<CDefPointer<const CTattooDef> > tattoos;
    ptc_hero_morph->GetTattoos(tattoos);
    for (long c1 = 0; c1 < tattoos.size(); ++c1) {
        CDefPointer<const CTattooDef> ptattoo = tattoos[c1];
        if (!ptc_clothing->IsWearingAppearanceModifierCoveringBodyArea(ptattoo->CoversBodyAreaFlags,
                                                                    APPEARANCE_CLOTHING)) {
            Attractiveness += ptattoo->GetTypeBalancedAttractiveness(hero_stats_def.AppearanceModifierScaling);
            Scariness += ptattoo->GetTypeBalancedScariness(hero_stats_def.AppearanceModifierScaling);
            AppearanceGoodstrength += ptattoo->GetTypeBalancedGoodstrength(hero_stats_def.AppearanceModifierScaling);
            VisibilityMultiplier *= ptattoo->VisibilityMultiplier;
        }
    }
    GFLimit(Attractiveness, -1.0f, 1.0f);
    GFLimit(Scariness, -1.0f, 1.0f);
    GFLimit(AppearanceGoodstrength, -2.0f, 2.0f);
    GFLimitLower(SoundRadiusMultiplier, 0.0f);
    GFLimitLower(VisibilityMultiplier, 0.0f);
    FrameUpdatedClothingStats = PeekWorld().GetFrame();
    FrameUpdatedTattooStats = PeekWorld().GetFrame();
}