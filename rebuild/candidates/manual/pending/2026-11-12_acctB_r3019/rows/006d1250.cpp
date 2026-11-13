// CTCWife::CheckAppearanceChanges(CThingPlayerCreature& hero) -- the wife notices changes in the husband's appearance: new clothing (by suit part), hair / beard / moustache (categories 6..8), hair hidden or shown (13), no body clothing (14) or no torso clothing (15), new tattoos (9) and a fatness change past the marriage def's threshold (10 fatter / 11 thinner), stamping each category's FrameNoticedChangeInAppearance and remembering every worn thing in ThingsSeenThePlayerWear
// Grade: RELOCATION_MATCH (1608v1608, VC7.1 RTM 13.10.3077, /GS, overlay hdr/)
// Inline-budget: __declspec(noinline) on the visible STLport const find<long> (hdr opt-in FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE,
//   _MSC_VER only) -- retail calls 0x00616580 at all three sites; the plain inline body is expanded at the loop-2 / tattoo sites (c2
//   inline trace: find scores 88 / 68 / 60 at the three sites, the two LowerBound sites retail EXPANDS score 63 / 60 and the one it
//   calls 100, so no /d2inlT<N> separates them; loop / guard spellings that lower the find frequencies also refuse the LowerBounds).
//   Declaration-only find keeps the calls but loses retail's forwarding of the tattoo key across ~CCharString (0x006D170F) and the
//   loop-1 covered_areas load order (0x006D132E).
// Leaf: ?CheckAppearanceChanges@CTCWife@@AAEXAAVCThingPlayerCreature@@@Z
// Flags: /GS
// Retail: Fable.exe 0x006D1250, 1608 bytes (`ret 4` at 0x006D1889 / 0x006D1895: two epilogue copies). Name: Ego_r.pdb / FableWin.pdb
//   ?CheckAppearanceChanges@CTCWife@@AAEXAAVCThingPlayerCreature@@@Z (private; Ego_r 0x007065B0 between FindAppropriateGift and Marry =
//   retail 0x006D1250 before the landed Marry 0x006D18A0); FableWin emits the inline AppearanceBool COMDAT right after it.
// Callees: CVectorMap::LowerBound 0x0040F020 (+ two expanded); malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes / vector);
//   CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList 0x00706800; CTCBase::GetDefinitionManager 0x006869D0 /
//   GetWorld 0x00686A70; CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040; CDefClassBase::GetSubDef<CAppearanceModifierDef>
//   0x0057FF92 / <CTattooDef> 0x005B1EFC; _Rb_tree<long,...>::find<long> 0x00616580 / insert_unique 0x00512DA0; CWorld::GetFrame
//   0x0049D870; CTCHeroMorph::GetTattooThings 0x0071DE20; CDefClassBase::GetInstantiationName (vtable slot 10, +0x28);
//   CDefStringTable::GetString 0x009D49B0 (global table 0x013CA828); CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410;
//   ~CCharString 0x0099EAE0. Inline: GFFloatAbs (FableWin ?GFFloatAbs@@YIMM@Z, the `and [m],7FFFFFFFh` at 0x006D181B).
// Containers: STLport 4.6 _tree.c insert_unique / _M_insert bodies visible (FABLE_RETAIL_STLP_MAP + _MAP_ORIGINAL_INSERT_BODY, as in
//   every retail TU that includes <set>): with the whole insert chain and find visible the keys passed by reference stay uncaptured.
// Shape (FITTED): the tattoo-site lookup is a named iterator (find before end(), retail 0x006D1734..0x006D173D); the fatness tail picks
//   both categories into named locals before the GetFrame call (two fcom tests ahead of the call, change kept on the x87 stack).
// TU note: /GS is this TU's (the landed Marry 0x006D18A0 of the same TU is /GS); no cookie here (no buffer).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCWIFE_MARRY
#define FABLE_RETAIL_CTCWIFE_APPEARANCEBOOL
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_FATNESS
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTATTOODEF_TYPENAME
#define FABLE_RETAIL_CTATTOODEF_MEMBERS
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_FIND
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CTattooDef.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CTCHeroMorph.h"
#include "engine/CTCHeroStats.h"
#include "engine/CHeroMarriageDef.h"
#include "engine/CWorld.h"
#include "engine/CTCWife.h"

inline float __fastcall GFFloatAbs(float f)
{
    *(unsigned long*)&f &= 0x7fffffff;
    return f;
}

