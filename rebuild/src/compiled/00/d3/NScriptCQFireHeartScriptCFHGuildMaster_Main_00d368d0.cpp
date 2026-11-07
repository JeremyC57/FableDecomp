// NScript::CQ_FireHeartScript::CFH_GuildMaster::Main() -- the Fireheart guildmaster: take scripted control and hand the handle to the quest (seh_GuildMaster); once the Scythe cut scene has played walk to FH_GuildMasterMarker, face FH_GuildMasterLookAt and show the information marker; until MissionSucceeded: when talked to say TEXT_QST_B01_GM_SPEECH, when hit by the hero (not a heal-life spell) say TEXT_QST_B01_GM_ATTACKED (both under a paused speech movie), else when his 15-30 s timer has run out and the hero is within 7 units call him over with the next TEXT_QST_B01_GM_ENTER_DOORn line
// Leaf: ?Main@CFH_GuildMaster@CQ_FireHeartScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D368D0, 2147 bytes, thiscall, plain `ret`; the last pad ends on a `jmp` back to the shared
//   return tail (0x00D3709B).
// Identity (proven): slot 1 (Main) of the CFH_GuildMaster entity vtable stored by the landed Alloc 0x00D36790 (Init 0x00D36860,
//   GetParentScript 0x00D36830 landed); Ego_r.pdb ?Main@CFH_GuildMaster@CQ_FireHeartScript@NScript@@EAEXXZ.
// Callees: CBaseObject 0x0099A380 / operator= 0x0099A3B0 / ~ 0x0099A430, ScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   dtor 0x007E74D0 / GetScriptThing 0x007E7490 / MoveToPosition 0x007E72F0 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450,
//   CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::ShareData 0x008AA8E0 (the PImp share of the expanded implicit
//   operator=; FableWin.pdb has the symbol), MovieBase dtor 0x006E7B80 (ctor expanded), CTimer 0x00CD4450 / ~ 0x00CD4470 (operator= /
//   GetTime expanded over pDSTGame 0x0143E8F8), ~CScriptThing 0x004AA840, CScriptThing::NullCoord 0x0143E8E0 (GetPos expanded),
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690,
//   IsDistanceFromThingToPositionOver 0x00CBE45C / IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), rand 0x00BFEB16,
//   SetPredicate-free (no predicate); IsActiveThreadTerminating 0x00F35B30.
// Layout: engine/CQ_FireHeartScript.h (seh_GuildMaster +0x174, MissionSucceeded +0x49, HasScytheCutscenePlayed +0x1a1);
//   Ego_r.pdb CFH_GuildMaster (32 bytes = retail new size): ParentClass +0x14 (pointer), PMasterData +0x18, NextLine +0x1c.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectScriptedThingBase;
template <> void CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::ShareData(CScriptGameResourceObjectScriptedThingBase*, CCPPointerInfo*);   // 0x008AA8E0
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
#include "engine/CQ_FireHeartScript.h"
#include "engine/CTimer.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_text.h"
#include <stdlib.h>
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);            // 0x00CBE45C
extern CGameScriptInterfaceBase* pDSTGame;                                                                   // 0x0143E8F8
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }

namespace NScript {
class CQ_FireHeartScript::CFH_GuildMaster : public CEntityScriptBase {
public:
    CQ_FireHeartScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    long NextLine;                                  // +0x1c
private:
    virtual void Main();
};
}

void NScript::CQ_FireHeartScript::CFH_GuildMaster::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->seh_GuildMaster = seh_me;
    while (!ParentClass->HasScytheCutscenePlayed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing marker = Game->GetThingWithScriptName("FH_GuildMasterMarker");
    {
        C3DVector pos = marker.GetPos();
        while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), pos, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            seh_me.MoveToPosition(pos, 0.0f, (EScriptEntityMoveType)0, false, true);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing look_at = Game->GetThingWithScriptName("FH_GuildMasterLookAt");
    Game->EntitySetFacingAngleTowardsThing(Me, look_at, false);
    Game->SetThingHasInformation(Me, false, true, false);
    {
    CTimer timer;
    timer = 5;
    while (!ParentClass->MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_B01_GM_SPEECH", (ETextGroupSelectionMethod)0, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                 (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_B01_GM_ATTACKED", (ETextGroupSelectionMethod)0, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        else if (timer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 7.0f)) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            CCharString line = "TEXT_QST_B01_GM_ENTER_DOOR" + GFIntToCharString(NextLine);
            if (++NextLine > 3)
                NextLine = 1;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, line, false, Me, Game->GetHero());
            timer = (long)((float)(rand() * 15.0f) * (1.0f / RAND_MAX) + 15.0f);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    }
}