// NScript::CQ_GuildTrainingScript::CMeleeApprentice::Main() -- Whisper, the melee apprentice: take scripted control, sheathe, join the hero's faction, unkillable; then while the thread runs give way while the master data or the quest wants her for a cut scene, run to the woods hero-walk marker when the woods Will quest is on (moaning about the scorpion when talked to), otherwise moan about the current training quest when talked to and walk back to her marker when she strays more than 4 from it
// Leaf: ?Main@CMeleeApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D40CF0, 3380 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1); RELOCATION_MATCH.
// Identity (proven, two sources): slot 1 (Main) of the RTTI-less entity vtable 0x012CDF7C that the landed
//   NScript::CQ_GuildTrainingScript::CMeleeApprentice::Alloc 0x00D40C10 stores (slots: 0 0x00D40CD0, 1 this row, 2 Init 0x00D40CC0,
//   3 GetParentScript 0x00D40CB0, 4-6 CEntityScriptBase defaults); Ego_r.pdb ?Main@CMeleeApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ.
// Layout: Ego_r.pdb CMeleeApprentice (40 bytes, entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, WaitingForFight
//   +0x1c, BadHit +0x20, WillWoodsChatDone +0x24 (hdr opt-in FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MELEEAPPRENTICE); master data
//   MeleeApprenticeNeededForCutscene +0xb0 (CQ_SunnyvaleMasterData.h), quest StartedMeleeTesting +0x5c (CQ_GuildTrainingScript.h).
// Callees: ScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 / ClearCommands 0x007E7360 / MoveToPosition 0x007E72F0 /
//   GetScriptThing 0x007E7490 / Speak(const char*) 0x007E7390 / IsPerformingScriptTask 0x007E7450 / dtor 0x007E74D0 (ctor expanded:
//   CBaseObject 0x0099A380), MovieBase ctor 0x006E7B60 / dtor 0x006E7B80, IsDistanceBetweenThingsOver 0x00CBE3EA,
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, ~CScriptThing 0x004AA840, CCharString 0x0099EBF0 / 0x0099EAE0;
//   CGameScriptInterfaceBase slots 7, 8, 70 GetHero, 72 GetThingWithScriptName, 264 GetHealth, 284 IsQuestActive, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities (inline CWideScreenMagicPauseEntities), 506 EntitySheatheWeapons, 517 EntitySetAsKillable,
//   521 EntitySetAllowBossPhaseChanges, 601 EntitySetInFaction; CScriptThing slots 6 GetPos, 27 MsgIsTalkedToBy, 67 SetFriendsWithEverythingFlag.
// Shape: the main loop is `for (;; NSF) { T; ... }` (gotcha 2376): VC7.1 rotates it and expands ~marker / ~seh_me only at the loop
//   exit pad, every other return calls the out-of-line dtors; `while (!T) { ... NSF; }` expands them at the first inner-wait return
//   instead (one extra jmp). GetPos is __forceinline (a plain inline stays a call under this function's budget).
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MELEEAPPRENTICE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_GuildTrainingScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1 };
inline CScriptThing::~CScriptThing() { }
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
#define MA_T if (IsActiveThreadTerminating()) return
#define MA_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        MA_T; \
    } \
    MA_T
#define MA_MOAN(key) { \
        CScriptGameResourceObjectMovieBase movie; \
        Game->StartMovieSequence("", movie); \
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game); \
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
            seh_me.Speak(Game->GetHero(), key, GROUP_SELECT_NONE, false, true, false); \
            while (seh_me.IsPerformingScriptTask()) { \
                Game->NewScriptFrame(); \
                MA_T; \
            } \
            MA_T; \
        } \
    }

void NScript::CQ_GuildTrainingScript::CMeleeApprentice::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    MA_START;
    Game->EntitySheatheWeapons(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_HERO");
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAllowBossPhaseChanges(Me, false);
    Me.SetFriendsWithEverythingFlag(true);
    CScriptThing marker = Game->GetThingWithScriptName("MeleeApprenticeMarker");
    for (;; Game->NewScriptFrame()) {
        MA_T;
        if (PMasterData->MeleeApprenticeNeededForCutscene) {
            MA_T;
            if (seh_me.IsValid()) seh_me.Release();
            while (PMasterData->MeleeApprenticeNeededForCutscene) {
                Game->NewScriptFrame();
                MA_T;
            }
            MA_T;
            MA_START;
        }
        if (ParentClass.StartedMeleeTesting) {
            MA_T;
            if (seh_me.IsValid()) seh_me.Release();
            while (ParentClass.StartedMeleeTesting) {
                Game->NewScriptFrame();
                MA_T;
            }
            MA_T;
            MA_START;
            seh_me.ClearCommands();
            Game->EntitySheatheWeapons(Me, false);
        }
        if (Game->IsQuestActive("Q_GuildTrainingWoodsWill")) {
            MA_T;
            if (!WillWoodsChatDone) {
                MA_T;
                seh_me.MoveToPosition(Game->GetThingWithScriptName("MK_GTM_WD_HEROWALK").GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
                WillWoodsChatDone = true;
            } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                MA_T;
                seh_me.ClearCommands();
                MA_MOAN("TEXT_QST_028_WHISPER_SCORPION_WOODS");
                seh_me.MoveToPosition(Game->GetThingWithScriptName("MK_GTM_WD_HEROWALK").GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            }
        } else {
            MA_T;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                MA_T;
                seh_me.ClearCommands();
                if (Game->IsQuestActive("Q_GuildTrainingSkill")) {
                    MA_T;
                    MA_MOAN("TEXT_QST_028_TEEN_WHISPER_SKILL_MOAN");
                } else if (Game->IsQuestActive("Q_GuildTrainingWill")) {
                    MA_T;
                    MA_MOAN("TEXT_QST_028_TEEN_WHISPER_WILL_MOAN");
                } else if (Game->IsQuestActive("Q_GuildTrainingDeparture")) {
                    MA_T;
                    if (Game->IsQuestActive("Q_GuildTrainingWoodsDeparture")) {
                        MA_T;
                        MA_MOAN("TEXT_QST_028_WHISPER_END_MOAN");
                    } else {
                        MA_T;
                        MA_MOAN("TEXT_QST_028_WHISPER_MELEE_MOAN");
                    }
                }
                seh_me.MoveToPosition(marker.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            }
            if (IsDistanceBetweenThingsOver(Me, marker, 4.0f) && !seh_me.IsPerformingScriptTask()) {
                MA_T;
                seh_me.MoveToPosition(marker.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            }
        }
    }
}