void CTCWife::CheckAppearanceChanges(CThingPlayerCreature& hero)
{
    long covered_areas = 0;
    bool hair_visible = true;
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (hero.GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &modifiers)) {
        std::list<long> clothing;
        modifiers->GetAppearanceModifierList(clothing, APPEARANCE_CLOTHING);
        for (std::list<long>::iterator it = clothing.begin(), end = clothing.end(); it != end; ++it) {
            long def_index = *it;
            CDefPointer<const CThingObjectDef> object_def;
            CDefPointer<const CAppearanceModifierDef> modifier;
            if (GetDefinitionManager().GetDef(def_index, &object_def)) {
                if (object_def->GetSubDef(&modifier)) {
                    long covers = modifier->CoversBodyAreaFlags;
                    covered_areas |= covers;
                    std::set<long>::iterator seen = ThingsSeenThePlayerWear.find(*it);
                    if (seen == ThingsSeenThePlayerWear.end() && covers) {
                        FrameNoticedChangeInAppearance[modifier->SuitPart] = CWorld::GetFrame();
                        ThingsSeenThePlayerWear.insert(def_index);
                    }
                }
            }
        }
        std::list<long> hair;
        modifiers->GetAppearanceModifierList(hair, APPEARANCE_HAIR);
        for (std::list<long>::iterator it = hair.begin(), end = hair.end(); it != end; ++it) {
            long def_index = *it;
            CDefPointer<const CThingObjectDef> object_def;
            CDefPointer<const CAppearanceModifierDef> modifier;
            if (GetDefinitionManager().GetDef(def_index, &object_def)) {
                if (object_def->GetSubDef(&modifier)) {
                    if (hair_visible && (modifier->CoversBodyAreaFlags & 0x10))
                        hair_visible = false;
                    std::set<long>::iterator seen = ThingsSeenThePlayerWear.find(*it);
                    if (seen == ThingsSeenThePlayerWear.end() && (modifier->CoversBodyAreaFlags & ~covered_areas)) {
                        long category = (modifier->CoversBodyAreaFlags & 0x100) ? 8 : ((modifier->CoversBodyAreaFlags & 0x80) ? 7 : 6);
                        FrameNoticedChangeInAppearance[category] = CWorld::GetFrame();
                        ThingsSeenThePlayerWear.insert(*it);
                    }
                }
            }
        }
        if (AppearanceBool((EWifeAppearanceCategories)14) != ((covered_areas & 0xe) == 0)) {
            if (!AppearanceBool((EWifeAppearanceCategories)14)) {
                AppearanceBool((EWifeAppearanceCategories)14) = true;
                FrameNoticedChangeInAppearance[14] = CWorld::GetFrame();
            } else {
                AppearanceBool((EWifeAppearanceCategories)14) = false;
                FrameNoticedChangeInAppearance[14] = 0;
            }
        }
        if (AppearanceBool((EWifeAppearanceCategories)15) != ((covered_areas & 8) == 0)) {
            if (!AppearanceBool((EWifeAppearanceCategories)15)) {
                AppearanceBool((EWifeAppearanceCategories)15) = true;
                FrameNoticedChangeInAppearance[15] = CWorld::GetFrame();
            } else {
                AppearanceBool((EWifeAppearanceCategories)15) = false;
                FrameNoticedChangeInAppearance[15] = 0;
            }
        }
        if (AppearanceBool((EWifeAppearanceCategories)14))
            FrameNoticedChangeInAppearance[15] = 0;
    }
    if (!AppearanceBool((EWifeAppearanceCategories)13) && hair_visible) {
        AppearanceBool((EWifeAppearanceCategories)13) = true;
        FrameNoticedChangeInAppearance[13] = CWorld::GetFrame();
    } else if (AppearanceBool((EWifeAppearanceCategories)13) && !hair_visible) {
        AppearanceBool((EWifeAppearanceCategories)13) = hair_visible;
        FrameNoticedChangeInAppearance[13] = 0;
    }
    CTCHeroMorph* morph;
    if (hero.GetTC(TCI_APPEARANCE_MORPH, &morph)) {
        std::vector<CDefPointer<const CThingBaseDef> > tattoos;
        morph->GetTattooThings(tattoos);
        for (unsigned long i = 0; i < tattoos.size(); ++i) {
            CDefPointer<const CTattooDef> tattoo;
            tattoos[i]->GetSubDef(&tattoo);
            if (!(tattoo->CoversBodyAreaFlags & covered_areas)) {
                long def_index = GetWorld().PeekDefinitionManager().GetDefGlobalIndexFromName(tattoos[i]->GetInstantiationName());
                if (def_index > 0) {
                    std::set<long>::iterator seen = ThingsSeenThePlayerWear.find(def_index);
                    if (seen == ThingsSeenThePlayerWear.end()) {
                        FrameNoticedChangeInAppearance[9] = CWorld::GetFrame();
                        ThingsSeenThePlayerWear.insert(def_index);
                    }
                }
            }
        }
    }
    CTCHeroStats* stats;
    if (hero.GetTC(TCI_HERO_STATS, &stats)) {
        float change = stats->GetFatness() - LastFatnessChangePoint;
        if (GFFloatAbs(change) > PHeroMarriageDef->FatnessChangeThresholdForComment) {
            LastFatnessChangePoint = stats->GetFatness();
            long noticed = change > 0.0f ? 10 : 11;
            long cleared = change > 0.0f ? 11 : 10;
            FrameNoticedChangeInAppearance[noticed] = CWorld::GetFrame();
            FrameNoticedChangeInAppearance[cleared] = 0;
        }
    }
}
