// 0078cd80 CTCBerserk::CleanupAbility
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
// Leaf: ?CleanupAbility@CTCBerserk@@QAEX_N@Z
// Flags: /Gr
#define FABLE_RETAIL_BERSERK_CLEANUP_PASS29
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_ABILITYLIST
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_UNSET_INLINE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY_DISTANCE
#define FABLE_RETAIL_STLP_DISTANCE_INPUT_NOINLINE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADEOUT
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_REMOVETINT
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
#include "stlport/fable_map.h"
// The STLport equal_range owner at 00580B2F uses size optimization and calls
// the original upper and lower walks at 0057FED6 and 0057FEB1. The game
// method uses speed optimization. The specialization retains the library body.
namespace std {
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
pair<map<void*,float>::iterator, map<void*,float>::iterator>
_Rb_tree<void*, pair<void* const,float>, _Select1st<pair<void* const,float> >,
         less<void*>, allocator<pair<void* const,float> > >::equal_range(void* const& key)
{
    return pair<iterator,iterator>(lower_bound(key),upper_bound(key));
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
}
namespace std {
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline 
#ifdef _MSC_VER
__declspec(noinline)
#endif
ptrdiff_t FABLE_CDECL __distance(const map<void*,float>::iterator& first,const map<void*,float>::iterator& last,const input_iterator_tag&)
{
    ptrdiff_t n=0;
    map<void*,float>::iterator it(first);
    map<void*,float>::iterator end(last);
    while(it!=end) { it._M_node = _Rb_global_inst::_M_increment(it._M_node); ++n; }
    return n;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
template<> inline size_t _Rb_tree<void*, pair<void* const,float>, _Select1st<pair<void* const,float> >,less<void*>, allocator<pair<void* const,float> > >::erase(void* const& key) {
    pair<iterator,iterator> p=equal_range(key);
    size_t n=distance(p.first,p.second);
    erase(p.first,p.second);
    return n;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
}
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

#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#ifdef _MSC_VER
#pragma optimize("sy",on)
#endif
#include "engine/CTCHaste.h"
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CSpecialEffectsDef.h"

void CTCBerserk::CleanupAbility(bool immediate)
{
    CThingCreatureBase* pcreature=GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
    CTCCombat* ptc_combat;
    if(pcreature->GetTC(&ptc_combat)) {
        long combat_def_index=CMainGameComponent::Get()->PeekDefinitionManager().GetDefGlobalIndexFromName("HERO_MELEE_COMBAT_ABILITY_BERSERK_CRAZY_MOVE");
        ptc_combat->RemoveMeleeFlourishAbility(combat_def_index);
    }
    if(IS_THING_ALIVE(BerserkBodyEffect)) BerserkBodyEffect->Kill(true);
    CTCHeroMorph* ptc_hero_morph;
    if(pcreature->GetTC(&ptc_hero_morph)) ptc_hero_morph->SetBerserkSkin(0.0f);
    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=pcreature->GetTCGraphicAppearance();
    if(ptc_graphic_appearance) {
        ptc_graphic_appearance->RemoveTintColour(this);
        ptc_graphic_appearance->SetScale(1.0f);
    }
    for(std::vector<CIntelligentPointer<CThing> >::iterator it=DependentThings.begin();it!=DependentThings.end();++it) {
        CThing* pthing=it->Get();
        if(IS_THING_ALIVE(pthing)) pthing->Kill(true);
    }
    CTCHaste* ptc_haste;
    if(pcreature->GetTC(&ptc_haste)) ptc_haste->UnsetActionSpeedMultiplier(this);
    CEnvironmentSpellEffectInterpolator& interp=GetWorld().GetEnvironment().GetSpellEffectInterpolator();
    interp.RadialBlurFadeOut(immediate?0.0f:PDef->TimeForRadialBlurToFadeOut,RadialBlurFadeKey);
    if(!immediate) {
        CDefPointer<const CSpecialEffectsDef> pspecial_effects(NULL_DEF);
        long effect=0;
        if(PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("BERSERK_EXIT_EFFECT",effect))
            CTCSpecialAbilities::CreateParticleEffect(effect,pcreature->GetPos());
    }
    CTCSoundPlayer* ptc_sound_player;
    if(GetParentThing().GetTC(&ptc_sound_player)) {
        ptc_sound_player->StopAttachedSound(BerserkingLoopingSound);
        ptc_sound_player->SetExtraTag("");
        if(!immediate) switch(SpellLevel) {
        case 0: ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_NO_POWERUP",true,false,false,-1); break;
        case 1: ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_LEVEL_1",true,false,false,-1); break;
        case 2: ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_LEVEL_2",true,false,false,-1); break;
        case 3: ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_LEVEL_3",true,false,false,-1); break;

        }
    }
    CTCCreatureModeManager* ptc_mode_manager;
    pcreature->GetTC(&ptc_mode_manager);
    ptc_mode_manager->RemoveMode(0x24);
    pcreature->FinishCurrentAction();
    if(!immediate) pcreature->SetCurrentAction(CCreatureAction_PlayAnimation(*pcreature,"STRENGTH_BERSERK_EXIT",CSoundPair(0),false,true,true,0,0));
    GetWorld().GetOpinionReactionManager().ClearOpinionSource(*pcreature,true);
}