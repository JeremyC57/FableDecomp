// NScript::CQ_GuildTrainingScript::CheckFriendlyAttacks() -- the Guild guard thread: make every creature but the sparrows, Whisper and the maze rival friends with everything and unkillable; then each frame (re-applying that whenever the hero comes back into the Guild complex) watch for the hero hitting a friend with bare hands, a melee or a ranged weapon inside the complex, other than the rival in the maze: the first three times speak the next guild-seal warning, the fourth time (unless a skill or will test is running, which it waits out) play CS_GUILD_BADHERO with the fourth warning, then reset the warnings
// Grade: DIFFER(3402v3439) -- md=375: frame slot order -- retail puts maze at +0x18, the first creatures vector at +0x24 and the name temporary at +0x30; ours has the vector lowest (+0x18), the temporary at +0x24 and maze above; the rest of the diff is mostly the shifted offsets plus the release-copy layout
// Chunk 868: not attempted beyond the baseline (budget spent on the other rows; frame-slot order residue as chunk 865 left it)
// Next idea: find the scope/declaration shape that puts maze lowest (vector at function scope makes the frame 0xB4; a helper for the twice-repeated make-friendly loop is unproven -- no PDB member found)
// Leaf: ?CheckFriendlyAttacks@CQ_GuildTrainingScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D45060, 3439 bytes, thiscall, no EH frame.
// Identity (proven, two sources): CQ_GuildTrainingScript::Main 0x00D3BC60 spawns it under the thread-name literal
//   "CheckFriendlyAttacks" (engine/CQ_GuildTrainingScript.h, chunk 800); Ego_r.pdb / FableWin.pdb
//   ?CheckFriendlyAttacks@CQ_GuildTrainingScript@NScript@@QAEXXZ.
// Callees: CCharString 0x0099EBF0 / 0x0099EAE0 / operator!=(const char*) 0x0099E960, IsActiveThreadTerminating 0x00CB7940,
//   ~vector<CScriptThing> 0x008AC970 (some exits; others expanded with free 0x00BFEA14), ~CScriptThing 0x004AA840 (some exits;
//   others expanded with operator delete 0x00BFE9BC), scripted-thing ctor / IsValid / Release / dtor / operator= 0x007E72A0 /
//   0x00CD23B9 / 0x00CD2770 / 0x007E74D0 / 0x008ABD10, cast map ctor / dtor / operator[] 0x00CDBF70 / 0x00CDBFB0 / 0x00CD3D2E,
//   movie ctor / dtor 0x006E7B60 / 0x006E7B80, RunCutsceneMacro 0x00CBFB7D (__fastcall); CScriptThing slots GetDefName (2),
//   MsgIsHitBy (21), MsgIsHitBySpecialAbilityFrom (41), MsgIsHitByAnySpecialAbilityFrom (42), MsgHitFriendWithMeleeWeapon (47),
//   MsgHitFriendWithRangedWeapon (49), MsgHitFriendWithBareHands (51), SetFriendsWithEverythingFlag (67).
// Data: engine/CQ_GuildTrainingScript.h HeroWarnings +0x50; CQ_SunnyvaleMasterData GuildWarningOccuring +0xc0, SkillTestOccuring
//   +0xc1, WillTestOccuring +0xc2.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
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

inline bool CScriptThing::MsgIsHitBy(const CCharString& name) const { if (PImp.IsNull()) return false; return PImp->MsgIsHitBy(name); }
inline bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const { if (PImp.IsNull()) return false; return PImp->MsgIsHitBySpecialAbilityFrom(ability, name); }
inline bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const { if (PImp.IsNull()) return false; return PImp->MsgIsHitByAnySpecialAbilityFrom(name); }
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();

void NScript::CQ_GuildTrainingScript::CheckFriendlyAttacks()
{
    CScriptThing maze = Game->GetThingWithScriptName("PreMeleeMaze");
    {
        std::vector<CScriptThing> creatures;
        Game->GetAllCreaturesExcludingHero(creatures);
        for (unsigned long i = 0; i < creatures.size(); ++i) {
            if (IsActiveThreadTerminating()) return;
            bool friendly = creatures[i].GetDefName() != "CREATURE_BIRD_GUILD_SPARROW"
                && creatures[i].GetDefName() != "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE"
                && creatures[i].GetDefName() != "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"
                && creatures[i].GetDefName() != "CREATURE_RIVAL_HERO_MAZE";
            if (friendly) {
                if (IsActiveThreadTerminating()) return;
                creatures[i].SetFriendsWithEverythingFlag(true);
                Game->EntitySetAsKillable(creatures[i], false, true);
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (!Game->IsLevelLoaded("HeroGuildComplex")) {
            if (IsActiveThreadTerminating()) return;
            while (!Game->IsLevelLoaded("HeroGuildComplex")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            std::vector<CScriptThing> creatures;
            Game->GetAllCreaturesExcludingHero(creatures);
            for (unsigned long i = 0; i < creatures.size(); ++i) {
                if (IsActiveThreadTerminating()) return;
                bool friendly = creatures[i].GetDefName() != "CREATURE_BIRD_GUILD_SPARROW"
                    && creatures[i].GetDefName() != "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE"
                    && creatures[i].GetDefName() != "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"
                    && creatures[i].GetDefName() != "CREATURE_RIVAL_HERO_MAZE";
                if (friendly) {
                    if (IsActiveThreadTerminating()) return;
                    creatures[i].SetFriendsWithEverythingFlag(true);
                    Game->EntitySetAsKillable(creatures[i], false, true);
                }
            }
            if (IsActiveThreadTerminating()) return;
        }
        if ((Game->GetHero().MsgHitFriendWithBareHands() || Game->GetHero().MsgHitFriendWithMeleeWeapon()
                || Game->GetHero().MsgHitFriendWithRangedWeapon())
            && Game->IsLevelLoaded("HeroGuildComplex")
            && !maze.MsgIsHitBy("SCRIPT_NAME_HERO")
            && (!maze.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                || maze.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
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
                    CScriptGameResourceObjectScriptedThingBase seh_maze;
                    if (seh_maze.IsValid()) seh_maze.Release();
                    while (!Game->StartScriptingEntity(maze, seh_maze, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    if (seh_hero.IsValid()) seh_hero.Release();
                    while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
                    cast["HERO"] = seh_hero;
                    cast["MAZE"] = seh_maze;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CWideScreenMagicPauseEntities pause(*Game);
                    long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
                    Game->AddLineToConversation(conversation, "TEXT_QST_028_GUILD_SEAL_FOURTH_WARNING", false, Game->GetHero(), Game->GetHero());
                    Game->Pause(2.0f);
                    Game->FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_GUILD_BADHERO", cast, 0, 0, false, true);
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
