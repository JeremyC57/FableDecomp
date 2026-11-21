// CThingManager::CopyHeroToHero(CThing& loaded_hero, CThing& hero, vector<CThing*>& loaded_things) -- carry the hero state from the freshly loaded hero thing over to the live hero: the appearance morph (after the live hero's morph particles are killed), hero stats, attachable appearance modifiers, the basic graphic appearance and hero experience are copied in through CThing::AddAndCopyTC; abilities and inventory stats through the live component's virtual Copy
// Leaf: ?CopyHeroToHero@CThingManager@@AAEXAAVCThing@@0AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051F0E0, length 595 bytes (+13 int3), thiscall `ret 0Ch` (`this` and loaded_things
//   unused). thing_manager.obj. Caller: CThingManager::LoadMainPlayerCreatureState 0x00521E42.
// Identity: Ego_r.pdb / FableWin.pdb ?CopyHeroToHero@CThingManager@@AAEXAAVCThing@@0AAV?$vector@PAVCThing@@..@std@@@Z (private). Ego_d.pdb
//   (debug length 710): parameters loaded_hero, hero, loaded_things; locals thing_to_load_into, tc_morph_base, tc_hero_aam, tc_experience,
//   tc_hero_stats, tc_stats, tc_abilities, ptc_graphic_appearance_basic_component, and in the blocks tc_hero_morph, old_tc_abilities,
//   old_tc_stats (the local types are the ones declared below).
// Callees: CInterfaceMap::LowerBound 0x0040F020 (the non-const GetTC lookups) and its const overload 0x004365B0 (the const PeekTC lookups),
//   CTCHeroMorph::KillParticles 0x0071C260 (Ego_r.pdb ?KillParticles@CTCHeroMorph@@QAEXXZ), CThing::AddAndCopyTC(const CTCBase&, bool)
//   0x004C9F60, the virtual Copy of CTCInventoryAbilities / CTCInventoryStats (slot 12, +0x30).
// Ego_d lines (VC10 /Od): 1241 (6 B) thing_to_load_into = hero; 1244 (25 B) the morph GetTC test; 1247 (364 B) the live hero's
//   bare GetTC(&tc_hero_morph), an oversize line (one file block, nothing inlined): the debug check FINDINGS 2026-11-20 (p25G) finds on
//   discarded-result getter statements, no retail code beyond the lookup; 1248 (8 B) KillParticles; 1249 (14 B) AddAndCopyTC; 1252 / 1257 / 1262 / 1267 (21 B) the PeekTC
//   tests and 1254 / 1259 / 1264 / 1269 (14 B) their AddAndCopyTC calls; 1273 / 1281 (21 B) the abilities / inventory-stats PeekTC tests,
//   1276 / 1284 (12 B) the live component's GetTC, 1277 / 1285 (17 B) the virtual Copy.
// Inferred: the seven out locals declared together at the top, uninitialised. Ego_d puts them at function scope (the three "old_"
//   / tc_hero_morph locals sit in the blocks) but uninitialised declarations have no line, so their place and order are not decoded;
//   declared at the point of use, or in the reverse order, they grade the same.
// Re-port (p25Q, 2026-11-21): the landed local models (a stand-in CTCBase with a CopyStateFrom slot, a local CThing interface bitset and
//   map, the "from / to / things" parameter names) replaced by the shared headers and Ego_d's names. CTCHeroAttachableAppearanceModifiers
//   gets its RTTI bases (CTCBase @0, NGraphicAppearance::CAGraphicAppearanceComponent @12) in an opt-in, so AddAndCopyTC takes it as a CTCBase.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASSFACTORY_ALLOCATECLASS
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_ISBINARY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
#define FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_DEAD
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ONACTIVATEENTITY
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CLEVELLOADER_LOADALL
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CTHINGMANAGER_LOADMAINPLAYERCREATURESTATE
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROMORPH
#define FABLE_RETAIL_CTCHEROMORPH_KILLPARTICLES
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_TCBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYSTATS
#include "stlport/fable_containers.h"
#include "engine/CThingManager.h"
#include "fable_game.h"
#include "engine/CWorld.h"
#include "engine/CThing.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CStringParser.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CQuestManager.h"
#include "engine/CLevelLoader.h"
#include "engine/CASuspendableProcess.h"
#include "engine/CTCBase.h"
#include "engine/CTCAppearanceMorphBase.h"
#include "engine/CTCHeroMorph.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCInventoryStats.h"
void CThingManager::CopyHeroToHero(CThing& loaded_hero, CThing& hero, std::vector<CThing*>& loaded_things)
{
    CThing& thing_to_load_into = hero;
    CTCAppearanceMorphBase* tc_morph_base;
    const CTCHeroAttachableAppearanceModifiers* tc_hero_aam;
    const CTCHeroExperience* tc_experience;
    const CTCHeroStats* tc_hero_stats;
    const CTCInventoryStats* tc_stats;
    const CTCInventoryAbilities* tc_abilities;
    const CTCBase* ptc_graphic_appearance_basic_component;
    if (loaded_hero.GetTC(TCI_APPEARANCE_MORPH, &tc_morph_base)) {
        CTCHeroMorph* tc_hero_morph;
        thing_to_load_into.GetTC(&tc_hero_morph);
        tc_hero_morph->KillParticles();
        thing_to_load_into.AddAndCopyTC(*tc_morph_base, true);
    }
    if (loaded_hero.PeekTC(TCI_HERO_STATS, &tc_hero_stats))
        thing_to_load_into.AddAndCopyTC(*tc_hero_stats, true);
    if (loaded_hero.PeekTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &tc_hero_aam))
        thing_to_load_into.AddAndCopyTC(*tc_hero_aam, true);
    if (loaded_hero.PeekTC(TCI_GRAPHIC_APPEARANCE_BASIC_COMPONENT, &ptc_graphic_appearance_basic_component))
        thing_to_load_into.AddAndCopyTC(*ptc_graphic_appearance_basic_component, true);
    if (loaded_hero.PeekTC(TCI_HERO_EXPERIENCE, &tc_experience))
        thing_to_load_into.AddAndCopyTC(*tc_experience, true);
    if (loaded_hero.PeekTC(TCI_HERO_ABILITIES, &tc_abilities)) {
        CTCInventoryAbilities* old_tc_abilities;
        thing_to_load_into.GetTC(&old_tc_abilities);
        old_tc_abilities->Copy(*tc_abilities);
    }
    if (loaded_hero.PeekTC(TCI_INVENTORY_STATS, &tc_stats)) {
        CTCInventoryStats* old_tc_stats;
        thing_to_load_into.GetTC(&old_tc_stats);
        old_tc_stats->Copy(*tc_stats);
    }
}
