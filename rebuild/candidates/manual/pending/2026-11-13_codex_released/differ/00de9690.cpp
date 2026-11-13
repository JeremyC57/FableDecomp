// NScript::CQ_RansomVictimScript::CRansomHelper::Main() -- Ransom Victim, the ransom helper at the cave: wait for the hero (or the helper start); when hit turn bandit; play CS_RANSOM_CAVE_EXTERIOR_1 (father's guard alive) or _2 and open the demon door; then until the ransommer dies: walk home after being hit, answer the hero (_RESCUED_VICTIM_10 / _YET_TO_PAY_10); finally turn bandit
// Grade: DIFFER(3428v3412) -- md 128: the CIsThingAlive copy-inline blocker (GOTCHAS chunks 810 / 816): retail CALLS the CScriptThing copy 0x004ABE90 inside the expanded CIsThingAlive ctor (0x00DE96B9) while it expands the first by-value SetIsPushableByHero(Me) copy (0x00DE9A0E) and calls the second (0x00DE9C9E); FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (this source) expands the predicate copy too (the only structural residue, plus loop-align pads from the 16 extra bytes); without it (r3, md 160) every copy is called
// Next idea: none in plain C++ known (named local, temporary, for(;;) idle form, /d2inlS15 all leave it); land the rest when the copy split gets a spelling
// Leaf: ?Main@CRansomHelper@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE9690, 3412 bytes, thiscall, plain `ret` (three epilogues: the hit-forever exit 0x00DE9A93, the shared 0x00DE9D0C, the loop tail after the backward jmp).
// Identity (proven, two sources): slot 1 (Main) of the CRansomHelper entity vtable 0x012DD1E4 stored by its Alloc 0x00DE1D00; Ego_r.pdb and
//   FableWin.pdb ?Main@CRansomHelper@CQ_RansomVictimScript@NScript@@EAEXXZ.
// Model: the landed sibling CVictimsBrotherHelper::Main 0x00DEB280 (same script: defines, includes, CTimer bodies, movie COMDAT ctor/dtor
//   0x006E7B60 / 0x006E7B80). Fields (engine/CQ_RansomVictimScript.h, Ego_r.pdb): RescuedVictim +0x4B, RansommerDied +0x4C, RansomHelperStarted
//   +0x4D, RansomHelperSpoken +0x4E, RansomHelpersAttacked +0x50, ToldGuardToWaitOutside +0x51. Callees: PlayCutscene 0x00DE6730,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, IsDistanceFromThingToPositionUnder 0x00CBE4B7, CTimer 0x00CD4450 / ~ 0x00CD4470 (GetTime / = expanded
//   over pDSTGame 0x0143E8F8 slots 89 / 90), operator+ 0x0099F690 / 0x0099F600, CCharString::operator const char* 0x0099E4C0; Game slots 7, 8,
//   70, 72, 264, 370, 379, 425 OpenDoor, 477 SetThingPersistent, 601 EntitySetInFaction, 844 SetIsPushableByHero; Me slots 3 GetDataString,
//   7 GetHomePos, 21 / 41 / 42 hit tests, 27 MsgIsTalkedToBy, 75 IsAlive. The locals HitTimer / GoingHome are descriptive names.
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/fable_script_text.h"
bool FABLE_FASTCALL IsDistanceFromThingToPositionUnder(const CScriptThing&, const C3DVector&, float);   // 0x00CBE4B7
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
__forceinline CTimer& CTimer::operator=(long n){pDSTGame->SetTimer(TimerIndex,n);return *this;}
__forceinline long CTimer::GetTime() const{return pDSTGame->GetTimer(TimerIndex);}
__forceinline bool CTimer::operator==(long n) const{return GetTime()==n;}
void NScript::CQ_RansomVictimScript::CRansomHelper::Main()
{
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetInFaction(Me,"FACTION_HERO");
    while (!ParentClass.RansomHelperStarted && !IsDistanceBetweenThingsUnder(Me,Game->GetHero(),10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || ParentClass.RansomHelpersAttacked) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.RansomHelpersAttacked=true;
            Game->SetIsPushableByHero(Me,true);
            Game->EntitySetInFaction(Me,"FACTION_BANDITS");
            if (seh_me.IsValid()) seh_me.Release();
            while (true) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    if (seh_me.IsValid()) seh_me.Release();
    if (!ParentClass.RansomHelperStarted) {
        if (IsActiveThreadTerminating()) return;
        ParentClass.RansomHelperStarted=true;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Game->GetThingWithScriptName("VictimsFatherGuard").IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_EXTERIOR_1","NULL");
            ParentClass.ToldGuardToWaitOutside=true;
        } else {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_EXTERIOR_2","NULL");
        }
        Game->OpenDoor(Game->GetThingWithScriptName("DemonDoorDoor"));
        Game->SetThingPersistent(Game->GetThingWithScriptName("DemonDoorDoor"),true);
        ParentClass.RansomHelperSpoken=true;
    }
    while (!ParentClass.RansomHelperSpoken) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CTimer HitTimer;
    bool GoingHome=false;
    while (!ParentClass.RansommerDied) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || ParentClass.RansomHelpersAttacked) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.RansomHelpersAttacked=true;
            break;
        }
        if (Me.MsgIsHitBy("")) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            HitTimer=8;
            GoingHome=true;
        }
        if (GoingHome && HitTimer==0) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            if (!seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return;
                seh_me.MoveToPosition(Me.GetHomePos(),1.0f,(EScriptEntityMoveType)1,false,true);
            }
            if (IsDistanceFromThingToPositionUnder(Me,Me.GetHomePos(),2.0f)) {
                if (IsActiveThreadTerminating()) return;
                GoingHome=false;
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !GoingHome) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            if (ParentClass.RescuedVictim) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B06_"+Me.GetDataString()+"_RESCUED_VICTIM_10",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B06_"+Me.GetDataString()+"_YET_TO_PAY_10",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetIsPushableByHero(Me,true);
    Game->EntitySetInFaction(Me,"FACTION_BANDITS");
    if (seh_me.IsValid()) seh_me.Release();
    while (true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}
