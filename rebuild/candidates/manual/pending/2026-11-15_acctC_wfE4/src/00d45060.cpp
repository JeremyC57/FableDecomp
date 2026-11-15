// NScript::CQ_GuildTrainingScript::CheckFriendlyAttacks() -- the Guild guard thread: make every creature but the sparrows, Whisper and the maze rival friends with everything and unkillable; then each frame (re-applying that whenever the hero comes back into the Guild complex) watch for the hero hitting a friend with bare hands, a melee or a ranged weapon inside the complex, other than the rival in the maze: the first three times speak the next guild-seal warning, the fourth time (unless a skill or will test is running, which it waits out) play CS_GUILD_BADHERO with the fourth warning, then reset the warnings
// Flags: /d2inlT53
// Inline-budget: per-row flag /d2inlT53 -- c2 inline trace (default threshold): retail CALLS ~vector<CScriptThing> 0x008AC970 /
//   ~CScriptThing 0x004AA840 at the first-loop friend-block exit (0x00D451D7, site score 69 / 65), the pre-wait exit 0x00D45463 (62)
//   and the two warning-branch exits 0x00D45813 / 0x00D45938 (72 / 72), all of which the default threshold expands, while it
//   EXPANDS every site scoring <= 52 (first-loop exits 0x00D450E2 / 0x00D45237, the loop-head pads 0x00D45CEE / 0x00D45D15, the
//   0x00D457FA pad). Window T53..T60 matches (T52 and T62 differ: the _Rb_tree dtor of mymap scores 60); T53 is the smallest.
// Leaf: ?CheckFriendlyAttacks@CQ_GuildTrainingScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D45060, 3439 bytes (0x00D45060..0x00D45DCE `ret`; 0x00D45DD0 is the next spawned thread),
//   thiscall, no EH frame.
// Identity (proven, two sources): CQ_GuildTrainingScript::Main 0x00D3BC60 spawns it under the thread-name literal
//   "CheckFriendlyAttacks" (engine/CQ_GuildTrainingScript.h, chunk 800); Ego_r.pdb / FableWin.pdb
//   ?CheckFriendlyAttacks@CQ_GuildTrainingScript@NScript@@QAEXXZ.
// Callees: CCharString 0x0099EBF0 / 0x0099EAE0 / operator!=(const char*) 0x0099E960, IsActiveThreadTerminating 0x00CB7940,
//   ~vector<CScriptThing> 0x008AC970 (some exits; others expanded with free 0x00BFEA14), ~CScriptThing 0x004AA840 (some exits;
//   others expanded with operator delete 0x00BFE9BC), scripted-thing ctor / IsValid / Release / dtor / operator= 0x007E72A0 /
//   0x00CD23B9 / 0x00CD2770 / 0x007E74D0 / 0x008ABD10, mymap ctor / operator[] 0x00CDBF70 / 0x00CD3D2E, its ~_Rb_tree 0x00CDBFB0 (ICF-folded; labelled ~map / ~CCutsceneCastMap),
//   movie ctor / dtor 0x006E7B60 / 0x006E7B80, RunCutsceneMacro 0x00CBFB7D (__fastcall); CScriptThing slots GetDefName (2),
//   MsgIsHitBy (21), MsgIsHitBySpecialAbilityFrom (41), MsgIsHitByAnySpecialAbilityFrom (42), MsgHitFriendWithMeleeWeapon (47),
//   MsgHitFriendWithRangedWeapon (49), MsgHitFriendWithBareHands (51), SetFriendsWithEverythingFlag (67).
// Data: engine/CQ_GuildTrainingScript.h HeroWarnings +0x50; CQ_SunnyvaleMasterData GuildWarningOccuring +0xc0, SkillTestOccuring
//   +0xc1, WillTestOccuring +0xc2.
// Local names: the Anniversary PDB (WellingtonGame-Xbox360-Test.4k.pdb S_REGREL32 of this function): Maze, AllThings (two),
//   seh_Hero, seh_Maze, speech_movie, mymap; PauseMyEntitiesPlease as the same-script CCombatApprentice::Main (0x00D4A270).
// Shape: EVIDENCED -- the main loop `for (;; Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return; ... }` (retail's
//   loop-head exit pad 0x00D45CC7 is the FIRST expanded ~CScriptThing (eax), the in-loop pads follow as ecx / edx / eax; with
//   `for (; !T; NSF)` the loop exit is generated last and two pads merge, 3400v3439); inline helper texts as CCombatApprentice::Main.
//   FITTED -- the re-friend loop after the level wait is a `while (i < AllThings.size()) { ... ++i; }` (exit = header / 2): with
//   the same `for` as the first loop its exits score 53 / 50 and expand where retail calls them (3667 bytes at T56).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "engine/EHeroAbility.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_GuildTrainingScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };

inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBySpecialAbilityFrom(ability, attacker); }
inline bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnySpecialAbilityFrom(attacker); }
inline CScriptThing::~CScriptThing() { }

void NScript::CQ_GuildTrainingScript::CheckFriendlyAttacks()
{
    CScriptThing Maze = Game->GetThingWithScriptName("PreMeleeMaze");
    {
        std::vector<CScriptThing> AllThings;
        Game->GetAllCreaturesExcludingHero(AllThings);
        for (unsigned long i = 0; i < AllThings.size(); ++i) {
            if (IsActiveThreadTerminating()) return;
            bool friendly = AllThings[i].GetDefName() != "CREATURE_BIRD_GUILD_SPARROW"
                && AllThings[i].GetDefName() != "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE"
                && AllThings[i].GetDefName() != "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"
                && AllThings[i].GetDefName() != "CREATURE_RIVAL_HERO_MAZE";
            if (friendly) {
                if (IsActiveThreadTerminating()) return;
                AllThings[i].SetFriendsWithEverythingFlag(true);
                Game->EntitySetAsKillable(AllThings[i], false, true);
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return;
        if (!Game->IsLevelLoaded("HeroGuildComplex")) {
            if (IsActiveThreadTerminating()) return;
            while (!Game->IsLevelLoaded("HeroGuildComplex")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            std::vector<CScriptThing> AllThings;
            Game->GetAllCreaturesExcludingHero(AllThings);
            unsigned long i = 0;
            while (i < AllThings.size()) {
                if (IsActiveThreadTerminating()) return;
                bool friendly = AllThings[i].GetDefName() != "CREATURE_BIRD_GUILD_SPARROW"
                    && AllThings[i].GetDefName() != "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE"
                    && AllThings[i].GetDefName() != "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"
                    && AllThings[i].GetDefName() != "CREATURE_RIVAL_HERO_MAZE";
                if (friendly) {
                    if (IsActiveThreadTerminating()) return;
                    AllThings[i].SetFriendsWithEverythingFlag(true);
                    Game->EntitySetAsKillable(AllThings[i], false, true);
                }
                ++i;
            }
            if (IsActiveThreadTerminating()) return;
        }
        if ((Game->GetHero().MsgHitFriendWithBareHands() || Game->GetHero().MsgHitFriendWithMeleeWeapon()
                || Game->GetHero().MsgHitFriendWithRangedWeapon())
            && Game->IsLevelLoaded("HeroGuildComplex")
            && !Maze.MsgIsHitBy("SCRIPT_NAME_HERO")
            && (!Maze.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                || Maze.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            if (HeroWarnings < 3) {
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
                switch (HeroWarnings) {
                case 0:
                    Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILD_SEAL_FIRST_WARNING", false, Game->GetHero(), Game->GetHero());
                    break;
                case 1:
                    Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILD_SEAL_SECOND_WARNING", false, Game->GetHero(), Game->GetHero());
                    break;
                case 2:
                    Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILD_SEAL_THIRD_WARNING", false, Game->GetHero(), Game->GetHero());
                    break;
                }
                ++HeroWarnings;
            } else {
                if (IsActiveThreadTerminating()) return;
                MasterScriptData->GuildWarningOccuring = true;
                if (!MasterScriptData->SkillTestOccuring && !MasterScriptData->WillTestOccuring) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectScriptedThingBase seh_Maze;
                    if (seh_Maze.IsValid()) seh_Maze.Release();
                    while (!Game->StartScriptingEntity(Maze, seh_Maze, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectScriptedThingBase seh_Hero;
                    if (seh_Hero.IsValid()) seh_Hero.Release();
                    while (!Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["HERO"] = seh_Hero;
                    mymap["MAZE"] = seh_Maze;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
                    Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILD_SEAL_FOURTH_WARNING", false, Game->GetHero(), Game->GetHero());
                    Game->Pause(2.0f);
                    Game->FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_GUILD_BADHERO", mymap, 0, 0, false, true);
                    Game->FixMovieSequenceCamera(false);
                } else {
                    if (IsActiveThreadTerminating()) return;
                    while (MasterScriptData->SkillTestOccuring || MasterScriptData->WillTestOccuring) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                HeroWarnings = 0;
                MasterScriptData->GuildWarningOccuring = false;
            }
        }
    }
}
