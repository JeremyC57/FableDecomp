// CThingManager::LoadMainPlayerCreatureState(CCharString& old_script) -- swap a saved hero state into the live hero: the saved thing script (its unique ids remapped by a CThingScriptUIDReplacer) is loaded as version-2 entities of every section on the map level the live hero stands on, with NThingManager::HeroSwapInProgress set; the first loaded player creature is moved to the live hero's position, the loaded things are activated, and the loaded hero's state is copied into the live hero
// Leaf: ?LoadMainPlayerCreatureState@CThingManager@@QAEXAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00521CF0, length 446 bytes, thiscall `ret 4`. thing_manager.obj.
// Identity: Ego_r.pdb 0x0052BD50 / FableWin.pdb 0x01D37120 / Ego_d.pdb ?LoadMainPlayerCreatureState@CThingManager@@QAEXAAVCCharString@@@Z
//   (public; the catalog's "CThingManager_LoadMorphFromScript" is descriptive, NAME_FIXES). Ego_d (debug length 1053): parameter
//   old_script; locals uid_replacer, script, sections, loaded_things, pos, level, hero, loaded_hero, and in the loop it / loaded_thing.
// Callees: CThingScriptUIDReplacer(const CCharString&) 0x0083E1F0, CThingScriptUIDReplacer::GetReplacedUIDScript() const 0x0083E1B0
//   (Ego_r.pdb / FableWin.pdb; FableWin lists GetLoadAllSectionName and ~CThingScriptUIDReplacer right after this function), operator new
//   0x00BFEA0E (the set's header node), CCharString(const CCharString&) 0x0099EC30 (CLevelLoader::GetLoadAllSectionName, inline, from
//   CLevelLoader::LoadAllSectionName 0x013BAB08), set<CCharString>::insert 0x004AC380, ~CCharString 0x0099EAE0, CPlayerManager::GetMainPlayer
//   0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CThing::GetPos 0x004C73D0, CWorldMap::GetMapNumberAt(const C3DVector&) (virtual
//   +0x40), the same-TU CThingManager::LoadEntitiesFromScript 0x00520D00 / ActivateThings 0x0051E2F0 / CopyHeroToHero 0x0051F0E0,
//   CTCPhysicsBase::SetPosition (virtual +0x78), the set's erase 0x004ABA70, operator delete 0x00BFEA14. Data: NThingManager::HeroSwapInProgress
//   0x013B8A88 (Ego_d.pdb public ?HeroSwapInProgress@NThingManager@@3_NA; set at 0x00521DC0, cleared at 0x00521E49). Inline: GetPlayerManager
//   (+0x30), WorldMap (+0x28), GetPhysicsTC (+0x60), CClass::GetType (+0x10).
// Ego_d lines (VC10 /Od; VC7.1 /Od of this text in brackets): 1201 (19 B) [12] the replacer; 1202 (11 B) [11] script; 1205 (12 B) [8] the
//   set; 1206 (69 B) [29] the insert of GetLoadAllSectionName(); 1207 (12 B) [25] loaded_things; 1208 (41 B) [38] pos; 1209 (43 B) [26]
//   level (not decoded further: GetWorldMap() measures 29); 1210 (7 B) [7] the flag set; 1211 (33 B) [30] LoadEntitiesFromScript; 1214
//   (28 B) [25] hero; 1219 (7 B) [7] loaded_hero = NULL; 1220 (119 B) the for; 1222 (13 B) loaded_thing = *it; 1223 (13 B) [13] the
//   GetType() test; 1225 (6 B) [6] loaded_hero = loaded_thing; 1226 (2 B) [2] break; 1227 (2 B) [2] the loop's closing jump; 1230 (45 B)
//   the SetPosition statement; 1232 (15 B) ActivateThings (lea / push / this / call); 1233 (23 B) CopyHeroToHero (three pushes / this /
//   call); 1235 (7 B) [7] the flag cleared. Lines 1216 (216 B), 1228 (12 B) and 1229 (207 B) have no retail code (no locals, one file block,
//   nothing inlined), not decoded (likely asserts); retail goes straight from hero to the loop
//   (0x00521DDB) and from the loop exit to the SetPosition lookup (0x00521E0A). Not reproduced.
// Inferred: `WorldMap.GetMapNumberAt(pos)` reads the member directly. FableWin / Ego_d have the inline accessor
//   CThingManager::GetWorldMap(), and `GetWorldMap().GetMapNumberAt(pos)` grades RELOCATION_MATCH too; Ego_d's 43-B line 1209 fits neither
//   form (VC7.1 /Od 26 / 29 B, +2 for VC10's virtual call). GetPlayerManager() is not a choice: the direct PlayerManager member
//   grades DIFFER(445v446).
// Same TU: VC7.1 c2 keeps loaded_things._M_start in edi across the three same-TU calls (0x00521DDB .. 0x00521E47, the vector's
//   deallocate test) only when it sees their bodies: without the three copies below the row grades DIFFER(433v446), measured for each
//   copy separately.
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
#define FABLE_RETAIL_CTHING_ENTITYINITLEVEL
#define FABLE_RETAIL_CTHINGMANAGER_GETWORLD
#define FABLE_RETAIL_CWORLD_ABOUTTOLOADHEROSTATE
#define FABLE_RETAIL_CTHINGMANAGER_LOADMAINPLAYERCREATURESTATE
#define FABLE_RETAIL_THING_TYPE_IDS
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
#define FABLE_RETAIL_CTHINGMANAGER_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLDMAP
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
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorldMap.h"
#include "engine/CThingScriptUIDReplacer.h"
extern unsigned long g_LoadTickInterval;   // 0x01375714 (established spelling)
namespace NProgressDisplay {void __fastcall DisplayProgress(const CCharString&,float,bool,bool);}
// Fidelity: same-TU strong definition of 0x0051E2F0 (retail TU-mate)
void CThingManager::ActivateThings(const std::vector<CThing*>& activate_these_things)
{
    for (std::vector<CThing*>::const_iterator it = activate_these_things.begin(); it != activate_these_things.end(); ++it) {
        NProgressDisplay::DisplayProgress("Activate Things", -1.0f, false, false);
        CThing* pthing = *it;
        if (pthing->GetEntityInitLevel() == CThing::ENTITY_INIT_NONE) {
            if (pthing->GetType() == THING_TYPE_PLAYER_CREATURE && GetWorld().IsSerialisingAboutToLoadHeroState())
                continue;
            pthing->OnCreate();
        }
    }
    for (std::vector<CThing*>::const_iterator it = activate_these_things.begin(); it != activate_these_things.end(); ++it) {
        NProgressDisplay::DisplayProgress("Activate Things: OnCreate", -1.0f, false, false);
        CThing* pthing = *it;
        if (pthing->GetEntityInitLevel() == CThing::ENTITY_INIT_ON_CREATE) {
            if (pthing->GetType() == THING_TYPE_PLAYER_CREATURE && GetWorld().IsSerialisingAboutToLoadHeroState())
                continue;
            pthing->InitialActivate();
        }
    }
    CThing* phero = NULL;
    for (std::vector<CThing*>::const_iterator it = activate_these_things.begin(); it != activate_these_things.end(); ++it) {
        NProgressDisplay::DisplayProgress("Activate Things: Initial Activate", -1.0f, false, false);
        CThing* pthing = *it;
        if (pthing->GetEntityInitLevel() == CThing::ENTITY_INIT_INITIAL_ACTIVATE) {
            if (pthing->GetType() == THING_TYPE_PLAYER_CREATURE)
                phero = pthing;
            else
                pthing->OnInitialActivate();
        }
    }
    if (phero && !phero->IsDead() && !GetWorld().IsSerialisingAboutToLoadHeroState())
        phero->OnInitialActivate();
    NProgressDisplay::DisplayProgress("Activate Things End", -1.0f, false, false);
}
// Fidelity: same-TU strong definition of 0x0051F0E0 (retail TU-mate)
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
// Fidelity: same-TU strong definition of 0x00520D00 (retail TU-mate)
void CThingManager::LoadEntitiesFromScript(const CCharString& script,long version,long level,const std::set<CCharString>& sections,CASuspendableProcess* psuspend,long inclusions,std::vector<CThing*>& loaded_things)
{
    unsigned long how_many_is_some=g_LoadTickInterval;
    SerialiseVersionNumber=version;
    LevelBeingLoaded=level;
    bool loading_with_suspends=psuspend!=0;
    CStringParser parser(script,0,CCharString(""),0);
    long count=0;
    CurrentSectionName="";
    if(version>=2) {
        while(parser.SkipPastString("XXXSectionStart")) {
            parser.SkipWhitespace();
            CCharString name=parser.ReadAsStringUntilString(";");
            parser.SkipPastString(";");
            if(sections.find(name)!=sections.end() || sections.find(CLevelLoader::GetLoadAllSectionName())!=sections.end()) {
                CurrentSectionName=name;
                if(parser.SkipPastNextItemIfIs("XXXSectionEnd;")) {
                    parser.ReadAsStringUntilPastString(";");
                    CurrentSectionName="";
                } else {
                    while(parser.SkipPastNextItemIfIs("NewThing")) {
                        NProgressDisplay::DisplayProgress("Loading entities from script",-1.0f,false,false);
                        CPersistContext thing_context(parser,0,true,0,0,0);
                        CThing* pthing=LoadSingleThing(thing_context,inclusions);
                        if(pthing) loaded_things.push_back(pthing);
                        if(++count>=how_many_is_some) {
                            count=0;
                            if(loading_with_suspends)psuspend->SuspendSuspendableProcess();
                        }
                    }
                    parser.SkipPastString("XXXSectionEnd;");
                }
            } else parser.SkipPastString("XXXSectionEnd;");
        }
    } else {
        CurrentSectionName="NULL";
        while(parser.SkipPastString("NewThing")) {
            NProgressDisplay::DisplayProgress("Loading entities from script",-1.0f,false,false);
            CPersistContext thing_context(parser,0,true,0,0,0);
            CThing* pthing=LoadSingleThing(thing_context,inclusions);
            if(pthing) loaded_things.push_back(pthing);
            ++count;
            if(loading_with_suspends && count>=how_many_is_some) {
                count=0;
                psuspend->SuspendSuspendableProcess();
            }
        }
    }
    if(psuspend)psuspend->SuspendSuspendableProcess();
    ActivateAfterLoading(level);
}
void CThingManager::LoadMainPlayerCreatureState(CCharString& old_script)
{
    CThingScriptUIDReplacer uid_replacer(old_script);
    const CCharString& script = uid_replacer.GetReplacedUIDScript();
    std::set<CCharString> sections;
    sections.insert(CLevelLoader::GetLoadAllSectionName());
    std::vector<CThing*> loaded_things;
    C3DVector pos = GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetPos();
    long level = WorldMap.GetMapNumberAt(pos);
    NThingManager::HeroSwapInProgress = true;
    LoadEntitiesFromScript(script, 2, level, sections, NULL, 0, loaded_things);
    CThingPlayerCreature& hero = *GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    CThing* loaded_hero = NULL;
    for (std::vector<CThing*>::iterator it = loaded_things.begin(); it != loaded_things.end(); ++it) {
        CThing* loaded_thing = *it;
        if (loaded_thing->GetType() == THING_TYPE_PLAYER_CREATURE) {
            loaded_hero = loaded_thing;
            break;
        }
    }
    loaded_hero->GetPhysicsTC()->SetPosition(hero.GetPos());
    ActivateThings(loaded_things);
    CopyHeroToHero(*loaded_hero, hero, loaded_things);
    NThingManager::HeroSwapInProgress = false;
}